#include <iostream>
#include <memory>
#include <string>

#include "moyi/context/context_retriever.h"
#include "moyi/learning/learning_memory.h"
#include "moyi/pipeline/session.h"
#include "moyi/runtimes/mock/mock_asr_adapter.h"
#include "moyi/runtimes/mock/mock_translation_adapter.h"
#include "moyi/safety/safety_checker.h"

namespace {

std::string read_arg(int argc, char** argv, const std::string& flag, std::string fallback) {
  for (int i = 1; i + 1 < argc; ++i) {
    if (argv[i] == flag) {
      return argv[i + 1];
    }
  }
  return fallback;
}

void print_usage() {
  std::cout << "MoYi Edge Translation CLI\n"
            << "Usage:\n"
            << "  moyi --text \"Dung may lai, kiem tra cam bien an toan.\" --pair vi-en\n";
}

}  // namespace

int main(int argc, char** argv) {
  if (argc == 1) {
    print_usage();
    return 0;
  }

  const auto text = read_arg(argc, argv, "--text", "Dung may lai, kiem tra cam bien an toan.");
  const auto profile = read_arg(argc, argv, "--profile", "factory_worker");

  auto context = std::make_shared<moyi::LocalContextRetriever>();
  context->set_profile(profile, "factory_worker");
  context->add_glossary_term({"cam bien an toan", "safety sensor", "machine_safety"});
  context->add_glossary_term({"dung may", "stop the machine", "machine_safety"});
  context->add_safety_term("dung may");
  context->add_safety_term("stop the machine");

  auto safety = std::make_shared<moyi::RuleBasedSafetyChecker>();
  safety->add_rule({
      .keyword = "dung may",
      .severity = moyi::SafetySeverity::Critical,
      .requires_confirmation = true,
      .reason = "Contains stop-machine safety command",
  });
  safety->add_rule({
      .keyword = "stop the machine",
      .severity = moyi::SafetySeverity::Critical,
      .requires_confirmation = true,
      .reason = "Contains stop-machine safety command",
  });

  auto asr = std::make_shared<moyi::runtimes::mock::MockASRAdapter>();
  auto translator = std::make_shared<moyi::runtimes::mock::MockTranslationAdapter>();
  auto learning = std::make_shared<moyi::SimpleLearningMemory>();

  moyi::ModelManifest mock_manifest;
  mock_manifest.id = "mock-vi-en";
  mock_manifest.runtime = moyi::RuntimeKind::Mock;
  mock_manifest.language_pair = {moyi::Language::Vietnamese, moyi::Language::English};
  asr->load(mock_manifest);
  translator->load(mock_manifest);

  moyi::TranslationSession session(
      {.session_id = "cli-demo",
       .default_language_pair = {moyi::Language::Vietnamese, moyi::Language::English},
       .default_profile_id = profile},
      asr,
      translator,
      context,
      safety,
      learning);

  auto result = session.process_once({
      .text = text,
      .language_pair = {moyi::Language::Vietnamese, moyi::Language::English},
      .profile_id = profile,
  });

  if (!result.is_ok()) {
    std::cerr << "{\"error\":{\"code\":\"" << result.error().code << "\",\"message\":\""
              << result.error().message << "\"}}\n";
    return 1;
  }

  const auto& value = result.value();
  std::cout << "{\n";
  std::cout << "  \"session_id\": \"" << value.session_id << "\",\n";
  std::cout << "  \"language_pair\": \"" << value.language_pair.code() << "\",\n";
  std::cout << "  \"source_text\": \"" << value.source_text << "\",\n";
  std::cout << "  \"translated_text\": \"" << value.translated_text << "\",\n";
  std::cout << "  \"overall_confidence\": " << value.confidence.overall << ",\n";
  std::cout << "  \"safety\": {\n";
  std::cout << "    \"flagged\": " << (value.safety.flagged ? "true" : "false") << ",\n";
  std::cout << "    \"severity\": \"" << moyi::to_string(value.safety.severity) << "\",\n";
  std::cout << "    \"requires_confirmation\": "
            << (value.safety.requires_confirmation ? "true" : "false") << ",\n";
  std::cout << "    \"reason\": \"" << value.safety.reason << "\"\n";
  std::cout << "  },\n";
  std::cout << "  \"learning_cards\": " << value.learning_cards.size() << "\n";
  std::cout << "}\n";

  return 0;
}
