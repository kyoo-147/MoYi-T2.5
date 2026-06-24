#pragma once

#include "moyi/translation/translation_adapter.h"

namespace moyi::runtimes::onnx {

class OnnxTranslationAdapter final : public ITranslationAdapter {
 public:
  Result<void> load(const ModelManifest& manifest) override;
  Result<Translation> translate(const std::string& source_text,
                                const LanguagePair& pair,
                                const RetrievedContext& context) override;
  ModelCapabilities capabilities() const override;

 private:
  ModelManifest manifest_;
  bool loaded_{false};
};

}  // namespace moyi::runtimes::onnx
