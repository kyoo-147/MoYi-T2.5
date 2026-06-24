#include "moyi/runtimes/mock/mock_asr_adapter.h"

#include <utility>

namespace moyi::runtimes::mock {

MockASRAdapter::MockASRAdapter(std::string transcript) : transcript_(std::move(transcript)) {}

Result<void> MockASRAdapter::load(const ModelManifest&) {
  loaded_ = true;
  return Result<void>::ok();
}

Result<Transcript> MockASRAdapter::transcribe(const AudioChunk&, Language expected_language) {
  if (!loaded_) {
    return Result<Transcript>::error(RuntimeError{
        .code = "mock_asr_not_loaded",
        .message = "Mock ASR adapter must be loaded before transcribe",
    });
  }
  return Result<Transcript>::ok(Transcript{
      .text = transcript_,
      .language = expected_language,
      .confidence = 0.94,
      .is_final = true,
  });
}

ModelCapabilities MockASRAdapter::capabilities() const {
  return ModelCapabilities{
      .task = ModelTask::ASR,
      .runtime = RuntimeKind::Mock,
      .language_pairs = {{Language::Vietnamese, Language::English}},
      .supports_streaming = true,
      .supports_quantized = false,
      .device_targets = {"desktop_cpu", "android_cpu", "edge_box_cpu"},
  };
}

}  // namespace moyi::runtimes::mock
