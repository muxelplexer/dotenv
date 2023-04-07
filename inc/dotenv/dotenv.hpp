#pragma once
#include <ranges>
#include <filesystem>
#include <optional>
#include <string_view>
#include <vector>


/// @brief All-including dotenv namespace
namespace dotenv
{
    /// @brief Loads the .env file from the env_dir.
    /// Opens the file at `env_dir`/.env inside an ifstream and steps through
    /// each line, parsing the declarations from it. Environment variables will
    /// be expanded. It also stores the names of the variables inside a static
    /// vector for useage in `get_variables`.
    /// @param env_dir - filesystem path to the directory containing the .env file.
    /// @param overwrite Overwrite variable if it has already been defined.
    void load(std::filesystem::path env_dir, const bool overwrite = true);

    /// @brief Returns a vector of all environment variable names loaded from the .env file.
    [[nodiscard]] std::vector<std::string_view> get_variables();

    /// @brief Returns a vector of all environment variables loaded by the program.
    [[nodiscard]] std::vector<std::string_view> get_all_variables();

    /// @brief Returns the specified environment variable.
    /// @param name The name of the environment variable to return.
    /// @return std::string_view if successfull. std::nullopt otherwise.
    [[nodiscard]] std::optional<std::string_view> get_env(const std::string& name);
}