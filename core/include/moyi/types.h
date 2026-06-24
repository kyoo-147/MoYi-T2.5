#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace moyi {

enum class Language {
  Unknown,
  Vietnamese,
  English,
  Korean,
  Chinese,
};

struct LanguagePair {
  Language source{Language::Unknown};
  Language target{Language::Unknown};

  [[nodiscard]] std::string code() const;
};

struct Confidence {
  double asr{0.0};
  double translation{0.0};
  double overall{0.0};
};

struct AudioChunk {
  std::vector<float> samples;
  std::uint32_t sample_rate_hz{16000};
  bool final_chunk{false};
};

struct Transcript {
  std::string text;
  Language language{Language::Unknown};
  double confidence{0.0};
  bool is_final{true};
};

struct Translation {
  std::string text;
  LanguagePair language_pair;
  double confidence{0.0};
};

struct ContextTerm {
  std::string source;
  std::string target;
  std::string domain;
};

struct RetrievedContext {
  std::string profile_id;
  std::string role;
  std::vector<ContextTerm> glossary_terms;
  std::vector<std::string> safety_terms;
};

enum class SafetySeverity {
  None,
  Advisory,
  Warning,
  Critical,
};

struct SafetyDecision {
  bool flagged{false};
  bool requires_confirmation{false};
  SafetySeverity severity{SafetySeverity::None};
  std::string reason;
};

struct LearningCard {
  std::string front;
  std::string back;
  std::string type;
  std::string source_utterance;
};

struct UtteranceInput {
  std::optional<AudioChunk> audio;
  std::optional<std::string> text;
  LanguagePair language_pair;
  std::string profile_id{"default"};
};

[[nodiscard]] std::string to_string(Language language);
[[nodiscard]] std::string to_string(SafetySeverity severity);
[[nodiscard]] Language language_from_code(std::string_view code);

}  // namespace moyi
