#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "moyi/assets/glossary_loader.h"
#include "moyi/assets/model_manifest_loader.h"
#include "moyi/assets/safety_loader.h"
#include "moyi/context/context_retriever.h"
#include "moyi/learning/learning_memory.h"
#include "moyi/pipeline/session.h"
#include "moyi/runtimes/mock/mock_asr_adapter.h"
#include "moyi/runtimes/mock/mock_translation_adapter.h"
#include "moyi/safety/safety_checker.h"

namespace {

std::string read_arg(int argc, char** argv, const std::string& flag, std::string fallback = "") {
  for (int i = 1; i + 1 < argc; ++i) {
    if (argv[i] == flag) {
      return argv[i + 1];
    }
  }
  return fallback;
}

bool has_arg(int argc, char** argv, const std::string& flag) {
  for (int i = 1; i < argc; ++i) {
    if (argv[i] == flag) {
      return true;
    }
  }
  return false;
}

std::string json_escape(const std::string& value) {
  std::string escaped;
  escaped.reserve(value.size());
  for (const auto ch : value) {
    switch (ch) {
      case '\\':
        escaped += "\\\\";
        break;
      case '"':
        escaped += "\\\"";
        break;
      case '\n':
        escaped += "\\n";
        break;
      case '\r':
        escaped += "\\r";
        break;
      case '\t':
        escaped += "\\t";
        break;
      default:
        escaped += ch;
    }
  }
  return escaped;
}

moyi::LanguagePair parse_pair(const std::string& pair) {
  const auto dash = pair.find('-');
  if (dash == std::string::npos) {
    return {moyi::Language::Vietnamese, moyi::Language::English};
  }
  return {moyi::language_from_code(pair.substr(0, dash)), moyi::language_from_code(pair.substr(dash + 1))};
}

std::vector<std::string> read_lines(const std::string& path) {
  std::ifstream input(path);
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(input, line)) {
    if (!line.empty()) {
      lines.push_back(line);
    }
  }
  return lines;
}

std::shared_ptr<moyi::LocalContextRetriever> build_context(int argc,
                                                           char** argv,
                                                           const std::string& profile) {
  auto context = std::make_shared<moyi::LocalContextRetriever>();
  context->set_profile(profile, "factory_worker");

  const auto glossary_path = read_arg(argc, argv, "--glossary");
  if (!glossary_path.empty()) {
    auto terms = moyi::load_glossary_terms(glossary_path);
    if (terms.is_ok()) {
      for (const auto& term : terms.value()) {
        context->add_glossary_term(term);
      }
    }
  } else {
    context->add_glossary_term({"cam bien an toan", "safety sensor", "machine_safety"});
    context->add_glossary_term({"dung may", "stop the machine", "machine_safety"});
  }

  context->add_safety_term("dung may");
  context->add_safety_term("stop the machine");
  return context;
}

std::shared_ptr<moyi::RuleBasedSafetyChecker> build_safety(int argc, char** argv) {
  auto safety = std::make_shared<moyi::RuleBasedSafetyChecker>();
  const auto safety_path = read_arg(argc, argv, "--safety");
  if (!safety_path.empty()) {
    auto rules = moyi::load_safety_rules(safety_path);
    if (rules.is_ok()) {
      for (const auto& rule : rules.value()) {
        safety->add_rule(rule);
      }
      return safety;
    }
  }

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
  return safety;
}

std::shared_ptr<moyi::TranslationSession> build_session(int argc,
                                                        char** argv,
                                                        const std::string& profile,
                                                        const moyi::LanguagePair& pair) {
  auto asr = std::make_shared<moyi::runtimes::mock::MockASRAdapter>();
  auto translator = std::make_shared<moyi::runtimes::mock::MockTranslationAdapter>();
  auto learning = std::make_shared<moyi::SimpleLearningMemory>();

  moyi::ModelManifest manifest;
  const auto manifest_path = read_arg(argc, argv, "--manifest");
  if (!manifest_path.empty()) {
    auto loaded = moyi::load_model_manifest(manifest_path);
    manifest = loaded.is_ok() ? loaded.value() : moyi::ModelManifest{};
  }
  if (manifest.id.empty()) {
    manifest.id = "mock-" + pair.code();
    manifest.runtime = moyi::RuntimeKind::Mock;
    manifest.language_pair = pair;
  }
  asr->load(manifest);
  translator->load(manifest);

  return std::make_shared<moyi::TranslationSession>(
      moyi::TranslationSessionConfig{
          .session_id = "cli-demo",
          .default_language_pair = pair,
          .default_profile_id = profile,
      },
      asr,
      translator,
      build_context(argc, argv, profile),
      build_safety(argc, argv),
      learning);
}

