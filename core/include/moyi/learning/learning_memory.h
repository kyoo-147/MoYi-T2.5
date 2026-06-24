#pragma once

#include <vector>

#include "moyi/result.h"
#include "moyi/types.h"

namespace moyi {

class ILearningMemory {
 public:
  virtual ~ILearningMemory() = default;
  virtual Result<std::vector<LearningCard>> generate_cards(const Transcript& transcript,
                                                           const Translation& translation,
                                                           const RetrievedContext& context) = 0;
};

class SimpleLearningMemory final : public ILearningMemory {
 public:
  Result<std::vector<LearningCard>> generate_cards(const Transcript& transcript,
                                                   const Translation& translation,
                                                   const RetrievedContext& context) override;
};

}  // namespace moyi
