#include "moyi/safety/safety_checker.h"

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

bool contains_keyword(const std::string& text, const std::string& keyword) {
  if (keyword.empty()) {
    return false;
  }
  return lower_copy(text).find(lower_copy(keyword)) != std::string::npos;
}

bool more_severe(SafetySeverity lhs, SafetySeverity rhs) {
  return static_cast<int>(lhs) > static_cast<int>(rhs);
}

}  // namespace

void RuleBasedSafetyChecker::add_rule(SafetyRule rule) {
  rules_.push_back(std::move(rule));
}

void RuleBasedSafetyChecker::set_minimum_overall_confidence(double confidence) {
  minimum_overall_confidence_ = confidence;
}

Result<SafetyDecision> RuleBasedSafetyChecker::check(const std::string& source_text,
                                                     const Translation& translation,
                                                     const RetrievedContext& context,
                                                     const Confidence& confidence) {
  SafetyDecision decision;

  auto apply_match = [&](const SafetyRule& rule) {
    decision.flagged = true;
    decision.requires_confirmation = decision.requires_confirmation || rule.requires_confirmation;
    if (more_severe(rule.severity, decision.severity)) {
      decision.severity = rule.severity;
      decision.reason = rule.reason.empty() ? "Matched safety keyword: " + rule.keyword : rule.reason;
    }
  };

  for (const auto& rule : rules_) {
    if (contains_keyword(source_text, rule.keyword) || contains_keyword(translation.text, rule.keyword)) {
      apply_match(rule);
    }
  }

  for (const auto& term : context.safety_terms) {
    SafetyRule rule;
    rule.keyword = term;
    rule.severity = SafetySeverity::Critical;
    rule.requires_confirmation = true;
    rule.reason = "Matched safety context term: " + term;
    if (contains_keyword(source_text, term) || contains_keyword(translation.text, term)) {
      apply_match(rule);
    }
  }

  if (confidence.overall < minimum_overall_confidence_) {
    decision.flagged = true;
    decision.requires_confirmation = true;
    if (more_severe(SafetySeverity::Warning, decision.severity)) {
      decision.severity = SafetySeverity::Warning;
      decision.reason = "Overall confidence below safety threshold";
    }
  }

  return Result<SafetyDecision>::ok(decision);
}

}  // namespace moyi