std::string to_json(const moyi::TranslationResult& value) {
  std::ostringstream out;
  out << "{\n";
  out << "  \"session_id\": \"" << json_escape(value.session_id) << "\",\n";
  out << "  \"language_pair\": \"" << value.language_pair.code() << "\",\n";
  out << "  \"source_text\": \"" << json_escape(value.source_text) << "\",\n";
  out << "  \"translated_text\": \"" << json_escape(value.translated_text) << "\",\n";
  out << "  \"confidence\": {\n";
  out << "    \"asr\": " << value.confidence.asr << ",\n";
  out << "    \"translation\": " << value.confidence.translation << ",\n";
  out << "    \"overall\": " << value.confidence.overall << "\n";
  out << "  },\n";
  out << "  \"safety\": {\n";
  out << "    \"flagged\": " << (value.safety.flagged ? "true" : "false") << ",\n";
  out << "    \"severity\": \"" << moyi::to_string(value.safety.severity) << "\",\n";
  out << "    \"requires_confirmation\": " << (value.safety.requires_confirmation ? "true" : "false") << ",\n";
  out << "    \"reason\": \"" << json_escape(value.safety.reason) << "\"\n";
  out << "  },\n";
  out << "  \"context_used\": {\n";
  out << "    \"profile_id\": \"" << json_escape(value.context_used.profile_id) << "\",\n";
  out << "    \"role\": \"" << json_escape(value.context_used.role) << "\",\n";
  out << "    \"glossary_terms\": " << value.context_used.glossary_terms.size() << ",\n";
  out << "    \"safety_terms\": " << value.context_used.safety_terms.size() << "\n";
  out << "  },\n";
  out << "  \"learning_cards\": " << value.learning_cards.size() << "\n";
  out << "}\n";
  return out.str();
}

int run_demo(int argc, char** argv) {
  const auto text = read_arg(argc, argv, "--text", "Dung may lai, kiem tra cam bien an toan.");
  const auto profile = read_arg(argc, argv, "--profile", "factory_worker");
  const auto pair = parse_pair(read_arg(argc, argv, "--pair", "vi-en"));
  auto session = build_session(argc, argv, profile, pair);

  auto result = session->process_once({
      .text = text,
      .language_pair = pair,
      .profile_id = profile,
  });
  if (!result.is_ok()) {
    std::cerr << "{\"error\":{\"code\":\"" << json_escape(result.error().code)
              << "\",\"message\":\"" << json_escape(result.error().message) << "\"}}\n";
    return 1;
  }
  std::cout << to_json(result.value());
  return 0;
}

int run_benchmark(int argc, char** argv) {
  const auto samples_path = read_arg(argc, argv, "--bench-samples", "assets/samples/factory_utterances_vi.txt");
  const auto profile = read_arg(argc, argv, "--profile", "factory_worker");
  const auto pair = parse_pair(read_arg(argc, argv, "--pair", "vi-en"));
  auto samples = read_lines(samples_path);
  auto session = build_session(argc, argv, profile, pair);

  const auto start = std::chrono::steady_clock::now();
  std::size_t safety_flags = 0;
  std::size_t cards = 0;
  for (const auto& sample : samples) {
    auto result = session->process_once({.text = sample, .language_pair = pair, .profile_id = profile});
    if (result.is_ok()) {
      safety_flags += result.value().safety.flagged ? 1 : 0;
      cards += result.value().learning_cards.size();
    }
  }
  const auto end = std::chrono::steady_clock::now();
  const auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
  const auto avg_ms = samples.empty() ? 0.0 : elapsed_ms / static_cast<double>(samples.size());

  std::cout << "{\n";
  std::cout << "  \"mode\": \"benchmark\",\n";
  std::cout << "  \"runtime\": \"mock\",\n";
  std::cout << "  \"language_pair\": \"" << pair.code() << "\",\n";
  std::cout << "  \"samples\": " << samples.size() << ",\n";
  std::cout << "  \"elapsed_ms\": " << elapsed_ms << ",\n";
  std::cout << "  \"avg_ms\": " << avg_ms << ",\n";
  std::cout << "  \"safety_flags\": " << safety_flags << ",\n";
  std::cout << "  \"learning_cards\": " << cards << "\n";
  std::cout << "}\n";
  return 0;
}

void print_usage() {
  std::cout << "MoYi Edge Translation CLI\n"
            << "Usage:\n"
            << "  moyi --text \"Dung may lai, kiem tra cam bien an toan.\" --pair vi-en\n"
            << "  moyi --text \"...\" --glossary assets/glossary/factory_vi_en.json --safety assets/safety/factory_rules_vi_en.json\n"
            << "  moyi --bench-samples assets/samples/factory_utterances_vi.txt\n";
}

}  // namespace

int main(int argc, char** argv) {
  if (argc == 1 || has_arg(argc, argv, "--help")) {
    print_usage();
    return 0;
  }
  if (has_arg(argc, argv, "--bench-samples")) {
    return run_benchmark(argc, argv);
  }
  return run_demo(argc, argv);
}
