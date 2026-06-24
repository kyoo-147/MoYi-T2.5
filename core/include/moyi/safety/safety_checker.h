#pragma once

#include <string>
#include <vector>

#include "moyi/result.h"
#include "moyi/types.h"

namespace moyi {

struct SafetyRule {
  std::string keyword;
  SafetySeverity severity{SafetySeverity::Warning};
  bool requires_confirmation{true};
  std::string reason;
};

class ISafetyChecker {
 public:
  virtual ~ISafetyChecker() = default;
  virtual Result<SafetyDecision> check(const std::string& source_text,
                                       const Translation& translation,
                                       const RetrievedContext& context,
                                       const Confidence& confidence) = 0;
};

class RuleBasedSafetyChecker final : public ISafetyChecker {
 public:
  void add_rule(SafetyRule rule);
  void set_minimum_overall_confidence(double confidence);
  Result<SafetyDecision> check(const std::string& source_text,
                               const Translation& translation,
                               const RetrievedContext& context,
                               const Confidence& confidence) override;

 private:
  std::vector<SafetyRule> rules_;
  double minimum_overall_confidence_{0.72};
};

}  // namespace moyi
