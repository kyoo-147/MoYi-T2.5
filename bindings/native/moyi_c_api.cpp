#include "moyi_c_api.h"

#include <cstring>
#include <memory>
#include <sstream>
#include <string>

#include "moyi/context/context_retriever.h"
#include "moyi/learning/learning_memory.h"
#include "moyi/pipeline/session.h"
#include "moyi/runtimes/mock/mock_asr_adapter.h"
#include "moyi/runtimes/mock/mock_translation_adapter.h"
#include "moyi/safety/safety_checker.h"

namespace {

struct NativeSession {
  std::shared_ptr<moyi::TranslationSession> session;
};

std::string json_escape(const std::string& value) {
  std::string escaped;
  for (const auto ch : value) {
    if (ch == '\\') {
      escaped += "\\\\";
    } else if (ch == '"') {
      escaped += "\\\"";
    } else if (ch == '\n') {
      escaped += "\\n";
    } else {
      escaped += ch;
    }
  }
  return escaped;
}

std::shared_ptr<moyi::TranslationSession> make_session() {
  auto context = std::make_shared<moyi::LocalContextRetriever>();
  context->set_profile("factory_worker", "factory_worker");
  context->add_glossary_term({"cam bien an toan", "safety sensor", "machine_safety"});
  context->add_glossary_term({"dung may", "stop the machine", "machine_safety"});
  context->add_safety_term("dung may");

  auto safety = std::make_shared<moyi::RuleBasedSafetyChecker>();
  safety->add_rule({
      .keyword = "dung may",
      .severity = moyi::SafetySeverity::Critical,
      .requires_confirmation = true,
      .reason = "Contains stop-machine safety command",
  });

  auto asr = std::make_shared<moyi::runtimes::mock::MockASRAdapter>();
  auto translator = std::make_shared<moyi::runtimes::mock::MockTranslationAdapter>();
  moyi::ModelManifest manifest;
  manifest.id = "mock-vi-en";
  manifest.language_pair = {moyi::Language::Vietnamese, moyi::Language::English};
  asr->load(manifest);
  translator->load(manifest);

  return std::make_shared<moyi::TranslationSession>(
      moyi::TranslationSessionConfig{
          .session_id = "native-python",
          .default_language_pair = {moyi::Language::Vietnamese, moyi::Language::English},
          .default_profile_id = "factory_worker",
      },
      asr,
      translator,
      context,
      safety,
      std::make_shared<moyi::SimpleLearningMemory>());
}

std::string to_json(const moyi::TranslationResult& result) {
  std::ostringstream out;
  out << "{\"session_id\":\"" << json_escape(result.session_id) << "\",";
  out << "\"language_pair\":\"" << result.language_pair.code() << "\",";
  out << "\"source_text\":\"" << json_escape(result.source_text) << "\",";
  out << "\"translated_text\":\"" << json_escape(result.translated_text) << "\",";
  out << "\"overall_confidence\":" << result.confidence.overall << ",";
  out << "\"safety\":{\"flagged\":" << (result.safety.flagged ? "true" : "false")
      << ",\"severity\":\"" << moyi::to_string(result.safety.severity)
      << "\",\"requires_confirmation\":" << (result.safety.requires_confirmation ? "true" : "false")
      << ",\"reason\":\"" << json_escape(result.safety.reason) << "\"},";
  out << "\"learning_cards\":" << result.learning_cards.size() << "}";
  return out.str();
}

char* copy_string(const std::string& value) {
  auto* out = new char[value.size() + 1];
  std::memcpy(out, value.c_str(), value.size() + 1);
  return out;
}

}  // namespace

moyi_session_handle moyi_create_default_session(void) {
  return new NativeSession{.session = make_session()};
}

const char* moyi_process_text(moyi_session_handle handle, const char* text) {
  if (handle == nullptr || text == nullptr) {
    return copy_string("{\"error\":\"invalid_handle_or_text\"}");
  }
  auto* native = static_cast<NativeSession*>(handle);
  auto result = native->session->process_once({
      .text = std::string(text),
      .language_pair = {moyi::Language::Vietnamese, moyi::Language::English},
      .profile_id = "factory_worker",
  });
  if (!result.is_ok()) {
    return copy_string("{\"error\":\"" + json_escape(result.error().message) + "\"}");
  }
  return copy_string(to_json(result.value()));
}

void moyi_free_string(const char* value) {
  delete[] value;
}

void moyi_destroy_session(moyi_session_handle handle) {
  delete static_cast<NativeSession*>(handle);
}
