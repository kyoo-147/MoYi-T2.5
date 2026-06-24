#include "moyi/runtimes/mock/mock_translation_adapter.h"

#include <algorithm>
#include <cctype>

namespace moyi::runtimes::mock {
namespace {

std::string lower_copy(std::string value) {
  std::ranges::transform(value, value.begin(), [](unsigned char c) {
    return static_cast<char>(std::tolower(c));
  });
  return value;
}

bool contains(const std::string& text, const std::string& needle) {
  return lower_copy(text).find(lower_copy(needle)) != std::string::npos;
}

}  // namespace

Result<void> MockTranslationAdapter::load(const ModelManifest&) {
  loaded_ = true;
  return Result<void>::ok();
}

Result<Translation> MockTranslationAdapter::translate(const std::string& source_text,
                                                      const LanguagePair& pair,
                                                      const RetrievedContext& context) {
  if (!loaded_) {
    return Result<Translation>::error(RuntimeError{
        .code = "mock_translation_not_loaded",
        .message = "Mock translation adapter must be loaded before translate",
    });
  }

  std::string translated = "[mock translation] " + source_text;
  if (pair.source == Language::Vietnamese && pair.target == Language::English) {
    translated = "Stop the machine and check the safety sensor.";
    if (!contains(source_text, "dung") && !contains(source_text, "dừng")) {
      translated = "When does this machine need maintenance?";
    }
  }

  for (const auto& term : context.glossary_terms) {
    if (contains(source_text, term.source)) {
      translated += " [" + term.source + " = " + term.target + "]";
    }
  }

  return Result<Translation>::ok(Translation{
      .text = translated,
      .language_pair = pair,
      .confidence = 0.88,
  });
}

ModelCapabilities MockTranslationAdapter::capabilities() const {
  return ModelCapabilities{
      .task = ModelTask::Translation,
      .runtime = RuntimeKind::Mock,
      .language_pairs = {{Language::Vietnamese, Language::English}},
      .supports_streaming = false,
      .supports_quantized = false,
      .device_targets = {"desktop_cpu", "android_cpu", "edge_box_cpu"},
  };
}

}  // namespace moyi::runtimes::mock
