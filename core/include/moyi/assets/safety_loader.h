#pragma once

#include <filesystem>
#include <vector>

#include "moyi/result.h"
#include "moyi/safety/safety_checker.h"

namespace moyi {

Result<std::vector<SafetyRule>> load_safety_rules(const std::filesystem::path& path);

}  // namespace moyi
