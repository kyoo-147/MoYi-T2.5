#include "moyi/runtimes/onnx/onnx_asr_adapter.h"

#include <filesystem>

namespace moyi::runtimes::onnx {

Result<void> OnnxASRAdapter::load(const ModelManifest& manifest) {
  manifest_ = manifest;
#ifdef MOYI_ENABLE_REAL_ONNX
  if (manifest_.model_path.empty() || !std::filesystem::exists(manifest_.model_path)) {
    return Result<void>::error({
        .code = "onnx_model_not_found",
        .message = "ONNX ASR model not found: " + manifest_.model_path.string(),
    });
  }
  env_ = std::make_unique<Ort::Env>(ORT_LOGGING_LEVEL_WARNING, "moyi-asr");
  session_options_ = std::make_unique<Ort::SessionOptions>();
  session_options_->SetIntraOpNumThreads(1);
  session_options_->SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);
  session_ = std::make_unique<Ort::Session>(*env_, manifest_.model_path.string().c_str(), *session_options_);
#endif
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
#ifdef MOYI_ENABLE_REAL_ONNX
  if (!session_) {
    return Result<Transcript>::error({
        .code = "onnx_asr_session_missing",
        .message = "ONNX Runtime ASR session was not created",
    });
  }
  return Result<Transcript>::error(RuntimeError{
      .code = "onnx_asr_feature_extraction_not_wired",
      .message = "ONNX Runtime loaded the ASR model; feature extraction and tensor I/O wiring is the next step",
  });
#else
  return Result<Transcript>::error(RuntimeError{
      .code = "onnx_runtime_not_enabled",
      .message = "Reconfigure with MOYI_ENABLE_REAL_ONNX=ON and MOYI_ONNXRUNTIME_ROOT to load ONNX models",
  });
#endif
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
