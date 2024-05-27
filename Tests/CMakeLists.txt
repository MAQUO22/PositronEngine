cmake_minimum_required(VERSION 3.12)
project(Tests)

set(GTESTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../submodules/gTests")

# Добавьте исполняемый файл для вашего теста
add_executable(Tests core_tests.cpp)

# Google Test
add_subdirectory(${GTESTS_DIR} ${CMAKE_CURRENT_BINARY_DIR}/gTests)
target_link_libraries(Tests PositronEngineCore glm glad assimp GL)
target_link_libraries(Tests gtest_main)

target_include_directories(Tests PRIVATE ../submodules/gTests/googletest/include)

target_compile_features(Tests PRIVATE cxx_std_17)
