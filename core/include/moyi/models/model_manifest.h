#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "moyi/types.h"

namespace moyi {

enum class ModelTask {
  ASR,
  Translation,
  Embedding,
  TTS,
};

enum class RuntimeKind {
  Mock,
  Onnx,
  Tflite,
  LlamaCpp,
  Qnn,
  ApiFallback,
};

struct ModelCapabilities {
  ModelTask task{ModelTask::Translation};
  RuntimeKind runtime{RuntimeKind::Mock};
  std::vector<LanguagePair> language_pairs;
  bool supports_streaming{false};
  bool supports_quantized{false};
  std::vector<std::string> device_targets;
};

struct ModelManifest {
  std::string id;
  ModelTask task{ModelTask::Translation};
  RuntimeKind runtime{RuntimeKind::Mock};
  LanguagePair language_pair;
  std::filesystem::path model_path;
  std::filesystem::path tokenizer_path;
  std::string quantization;
  std::vector<std::string> device_targets;
  int latency_budget_ms{1000};
  int memory_budget_mb{512};
};

}  // namespace moyi
