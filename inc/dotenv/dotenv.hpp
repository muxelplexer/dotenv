#pragma once
#include <ranges>
#include <filesystem>
#include <optional>
#include <string_view>
#include <vector>


namespace dotenv
{
    void load(std::filesystem::path env_dir, const bool overwrite = true);

    [[nodiscard]] std::vector<std::string_view> get_variables();
    [[nodiscard]] std::vector<std::string_view> get_all_variables();
    [[nodiscard]] std::optional<std::string_view> get_env(const std::string& name);
}