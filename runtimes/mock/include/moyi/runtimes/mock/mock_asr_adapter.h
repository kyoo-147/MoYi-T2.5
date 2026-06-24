#pragma once

#include <string>

#include "moyi/translation/asr_adapter.h"

namespace moyi::runtimes::mock {

class MockASRAdapter final : public IASRAdapter {
 public:
  explicit MockASRAdapter(std::string transcript = "Dung may lai, kiem tra cam bien an toan.");

  Result<void> load(const ModelManifest& manifest) override;
  Result<Transcript> transcribe(const AudioChunk& chunk, Language expected_language) override;
  ModelCapabilities capabilities() const override;

 private:
  std::string transcript_;
  bool loaded_{false};
};

}  // namespace moyi::runtimes::mock
