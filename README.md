# dotenv - A simple c++ dotenv & environment variable handler 

Small and simple library to load a .env file from some directory.

## Requirements
- C++20 supporting Compiler (gcc 12.2.0+)
- CMake 3.24+
- Doxygen (if building the documentation is desired)

## Example Usage
```cpp
#include <iostream>
#include "dotenv/dotenv.hpp"

int main()
{
    // dotenv::load takes the path to the directory
    // where the .env file is located
    dotenv::load("../");

    // Gets all Variables successfully parsed from the .env file
    const auto dot_env_vars = dotenv::get_variables();
    for (const auto env_var : dot_env_vars)
    {
        // dotenv::getenv returns a std::optional object.
        // Since we know that these variables got set it's safe to call .value() on it
        std::cout << env_var << "=" << dotenv::get_env(std::string(env_var)).value() << "\n";
    }

    // dotenv::get_env can also be used to retrieve general environment variables.
    const auto home_var = dotenv::get_env("HOME");
    if (home_var != std::nullopt)
    {
        std::cout << home_var.value() << "\n";
    }

    // dotenv::get_cenv can be used to retrieve environment variables from the cache specifically.
    const auto dotenv_var = dotenv::get_cenv("HOME");
    if (dotenv_var != std::nullopt)
    {
        std::cout << dotenv_var.value() << "\n";
    }

    return 0;
}
```
