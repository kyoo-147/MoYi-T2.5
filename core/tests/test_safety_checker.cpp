#include "test_harness.h"

#include "moyi/safety/safety_checker.h"

int main() {
  moyi::RuleBasedSafetyChecker checker;
  checker.add_rule({
      .keyword = "dung may",
      .severity = moyi::SafetySeverity::Critical,
      .requires_confirmation = true,
      .reason = "Contains stop-machine safety command",
  });

  moyi::Translation translation{
      .text = "Stop the machine.",
      .language_pair = {moyi::Language::Vietnamese, moyi::Language::English},
      .confidence = 0.9,
  };
  moyi::RetrievedContext context;
  moyi::Confidence confidence{.asr = 1.0, .translation = 0.9, .overall = 0.95};

  auto decision = checker.check("Dung may lai.", translation, context, confidence);
  MOYI_ASSERT_TRUE(decision.is_ok());
  MOYI_ASSERT_TRUE(decision.value().flagged);
  MOYI_ASSERT_TRUE(decision.value().requires_confirmation);
  MOYI_ASSERT_TRUE(decision.value().severity == moyi::SafetySeverity::Critical);

  moyi::Confidence low{.asr = 0.5, .translation = 0.5, .overall = 0.5};
  auto low_decision = checker.check("Xin chao.", translation, context, low);
  MOYI_ASSERT_TRUE(low_decision.is_ok());
  MOYI_ASSERT_TRUE(low_decision.value().flagged);
  return EXIT_SUCCESS;
}
