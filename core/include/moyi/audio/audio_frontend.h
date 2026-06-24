#pragma once

#include "moyi/result.h"
#include "moyi/types.h"

namespace moyi {

class IAudioFrontend {
 public:
  virtual ~IAudioFrontend() = default;
  virtual Result<AudioChunk> process(AudioChunk chunk) = 0;
};

class PassthroughAudioFrontend final : public IAudioFrontend {
 public:
  Result<AudioChunk> process(AudioChunk chunk) override;
};

}  // namespace moyi
