cmake_minimum_required(VERSION 3.12)

set(ENGINE_PROJECT_NAME PositronEngineCore)

set(ENGINE_PUBLIC_INCLUDES
    includes/PositronEngineCore/Application.hpp
    includes/PositronEngineCore/Log.hpp
    includes/PositronEngineCore/Event.hpp
)

set(ENGINE_PRIVATE_INCLUDES
    src/PositronEngineCore/Window.hpp
)


set(ENGINE_PRIVATE_SOURSES
    src/PositronEngineCore/Application.cpp
    src/PositronEngineCore/Window.cpp
)


add_library(${ENGINE_PROJECT_NAME} STATIC
    ${ENGINE_PUBLIC_INCLUDES}
    ${ENGINE_PRIVATE_INCLUDES}
    ${ENGINE_PRIVATE_SOURSES}
)

set(IMGUI_INCLUDES
    ../submodules/imgui/imgui.h
    ../submodules/imgui/backends/imgui_impl_glfw.h
    ../submodules/imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_SOURCES
    ../submodules/imgui/imgui.cpp
    ../submodules/imgui/backends/imgui_impl_glfw.cpp
    ../submodules/imgui/backends/imgui_impl_opengl3.cpp
    ../submodules/imgui/imgui_demo.cpp
    ../submodules/imgui/imgui_draw.cpp
    ../submodules/imgui/imgui_tables.cpp
    ../submodules/imgui/imgui_widgets.cpp
)

add_library(ImGUI STATIC
    ${IMGUI_INCLUDES}
    ${IMGUI_SOURCES}
)

target_include_directories(ImGUI PUBLIC ../submodules)
target_include_directories(ImGUI PUBLIC ../submodules/imgui)
target_link_libraries(ImGUI PRIVATE glad glfw)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE ImGUI)

target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC includes)
target_include_directories(${ENGINE_PROJECT_NAME} PRIVATE src)

target_compile_features(${ENGINE_PROJECT_NAME} PUBLIC cxx_std_17)

add_subdirectory(../submodules/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glfw)

add_subdirectory(../submodules/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE spdlog)

add_subdirectory(../submodules/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glad)


