#pragma once

#include "moyi/translation/asr_adapter.h"

#ifdef MOYI_ENABLE_REAL_ONNX
#include <memory>
#include <onnxruntime_cxx_api.h>
#endif

namespace moyi::runtimes::onnx {

class OnnxASRAdapter final : public IASRAdapter {
 public:
  Result<void> load(const ModelManifest& manifest) override;
  Result<Transcript> transcribe(const AudioChunk& chunk, Language expected_language) override;
  ModelCapabilities capabilities() const override;

 private:
  ModelManifest manifest_;
  bool loaded_{false};
#ifdef MOYI_ENABLE_REAL_ONNX
  std::unique_ptr<Ort::Env> env_;
  std::unique_ptr<Ort::SessionOptions> session_options_;
  std::unique_ptr<Ort::Session> session_;
#endif
};

}  // namespace moyi::runtimes::onnx
