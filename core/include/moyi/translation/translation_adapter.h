#pragma once

#include <string>

#include "moyi/models/model_manifest.h"
#include "moyi/result.h"
#include "moyi/types.h"

namespace moyi {

class ITranslationAdapter {
 public:
  virtual ~ITranslationAdapter() = default;
  virtual Result<void> load(const ModelManifest& manifest) = 0;
  virtual Result<Translation> translate(const std::string& source_text,
                                        const LanguagePair& pair,
                                        const RetrievedContext& context) = 0;
  virtual ModelCapabilities capabilities() const = 0;
};

}  // namespace moyi
