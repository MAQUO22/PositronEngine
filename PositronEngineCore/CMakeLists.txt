cmake_minimum_required(VERSION 3.12)

set(CORE_PROJECT_NAME PositronEngineCore)

add_library(${CORE_PROJECT_NAME} STATIC
    src/PositronEngineCore/Utilis/positron.cpp
    includes/PositroneEngineCore/Utilis/positron.hpp
    )

target_include_directories(${CORE_PROJECT_NAME} PUBLIC includes)

target_compile_features(${CORE_PROJECT_NAME} PUBLIC cxx_std_17)
