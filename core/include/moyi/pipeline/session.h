#pragma once

#include <functional>
#include <memory>
#include <string>

#include "moyi/context/context_retriever.h"
#include "moyi/learning/learning_memory.h"
#include "moyi/result.h"
#include "moyi/safety/safety_checker.h"
#include "moyi/translation/asr_adapter.h"
#include "moyi/translation/translation_adapter.h"

namespace moyi {

enum class SessionEventType {
  AudioChunkReceived,
  SpeechDetected,
  PartialTranscriptReady,
  FinalTranscriptReady,
  ContextRetrieved,
  PartialTranslationReady,
  FinalTranslationReady,
  SafetyWarningRaised,
  OutputReady,
  LearningCardsGenerated,
  ErrorRaised,
};

struct SessionEvent {
  SessionEventType type{SessionEventType::OutputReady};
  std::string message;
};

using SessionEventHandler = std::function<void(const SessionEvent&)>;

struct TranslationSessionConfig {
  std::string session_id{"demo-session"};
  LanguagePair default_language_pair{Language::Vietnamese, Language::English};
  std::string default_profile_id{"default"};
};

class TranslationSession final {
 public:
  TranslationSession(TranslationSessionConfig config,
                     std::shared_ptr<IASRAdapter> asr,
                     std::shared_ptr<ITranslationAdapter> translator,
                     std::shared_ptr<IContextRetriever> context,
                     std::shared_ptr<ISafetyChecker> safety,
                     std::shared_ptr<ILearningMemory> learning);

  void set_event_handler(SessionEventHandler handler);
  Result<TranslationResult> process_once(const UtteranceInput& input);
  Result<void> push_audio(const AudioChunk& chunk);
  Result<void> finish_stream();

 private:
  void emit(SessionEventType type, std::string message) const;
  Result<Transcript> resolve_transcript(const UtteranceInput& input);
  Confidence compose_confidence(const Transcript& transcript, const Translation& translation) const;

  TranslationSessionConfig config_;
  std::shared_ptr<IASRAdapter> asr_;
  std::shared_ptr<ITranslationAdapter> translator_;
  std::shared_ptr<IContextRetriever> context_;
  std::shared_ptr<ISafetyChecker> safety_;
  std::shared_ptr<ILearningMemory> learning_;
  SessionEventHandler handler_;
};

}  // namespace moyi
