#pragma once

#include <filesystem>
#include <vector>

#include "moyi/result.h"
#include "moyi/types.h"

namespace moyi {

Result<std::vector<ContextTerm>> load_glossary_terms(const std::filesystem::path& path);

}  // namespace moyi
