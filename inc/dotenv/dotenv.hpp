#pragma once
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>


/// @brief All-including dotenv namespace
namespace dotenv
{
    /// @brief Loads the .env file from the env_dir.
    /// Opens the file at `env_dir`/.env inside an ifstream and steps through
    /// each line, parsing the declarations from it. Environment variables will
    /// be expanded. It also stores the names of the variables inside a static
    /// vector for usage in `get_variables`.
    /// @throws std::runtime_error if file could not be loaded.
    /// @param env_dir - filesystem path to the directory containing the .env file.
    /// @param overwrite Overwrite variable if it has already been defined.
    void load(std::filesystem::path env_dir, const bool overwrite = true);

    /// @brief Returns a vector of all environment variable names loaded from the .env file.
    [[nodiscard]] std::vector<std::string_view> get_variables() noexcept;

    /// @brief Returns a vector of all environment variables loaded by the program.
    [[nodiscard]] std::vector<std::string_view> get_all_variables() noexcept;

    /// @brief Returns the specified environment variable.
    /// @param name The name of the environment variable to return.
    /// @return std::string_view if successful. std::nullopt otherwise.
    [[nodiscard]] std::optional<std::string_view> get_env(const std::string& name) noexcept;

    /// @brief Returns the specifier environment variable or an alternative value.
    /// @param name The name of the environment variable to return.
    /// @param alt_val The alternative value to return.
    /// @return Value of $`name` or `alt_val`
    [[nodiscard]] std::string_view get_env_or(const std::string& name, const std::string_view alt_val) noexcept;

}  // namespace dotenv
