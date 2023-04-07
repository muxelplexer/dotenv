#include "dotenv.hpp"
#include <algorithm>
#include <array>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <memory>

namespace dotenv
{
    using char_buf = std::array<char, 255>;
    static std::vector<std::string> env_vars{};

    std::pair<std::string, std::string> parse_line(const char_buf& line)
    {
        auto delim_pos = std::find(
            std::cbegin(line),
            std::cend(line),
            '='
        );

        auto last_char = std::find(
            std::cbegin(line),
            std::cend(line),
            '\0'
        );
        std::string name{std::cbegin(line), delim_pos};
        std::string value{delim_pos + 1, last_char};

        if (value.starts_with('$'))
        {
            value.erase(0, 1);
            auto env_val = dotenv::get_env(value);
            if (env_val == std::nullopt) value = "";
            else value = env_val.value();
        }

        return {std::move(name), std::move(value)};

    }

    void load(std::filesystem::path env_dir, const bool overwrite)
    {
        env_dir /= ".env";
        std::ifstream ifs(env_dir.c_str());

        if (ifs.bad()) throw std::runtime_error("Could not load .env file!");

        char_buf line{0};
        while (ifs.getline(std::begin(line), line.size()))
        {
            auto env_vals = parse_line(line);

            if (env_vals.second == "") continue;
            setenv(env_vals.first.c_str(), env_vals.second.c_str(), 1);
            env_vars.emplace_back(std::move(env_vals.first));
        }
    }

    [[nodiscard]] std::vector<std::string_view> get_variables()
    {
        std::vector<std::string_view> strings(env_vars.size());
        for (auto& var : env_vars)
        {
            strings.emplace_back(var.c_str());
        }
        return strings;
    }

    extern "C" char** environ;

    [[nodiscard]] std::vector<std::string_view> get_all_variables()
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

    [[nodiscard]] std::optional<std::string_view> get_env(const std::string& name)
    {
        char* env_var = ::getenv(name.data());
        if (!env_var) return {};
        else return {std::string_view{env_var}};
    }

    [[nodiscard]] std::optional<std::string_view> get_env(const std::string_view name)
    {
        std::string str_name(name);
        char* env_var = ::getenv(str_name.data());
        if (!env_var) return {};
        else return {std::string_view{env_var}};
    }

}