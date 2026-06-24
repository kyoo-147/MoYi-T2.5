#include "moyi/audio/audio_frontend.h"

#include <utility>

namespace moyi {

Result<AudioChunk> PassthroughAudioFrontend::process(AudioChunk chunk) {
  return Result<AudioChunk>::ok(std::move(chunk));
}

}  // namespace moyi
