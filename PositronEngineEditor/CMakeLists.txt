cmake_minimum_required(VERSION 3.12)

set(EDITOR_PROJECT_NAME PositronEngineEditor)

# Добавляем опцию компиляции -fsanitize=address
if(UNIX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()


add_executable(${EDITOR_PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${EDITOR_PROJECT_NAME} PositronEngineCore ImGUI glm glad assimp)

target_compile_features(${EDITOR_PROJECT_NAME} PUBLIC cxx_std_17)

set_target_properties(${EDITOR_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
