#pragma once

#include <string>
#include <vector>

#include "moyi/result.h"
#include "moyi/types.h"

namespace moyi {

struct ContextQuery {
  std::string profile_id;
  std::string utterance;
  LanguagePair language_pair;
};

class IContextRetriever {
 public:
  virtual ~IContextRetriever() = default;
  virtual Result<RetrievedContext> retrieve(const ContextQuery& query) = 0;
};

class LocalContextRetriever final : public IContextRetriever {
 public:
  void set_profile(std::string profile_id, std::string role);
  void add_glossary_term(ContextTerm term);
  void add_safety_term(std::string term);
  Result<RetrievedContext> retrieve(const ContextQuery& query) override;

 private:
  std::string profile_id_{"default"};
  std::string role_{"factory_worker"};
  std::vector<ContextTerm> glossary_terms_;
  std::vector<std::string> safety_terms_;
};

}  // namespace moyi
