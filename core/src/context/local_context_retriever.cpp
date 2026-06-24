#include "moyi/context/context_retriever.h"

#include <algorithm>
#include <cctype>
#include <utility>

namespace moyi {
namespace {

std::string lower_copy(std::string value) {
  std::ranges::transform(value, value.begin(), [](unsigned char c) {
    return static_cast<char>(std::tolower(c));
  });
  return value;
}

bool contains_term(const std::string& haystack, const std::string& needle) {
  if (needle.empty()) {
    return false;
  }
  return lower_copy(haystack).find(lower_copy(needle)) != std::string::npos;
}

}  // namespace

void LocalContextRetriever::set_profile(std::string profile_id, std::string role) {
  profile_id_ = std::move(profile_id);
  role_ = std::move(role);
}

void LocalContextRetriever::add_glossary_term(ContextTerm term) {
  glossary_terms_.push_back(std::move(term));
}

void LocalContextRetriever::add_safety_term(std::string term) {
  safety_terms_.push_back(std::move(term));
}

Result<RetrievedContext> LocalContextRetriever::retrieve(const ContextQuery& query) {
  RetrievedContext context;
  context.profile_id = query.profile_id.empty() ? profile_id_ : query.profile_id;
  context.role = role_;

  for (const auto& term : glossary_terms_) {
    if (contains_term(query.utterance, term.source) || contains_term(query.utterance, term.target)) {
      context.glossary_terms.push_back(term);
    }
  }

  for (const auto& term : safety_terms_) {
    if (contains_term(query.utterance, term)) {
      context.safety_terms.push_back(term);
    }
  }

  return Result<RetrievedContext>::ok(context);
}

}  // namespace moyi
