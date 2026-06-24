#include "test_harness.h"

#include "moyi/learning/learning_memory.h"

int main() {
  moyi::SimpleLearningMemory memory;
  moyi::Transcript transcript{
      .text = "Dung may lai, kiem tra cam bien an toan.",
      .language = moyi::Language::Vietnamese,
      .confidence = 1.0,
      .is_final = true,
  };
  moyi::Translation translation{
      .text = "Stop the machine and check the safety sensor.",
      .language_pair = {moyi::Language::Vietnamese, moyi::Language::English},
      .confidence = 0.9,
  };
  moyi::RetrievedContext context;
  context.glossary_terms.push_back({"cam bien an toan", "safety sensor", "machine_safety"});

  auto cards = memory.generate_cards(transcript, translation, context);
  MOYI_ASSERT_TRUE(cards.is_ok());
  MOYI_ASSERT_EQ(cards.value().size(), static_cast<std::size_t>(1));
  MOYI_ASSERT_EQ(cards.value().front().front, std::string("cam bien an toan"));
  return EXIT_SUCCESS;
}
