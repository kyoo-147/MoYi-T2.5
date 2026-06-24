#pragma once

#include <filesystem>

#include "moyi/models/model_manifest.h"
#include "moyi/result.h"

namespace moyi {

Result<ModelManifest> load_model_manifest(const std::filesystem::path& path);

}  // namespace moyi
