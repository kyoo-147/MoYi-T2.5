#include "moyi/assets/safety_loader.h"

#include <fstream>
#include <regex>
#include <sstream>

namespace moyi {
namespace {

Result<std::string> read_text_file(const std::filesystem::path& path) {
  std::ifstream input(path);
  if (!input) {
    return Result<std::string>::error({.code = "asset_not_found", .message = "Unable to open " + path.string()});
  }
  std::ostringstream buffer;
  buffer << input.rdbuf();
  return Result<std::string>::ok(buffer.str());
}

SafetySeverity severity_from_string(const std::string& value) {
  if (value == "critical") {
    return SafetySeverity::Critical;
  }
  if (value == "warning") {
    return SafetySeverity::Warning;
  }
  if (value == "advisory") {
    return SafetySeverity::Advisory;
  }
  return SafetySeverity::None;
}

}  // namespace

Result<std::vector<SafetyRule>> load_safety_rules(const std::filesystem::path& path) {
  auto text = read_text_file(path);
  if (!text.is_ok()) {
    return Result<std::vector<SafetyRule>>::error(text.error());
  }

  std::vector<SafetyRule> rules;
  const std::regex object_pattern(
      R"(\{[^{}]*"keyword"\s*:\s*"([^"]+)"[^{}]*"severity"\s*:\s*"([^"]+)"[^{}]*"requires_confirmation"\s*:\s*(true|false)[^{}]*"reason"\s*:\s*"([^"]+)"[^{}]*\})");
  auto begin = std::sregex_iterator(text.value().begin(), text.value().end(), object_pattern);
  auto end = std::sregex_iterator();
  for (auto it = begin; it != end; ++it) {
    rules.push_back({
        .keyword = (*it)[1].str(),
        .severity = severity_from_string((*it)[2].str()),
        .requires_confirmation = (*it)[3].str() == "true",
        .reason = (*it)[4].str(),
    });
  }

  if (rules.empty()) {
    return Result<std::vector<SafetyRule>>::error({
        .code = "safety_rules_empty_or_invalid",
        .message = "No safety rules found in " + path.string(),
    });
  }
  return Result<std::vector<SafetyRule>>::ok(rules);
}

}  // namespace moyi
