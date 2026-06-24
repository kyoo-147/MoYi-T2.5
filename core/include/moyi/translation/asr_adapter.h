#pragma once

#include "moyi/models/model_manifest.h"
#include "moyi/result.h"
#include "moyi/types.h"

namespace moyi {

class IASRAdapter {
 public:
  virtual ~IASRAdapter() = default;
  virtual Result<void> load(const ModelManifest& manifest) = 0;
  virtual Result<Transcript> transcribe(const AudioChunk& chunk, Language expected_language) = 0;
  virtual ModelCapabilities capabilities() const = 0;
};

}  // namespace moyi
