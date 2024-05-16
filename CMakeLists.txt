cmake_minimum_required(VERSION 3.12)
project(PositronEngine)

set(PROJECT_NAME PositronEngine)
set(CMAKE_PROJECT_INCLUDE_BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/EnableCMP0048.cmake")

add_subdirectory(subproj/)
add_subdirectory(PositronEngineCore)
add_subdirectory(PositronEngineEditor)
add_subdirectory(Tests)

unset(CMAKE_PROJECT_INCLUDE_BEFORE)
