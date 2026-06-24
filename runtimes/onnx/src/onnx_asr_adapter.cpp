#include "moyi/runtimes/onnx/onnx_asr_adapter.h"

namespace moyi::runtimes::onnx {

Result<void> OnnxASRAdapter::load(const ModelManifest& manifest) {
  manifest_ = manifest;
  loaded_ = true;
  return Result<void>::ok();
}

Result<Transcript> OnnxASRAdapter::transcribe(const AudioChunk&, Language expected_language) {
  if (!loaded_) {
    return Result<Transcript>::error(RuntimeError{
        .code = "onnx_asr_not_loaded",
        .message = "ONNX ASR adapter must be loaded before transcribe",
    });
  }
  return Result<Transcript>::error(RuntimeError{
      .code = "onnx_asr_inference_not_linked",
      .message = "ONNX ASR inference boundary exists, but ONNX Runtime backend is not linked yet",
  });
}

ModelCapabilities OnnxASRAdapter::capabilities() const {
  return ModelCapabilities{
      .task = ModelTask::ASR,
      .runtime = RuntimeKind::Onnx,
      .language_pairs = {manifest_.language_pair},
      .supports_streaming = true,
      .supports_quantized = true,
      .device_targets = manifest_.device_targets,
  };
}

}  // namespace moyi::runtimes::onnx
