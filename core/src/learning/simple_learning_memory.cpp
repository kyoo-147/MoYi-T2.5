#include "moyi/learning/learning_memory.h"

namespace moyi {

Result<std::vector<LearningCard>> SimpleLearningMemory::generate_cards(
    const Transcript& transcript,
    const Translation& translation,
    const RetrievedContext& context) {
  std::vector<LearningCard> cards;

  for (const auto& term : context.glossary_terms) {
    cards.push_back(LearningCard{
        .front = term.source,
        .back = term.target,
        .type = "term",
        .source_utterance = transcript.text,
    });
  }

  if (cards.empty() && !transcript.text.empty() && !translation.text.empty()) {
    cards.push_back(LearningCard{
        .front = transcript.text,
        .back = translation.text,
        .type = "phrase",
        .source_utterance = transcript.text,
    });
  }

  return Result<std::vector<LearningCard>>::ok(cards);
}

}  // namespace moyi
