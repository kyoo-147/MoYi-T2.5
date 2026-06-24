#include "moyi/runtimes/onnx/onnx_translation_adapter.h"

namespace moyi::runtimes::onnx {

Result<void> OnnxTranslationAdapter::load(const ModelManifest& manifest) {
  manifest_ = manifest;
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
  return Result<Translation>::error(RuntimeError{
      .code = "onnx_translation_inference_not_linked",
      .message = "ONNX translation inference boundary exists, but ONNX Runtime backend is not linked yet",
  });
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
