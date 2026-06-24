#include "moyi/assets/model_manifest_loader.h"

#include <exception>
#include <fstream>
#include <sstream>
#include <string>

namespace moyi {
namespace {

Result<std::string> read_text_file(const std::filesystem::path& path) {
  std::ifstream input(path);
  if (!input) {
    return Result<std::string>::error({.code = "asset_not_found", .message = "Unable to open " + path.string()});
  }
  std::ostringstream buffer;
  buffer << input.rdbuf();
  return Result<std::string>::ok(buffer.str());
}

std::string scalar_value(const std::string& text, const std::string& key) {
  const auto needle = key + ":";
  const auto pos = text.find(needle);
  if (pos == std::string::npos) {
    return "";
  }
  auto start = pos + needle.size();
  while (start < text.size() && (text[start] == ' ' || text[start] == '\t')) {
    ++start;
  }
  auto end = text.find_first_of("\r\n", start);
  auto value = text.substr(start, end == std::string::npos ? std::string::npos : end - start);
  if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
    value = value.substr(1, value.size() - 2);
  }
  return value;
}

RuntimeKind runtime_from_string(const std::string& value) {
  if (value == "onnx") {
    return RuntimeKind::Onnx;
  }
  if (value == "tflite") {
    return RuntimeKind::Tflite;
  }
  if (value == "llamacpp") {
    return RuntimeKind::LlamaCpp;
  }
  if (value == "qnn") {
    return RuntimeKind::Qnn;
  }
  if (value == "api-fallback") {
    return RuntimeKind::ApiFallback;
  }
  return RuntimeKind::Mock;
}

ModelTask task_from_string(const std::string& value) {
  if (value == "asr") {
    return ModelTask::ASR;
  }
  if (value == "embedding") {
    return ModelTask::Embedding;
  }
  if (value == "tts") {
    return ModelTask::TTS;
  }
  return ModelTask::Translation;
}

LanguagePair pair_from_string(const std::string& value) {
  const auto dash = value.find('-');
  if (dash == std::string::npos) {
    return {Language::Unknown, Language::Unknown};
  }
  return {language_from_code(value.substr(0, dash)), language_from_code(value.substr(dash + 1))};
}

}  // namespace

Result<ModelManifest> load_model_manifest(const std::filesystem::path& path) {
  auto text = read_text_file(path);
  if (!text.is_ok()) {
    return Result<ModelManifest>::error(text.error());
  }

  ModelManifest manifest;
  manifest.id = scalar_value(text.value(), "id");
  manifest.task = task_from_string(scalar_value(text.value(), "task"));
  manifest.runtime = runtime_from_string(scalar_value(text.value(), "runtime"));
  manifest.language_pair = pair_from_string(scalar_value(text.value(), "language_pair"));
  manifest.model_path = scalar_value(text.value(), "model_path");
  manifest.tokenizer_path = scalar_value(text.value(), "tokenizer_path");
  manifest.quantization = scalar_value(text.value(), "quantization");

  try {
    const auto latency = scalar_value(text.value(), "latency_budget_ms");
    const auto memory = scalar_value(text.value(), "memory_budget_mb");
    if (!latency.empty()) {
      manifest.latency_budget_ms = std::stoi(latency);
    }
    if (!memory.empty()) {
      manifest.memory_budget_mb = std::stoi(memory);
    }
  } catch (const std::exception& error) {
    return Result<ModelManifest>::error({
        .code = "manifest_invalid_number",
        .message = "Invalid numeric field in " + path.string() + ": " + error.what(),
    });
  }

  if (manifest.id.empty()) {
    return Result<ModelManifest>::error({
        .code = "manifest_invalid",
        .message = "Model manifest missing id: " + path.string(),
    });
  }
  return Result<ModelManifest>::ok(manifest);
}

}  // namespace moyi
