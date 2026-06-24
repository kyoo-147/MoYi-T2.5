#include "moyi/runtimes/onnx/onnx_translation_adapter.h"

#include <filesystem>

namespace moyi::runtimes::onnx {

Result<void> OnnxTranslationAdapter::load(const ModelManifest& manifest) {
  manifest_ = manifest;
#ifdef MOYI_ENABLE_REAL_ONNX
  if (manifest_.model_path.empty() || !std::filesystem::exists(manifest_.model_path)) {
    return Result<void>::error({
        .code = "onnx_model_not_found",
        .message = "ONNX translation model not found: " + manifest_.model_path.string(),
    });
  }
  env_ = std::make_unique<Ort::Env>(ORT_LOGGING_LEVEL_WARNING, "moyi-translation");
  session_options_ = std::make_unique<Ort::SessionOptions>();
  session_options_->SetIntraOpNumThreads(1);
  session_options_->SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);
  session_ = std::make_unique<Ort::Session>(*env_, manifest_.model_path.string().c_str(), *session_options_);
#endif
  loaded_ = true;
  return Result<void>::ok();
}

Result<Translation> OnnxTranslationAdapter::translate(const std::string&,
                                                      const LanguagePair& pair,
                                                      const RetrievedContext&) {
  if (!loaded_) {
    return Result<Translation>::error(RuntimeError{
        .code = "onnx_translation_not_loaded",
        .message = "ONNX translation adapter must be loaded before translate",
    });
  }
#ifdef MOYI_ENABLE_REAL_ONNX
  if (!session_) {
    return Result<Translation>::error({
        .code = "onnx_translation_session_missing",
        .message = "ONNX Runtime translation session was not created",
    });
  }
  return Result<Translation>::error(RuntimeError{
      .code = "onnx_translation_tokenizer_not_wired",
      .message = "ONNX Runtime loaded the translation model; tokenizer and tensor I/O wiring is the next step",
  });
#else
  return Result<Translation>::error(RuntimeError{
      .code = "onnx_runtime_not_enabled",
      .message = "Reconfigure with MOYI_ENABLE_REAL_ONNX=ON and MOYI_ONNXRUNTIME_ROOT to load ONNX models",
  });
#endif
}

ModelCapabilities OnnxTranslationAdapter::capabilities() const {
  return ModelCapabilities{
      .task = ModelTask::Translation,
      .runtime = RuntimeKind::Onnx,
      .language_pairs = {manifest_.language_pair},
      .supports_streaming = false,
      .supports_quantized = true,
      .device_targets = manifest_.device_targets,
  };
}

}  // namespace moyi::runtimes::onnx
