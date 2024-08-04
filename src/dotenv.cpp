#include "dotenv/dotenv.hpp"
#include <algorithm>
#include <array>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <optional>
#include <regex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace dotenv
{
    using char_buf = std::array<char, 255>;
    namespace {
        std::vector<std::string> env_vars{};
    }

    std::pair<std::string, std::string> parse_line(const char_buf& line)
    {
        auto delim_pos = std::find(
            std::cbegin(line),
            std::cend(line),
            '='
        );
        if (delim_pos == std::cend(line) || *(delim_pos + 1) != '\"') throw std::runtime_error("Missing start-quotation marks.");

        auto last_char = std::find(
            std::cbegin(line),
            std::cend(line),
            '\0'
        );
        if (last_char == std::cend(line)) last_char = std::cend(line) - 1;
        if (*(last_char - 1) != '\"') throw std::runtime_error("Missing end-quotation marks.");

        std::string name{std::cbegin(line), delim_pos};
        std::string value{delim_pos + 2, last_char - 1};

        for (auto it = std::find(std::begin(value), std::end(value), '$'); 
                  it != std::cend(value);
                  it = std::find(it + 1, std::end(value), '$'))
        {
            auto end_pos = std::find_if_not(
                it + 1,
                std::end(value),
                ::isalnum
            );

            std::string val{it, end_pos};
            auto env_val = dotenv::get_env(val.substr(1));
            if (env_val == std::nullopt) 
            {
                for (auto pos = 0; (pos = value.find(val, pos) + 1); )
                {
                    value.replace(--pos, val.size(), "");
                }
            }
            else
            {
                for (auto pos = 0; (pos = value.find(val, pos) + 1); pos += env_val.value().size())
                {
                    value.replace(--pos, val.size(), env_val.value());
                }
            }
        }

        return {std::move(name), std::move(value)};

    }

    void load(std::filesystem::path env_dir, const bool overwrite)
    {
        env_dir /= ".env";
        std::ifstream ifs(env_dir.c_str());

        if (ifs.bad()) throw std::runtime_error("Could not load .env file!");

        char_buf line{0};
        int line_n = 1;
        while (ifs.getline(std::begin(line), line.size()))
        {
            try
            {
                auto env_vals = parse_line(line);
                if (env_vals.second == "") continue;
                setenv(env_vals.first.c_str(), env_vals.second.c_str(), 1);
                auto it = std::find(
                    std::begin(env_vars),
                    std::end(env_vars),
                    env_vals.first
                );
                if (it != std::end(env_vars))
                {
                    *it = std::move(env_vals.first);
                } else 
                {
                    env_vars.emplace_back(std::move(env_vals.first));
                }
            }
            catch(const std::runtime_error& e)
            {
                throw;
            }
        }
    }

    [[nodiscard]] std::vector<std::string_view> get_variables() noexcept
    {
        std::vector<std::string_view> strings{};
        strings.reserve(env_vars.size());

        for (auto& var : env_vars)
        {
            strings.emplace_back(var.c_str());
        }

        return strings;
    }

    extern "C" char** environ;

    [[nodiscard]] std::vector<std::string_view> get_all_variables() noexcept
    {
        std::vector<std::string_view> strings{};
        auto env_ptr = environ;
        while (*env_ptr != nullptr)
        {
            std::string_view env_str(*env_ptr);
            strings.emplace_back(
                std::cbegin(env_str),
                std::find(
                    std::cbegin(env_str),
                    std::cend(env_str),
                    '='
                )
            );
            env_ptr++;
        }
        return strings;
    }

    [[nodiscard]] std::optional<std::string_view> get_env(const std::string& name) noexcept
    {
        char* env_var = ::getenv(name.data());
        if (!env_var) {
            return {};
        }
        else {
            return {std::string_view{env_var}};
        }
    }

    std::string_view get_env_or(const std::string& name, const std::string_view alt_val) noexcept
    {
        char* env_var = ::getenv(name.data());
        if (!env_var) {
            return alt_val;
        }
        else {
            return {std::string_view{env_var}};
        }
    }
}  // namespace dotenv
