
add_library(dotenv 
    dotenv.cpp
)
add_library(dotenv::dotenv ALIAS dotenv)

target_compile_features(dotenv PUBLIC cxx_std_23)