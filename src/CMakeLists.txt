
add_library(
    dotenv 
    dotenv.cpp
)
add_library(dotenv::dotenv ALIAS dotenv)
target_include_directories(dotenv
    PUBLIC
        ${CMAKE_SOURCE_DIR}/inc
)

target_compile_features(dotenv PUBLIC cxx_std_23)

set_target_properties(dotenv PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)