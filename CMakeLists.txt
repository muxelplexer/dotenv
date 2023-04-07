cmake_minimum_required(VERSION 3.24)
project(dotenv
        VERSION 1.0.0
)

add_subdirectory(src)
target_include_directories(dotenv
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)