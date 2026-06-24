#include "moyi/types.h"

#include <algorithm>
#include <cctype>

namespace moyi {
namespace {

std::string normalize(std::string_view code) {
  std::string out(code);
  std::ranges::transform(out, out.begin(), [](unsigned char c) {
    return static_cast<char>(std::tolower(c));
  });
  return out;
}

}  // namespace

std::string LanguagePair::code() const {
  return to_string(source) + "-" + to_string(target);
}

std::string to_string(Language language) {
  switch (language) {
    case Language::Vietnamese:
      return "vi";
    case Language::English:
      return "en";
    case Language::Korean:
      return "ko";
    case Language::Chinese:
      return "zh";
    case Language::Unknown:
      return "unknown";
  }
  return "unknown";
}

std::string to_string(SafetySeverity severity) {
  switch (severity) {
    case SafetySeverity::None:
      return "none";
    case SafetySeverity::Advisory:
      return "advisory";
    case SafetySeverity::Warning:
      return "warning";
    case SafetySeverity::Critical:
      return "critical";
  }
  return "none";
}

Language language_from_code(std::string_view code) {
  const auto normalized = normalize(code);
  if (normalized == "vi" || normalized == "vn" || normalized == "vietnamese") {
    return Language::Vietnamese;
  }
  if (normalized == "en" || normalized == "english") {
    return Language::English;
  }
  if (normalized == "ko" || normalized == "kr" || normalized == "korean") {
    return Language::Korean;
  }
  if (normalized == "zh" || normalized == "cn" || normalized == "chinese") {
    return Language::Chinese;
  }
  return Language::Unknown;
}

}  // namespace moyi
