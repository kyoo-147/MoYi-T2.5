#include "test_harness.h"

#include <memory>

#include "moyi/context/context_retriever.h"
#include "moyi/learning/learning_memory.h"
#include "moyi/pipeline/session.h"
#include "moyi/runtimes/mock/mock_asr_adapter.h"
#include "moyi/runtimes/mock/mock_translation_adapter.h"
#include "moyi/safety/safety_checker.h"

int main() {
  auto context = std::make_shared<moyi::LocalContextRetriever>();
  context->set_profile("factory_worker", "operator");
  context->add_glossary_term({"cam bien an toan", "safety sensor", "machine_safety"});
  context->add_safety_term("dung may");

  auto safety = std::make_shared<moyi::RuleBasedSafetyChecker>();
  safety->add_rule({
      .keyword = "dung may",
      .severity = moyi::SafetySeverity::Critical,
      .requires_confirmation = true,
      .reason = "Contains stop-machine safety command",
  });

  auto asr = std::make_shared<moyi::runtimes::mock::MockASRAdapter>();
  auto translator = std::make_shared<moyi::runtimes::mock::MockTranslationAdapter>();
  moyi::ModelManifest manifest;
  manifest.id = "mock";
  manifest.language_pair = {moyi::Language::Vietnamese, moyi::Language::English};
  asr->load(manifest);
  translator->load(manifest);

  moyi::TranslationSession session(
      {.session_id = "test",
       .default_language_pair = {moyi::Language::Vietnamese, moyi::Language::English},
       .default_profile_id = "factory_worker"},
      asr,
      translator,
      context,
      safety,
      std::make_shared<moyi::SimpleLearningMemory>());

  auto result = session.process_once({
      .text = "Dung may lai, kiem tra cam bien an toan.",
      .language_pair = {moyi::Language::Vietnamese, moyi::Language::English},
      .profile_id = "factory_worker",
  });

  MOYI_ASSERT_TRUE(result.is_ok());
  MOYI_ASSERT_TRUE(result.value().safety.flagged);
  MOYI_ASSERT_TRUE(!result.value().learning_cards.empty());
  MOYI_ASSERT_EQ(result.value().language_pair.code(), std::string("vi-en"));

  auto events = session.drain_events();
  MOYI_ASSERT_TRUE(!events.empty());

  auto stream_result = session.push_audio({.samples = {0.0F, 0.1F}, .sample_rate_hz = 16000, .final_chunk = true});
  MOYI_ASSERT_TRUE(stream_result.is_ok());
  auto stream_events = session.drain_events();
  MOYI_ASSERT_TRUE(!stream_events.empty());
  return EXIT_SUCCESS;
}
