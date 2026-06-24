#pragma once

#include <unordered_map>

#include "moyi/translation/translation_adapter.h"

namespace moyi::runtimes::mock {

class MockTranslationAdapter final : public ITranslationAdapter {
 public:
  Result<void> load(const ModelManifest& manifest) override;
  Result<Translation> translate(const std::string& source_text,
                                const LanguagePair& pair,
                                const RetrievedContext& context) override;
  ModelCapabilities capabilities() const override;

 private:
  bool loaded_{false};
};

}  // namespace moyi::runtimes::mock
