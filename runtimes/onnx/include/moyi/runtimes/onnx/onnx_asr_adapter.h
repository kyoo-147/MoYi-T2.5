#pragma once

#include "moyi/translation/asr_adapter.h"

namespace moyi::runtimes::onnx {

class OnnxASRAdapter final : public IASRAdapter {
 public:
  Result<void> load(const ModelManifest& manifest) override;
  Result<Transcript> transcribe(const AudioChunk& chunk, Language expected_language) override;
  ModelCapabilities capabilities() const override;

 private:
  ModelManifest manifest_;
  bool loaded_{false};
};

}  // namespace moyi::runtimes::onnx
