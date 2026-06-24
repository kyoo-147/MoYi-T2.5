#include "test_harness.h"

#include "moyi/context/context_retriever.h"

int main() {
  moyi::LocalContextRetriever retriever;
  retriever.set_profile("factory_worker", "operator");
  retriever.add_glossary_term({"cam bien an toan", "safety sensor", "machine_safety"});
  retriever.add_safety_term("dung may");

  auto result = retriever.retrieve({
      .profile_id = "factory_worker",
      .utterance = "Dung may lai, kiem tra cam bien an toan.",
      .language_pair = {moyi::Language::Vietnamese, moyi::Language::English},
  });

  MOYI_ASSERT_TRUE(result.is_ok());
  MOYI_ASSERT_EQ(result.value().profile_id, std::string("factory_worker"));
  MOYI_ASSERT_EQ(result.value().glossary_terms.size(), static_cast<std::size_t>(1));
  MOYI_ASSERT_EQ(result.value().safety_terms.size(), static_cast<std::size_t>(1));
  return EXIT_SUCCESS;
}
