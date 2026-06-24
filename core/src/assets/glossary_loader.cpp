#include "moyi/assets/glossary_loader.h"

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

}  // namespace

Result<std::vector<ContextTerm>> load_glossary_terms(const std::filesystem::path& path) {
  auto text = read_text_file(path);
  if (!text.is_ok()) {
    return Result<std::vector<ContextTerm>>::error(text.error());
  }

  std::vector<ContextTerm> terms;
  const std::regex object_pattern(R"(\{[^{}]*"source"\s*:\s*"([^"]+)"[^{}]*"target"\s*:\s*"([^"]+)"[^{}]*"domain"\s*:\s*"([^"]+)"[^{}]*\})");
  auto begin = std::sregex_iterator(text.value().begin(), text.value().end(), object_pattern);
  auto end = std::sregex_iterator();
  for (auto it = begin; it != end; ++it) {
    terms.push_back({.source = (*it)[1].str(), .target = (*it)[2].str(), .domain = (*it)[3].str()});
  }

  if (terms.empty()) {
    return Result<std::vector<ContextTerm>>::error({
        .code = "glossary_empty_or_invalid",
        .message = "No glossary terms found in " + path.string(),
    });
  }
  return Result<std::vector<ContextTerm>>::ok(terms);
}

}  // namespace moyi
