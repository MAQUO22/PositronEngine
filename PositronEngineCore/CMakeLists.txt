cmake_minimum_required(VERSION 3.12)

set(ENGINE_PROJECT_NAME PositronEngineCore)

set(ENGINE_PUBLIC_INCLUDES
    includes/PositronEngineCore/Application.hpp
    includes/PositronEngineCore/Log.hpp
    includes/PositronEngineCore/Event.hpp
    includes/PositronEngineCore/Camera.hpp
    includes/PositronEngineCore/KeyCode.hpp
    includes/PositronEngineCore/Input.hpp
)

set(ENGINE_PRIVATE_INCLUDES
    src/PositronEngineCore/Window.hpp
    src/PositronEngineCore/Rendering/OpenGL/Sphere.h
    src/PositronEngineCore/Rendering/OpenGL/ShaderProgram.hpp
    src/PositronEngineCore/Rendering/OpenGL/VertexBuffer.hpp
    src/PositronEngineCore/Rendering/OpenGL/VertexArray.hpp
    src/PositronEngineCore/Rendering/OpenGL/IndexBuffer.hpp
    src/PositronEngineCore/Rendering/OpenGL/RenderOpenGL.hpp

    src/PositronEngineCore/Modules/GUImodule.hpp
)

set(ENGINE_PRIVATE_SOURSES
    src/PositronEngineCore/Application.cpp
    src/PositronEngineCore/Window.cpp
    src/PositronEngineCore/Camera.cpp
    src/PositronEngineCore/Input.cpp
    src/PositronEngineCore/Rendering/OpenGL/Sphere.cpp
    src/PositronEngineCore/Rendering/OpenGL/ShaderProgram.cpp
    src/PositronEngineCore/Rendering/OpenGL/VertexBuffer.cpp
    src/PositronEngineCore/Rendering/OpenGL/VertexArray.cpp
    src/PositronEngineCore/Rendering/OpenGL/IndexBuffer.cpp
    src/PositronEngineCore/Rendering/OpenGL/RenderOpenGL.cpp

    src/PositronEngineCore/Modules/GUImodule.cpp
)

set(ENGINE_ALL_SOURCES
    ${ENGINE_PUBLIC_INCLUDES}
    ${ENGINE_PRIVATE_INCLUDES}
    ${ENGINE_PRIVATE_SOURSES}
)

add_library(${ENGINE_PROJECT_NAME} STATIC
    ${ENGINE_ALL_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
    ${ENGINE_ALL_SOURCES}
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
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE GL)

target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC includes)
target_include_directories(${ENGINE_PROJECT_NAME} PRIVATE src)

target_compile_features(${ENGINE_PROJECT_NAME} PUBLIC cxx_std_17)

add_subdirectory(../submodules/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glfw)

add_subdirectory(../submodules/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE spdlog)

add_subdirectory(../submodules/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glad)

add_subdirectory(../submodules/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glm)


