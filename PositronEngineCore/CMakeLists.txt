cmake_minimum_required(VERSION 3.12)

set(ENGINE_PROJECT_NAME PositronEngineCore)
set(ENGINE_PUBLIC_INCLUDES
    includes/PositroneEngineCore/Application.hpp
    includes/PositroneEngineCore/Log.hpp
)
set(ENGINE_PRIVATE_SOURSES
    src/PositronEngineCore/Application.cpp
)


add_library(${ENGINE_PROJECT_NAME} STATIC
    ${ENGINE_PUBLIC_INCLUDES}
    ${ENGINE_PRIVATE_SOURSES}
    )

target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC includes)

target_compile_features(${ENGINE_PROJECT_NAME} PUBLIC cxx_std_17)

add_subdirectory(../submodules/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glfw)

add_subdirectory(../submodules/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE spdlog)

add_subdirectory(../submodules/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glad)
