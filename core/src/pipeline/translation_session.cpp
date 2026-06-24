#include "moyi/pipeline/session.h"

#include <utility>

namespace moyi {

TranslationSession::TranslationSession(TranslationSessionConfig config,
                                       std::shared_ptr<IASRAdapter> asr,
                                       std::shared_ptr<ITranslationAdapter> translator,
                                       std::shared_ptr<IContextRetriever> context,
                                       std::shared_ptr<ISafetyChecker> safety,
                                       std::shared_ptr<ILearningMemory> learning)
    : config_(std::move(config)),
      asr_(std::move(asr)),
      translator_(std::move(translator)),
      context_(std::move(context)),
      safety_(std::move(safety)),
      learning_(std::move(learning)) {}

void TranslationSession::set_event_handler(SessionEventHandler handler) {
  handler_ = std::move(handler);
}

std::vector<SessionEvent> TranslationSession::drain_events() {
  auto events = event_history_;
  event_history_.clear();
  return events;
}

Result<TranslationResult> TranslationSession::process_once(const UtteranceInput& input) {
  auto transcript_result = resolve_transcript(input);
  if (!transcript_result.is_ok()) {
    emit(SessionEventType::ErrorRaised, transcript_result.error().message);
    return Result<TranslationResult>::error(transcript_result.error());
  }
  auto transcript = transcript_result.value();
  emit(SessionEventType::FinalTranscriptReady, transcript.text);

  ContextQuery query{
      .profile_id = input.profile_id.empty() ? config_.default_profile_id : input.profile_id,
      .utterance = transcript.text,
      .language_pair = input.language_pair.source == Language::Unknown ? config_.default_language_pair
                                                                       : input.language_pair,
  };
  auto context_result = context_->retrieve(query);
  if (!context_result.is_ok()) {
    emit(SessionEventType::ErrorRaised, context_result.error().message);
    return Result<TranslationResult>::error(context_result.error());
  }
  auto context = context_result.value();
  emit(SessionEventType::ContextRetrieved, "context ready");

  auto translation_result = translator_->translate(transcript.text, query.language_pair, context);
  if (!translation_result.is_ok()) {
    emit(SessionEventType::ErrorRaised, translation_result.error().message);
    return Result<TranslationResult>::error(translation_result.error());
  }
  auto translation = translation_result.value();
  emit(SessionEventType::FinalTranslationReady, translation.text);

  auto confidence = compose_confidence(transcript, translation);
  auto safety_result = safety_->check(transcript.text, translation, context, confidence);
  if (!safety_result.is_ok()) {
    emit(SessionEventType::ErrorRaised, safety_result.error().message);
    return Result<TranslationResult>::error(safety_result.error());
  }
  auto safety = safety_result.value();
  if (safety.flagged) {
    emit(SessionEventType::SafetyWarningRaised, safety.reason);
  }

  auto learning_result = learning_->generate_cards(transcript, translation, context);
  if (!learning_result.is_ok()) {
    emit(SessionEventType::ErrorRaised, learning_result.error().message);
    return Result<TranslationResult>::error(learning_result.error());
  }
  emit(SessionEventType::LearningCardsGenerated, "learning cards generated");

  TranslationResult result{
      .session_id = config_.session_id,
      .language_pair = query.language_pair,
      .source_text = transcript.text,
      .translated_text = translation.text,
      .confidence = confidence,
      .safety = safety,
      .context_used = context,
      .learning_cards = learning_result.value(),
  };
  emit(SessionEventType::OutputReady, result.translated_text);
  return Result<TranslationResult>::ok(result);
}

Result<void> TranslationSession::push_audio(const AudioChunk& chunk) {
  emit(SessionEventType::AudioChunkReceived, "audio chunk received");
  if (chunk.final_chunk) {
    emit(SessionEventType::SpeechDetected, "final audio chunk");
    auto transcript = asr_->transcribe(chunk, config_.default_language_pair.source);
    if (!transcript.is_ok()) {
      emit(SessionEventType::ErrorRaised, transcript.error().message);
      return Result<void>::error(transcript.error());
    }
    emit(SessionEventType::FinalTranscriptReady, transcript.value().text);
  }
  return Result<void>::ok();
}

Result<void> TranslationSession::finish_stream() {
  emit(SessionEventType::OutputReady, "stream finished");
  return Result<void>::ok();
}

void TranslationSession::emit(SessionEventType type, std::string message) const {
  SessionEvent event{.type = type, .message = std::move(message)};
  event_history_.push_back(event);
  if (handler_) {
    handler_(event);
  }
}

Result<Transcript> TranslationSession::resolve_transcript(const UtteranceInput& input) {
  if (input.text.has_value()) {
    return Result<Transcript>::ok(Transcript{
        .text = *input.text,
        .language = input.language_pair.source,
        .confidence = 1.0,
        .is_final = true,
    });
  }

  if (input.audio.has_value()) {
    return asr_->transcribe(*input.audio, input.language_pair.source);
  }

  return Result<Transcript>::error(RuntimeError{
      .code = "missing_input",
      .message = "UtteranceInput must contain text or audio",
  });
}

Confidence TranslationSession::compose_confidence(const Transcript& transcript,
                                                  const Translation& translation) const {
  const auto overall = (transcript.confidence + translation.confidence) / 2.0;
  return Confidence{
      .asr = transcript.confidence,
      .translation = translation.confidence,
      .overall = overall,
  };
}

}  // namespace moyi
