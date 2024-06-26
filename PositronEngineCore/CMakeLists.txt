cmake_minimum_required(VERSION 3.12)

set(ENGINE_PROJECT_NAME PositronEngineCore)

set(ENGINE_PUBLIC_INCLUDES
    includes/PositronEngineCore/Application.hpp
    includes/PositronEngineCore/Log.hpp
    includes/PositronEngineCore/Event.hpp
    includes/PositronEngineCore/Camera.hpp
    includes/PositronEngineCore/KeyCode.hpp
    includes/PositronEngineCore/Input.hpp
    includes/PositronEngineCore/VertexArray.hpp
    includes/PositronEngineCore/VertexBuffer.hpp
    includes/PositronEngineCore/FrameBuffer.hpp
    includes/PositronEngineCore/RenderBuffer.hpp
    includes/PositronEngineCore/Texture2D.hpp
    includes/PositronEngineCore/CubeMapTexture.hpp
    includes/PositronEngineCore/ShaderProgram.hpp
    includes/PositronEngineCore/IndexBuffer.hpp
    includes/PositronEngineCore/RenderOpenGL.hpp
    includes/PositronEngineCore/Sphere.h
    includes/PositronEngineCore/stb_image.h
    includes/PositronEngineCore/Scene.hpp

    includes/PositronEngineCore/GameObject.hpp
    includes/PositronEngineCore/LightObject.hpp

    includes/PositronEngineCore/Mesh.hpp
    includes/PositronEngineCore/Material.hpp
    includes/PositronEngineCore/LightMaterial.hpp

    includes/PositronEngineCore/CubeMap.hpp
    includes/PositronEngineCore/Primitives/Model.hpp
    includes/PositronEngineCore/Primitives/CubePrimitive.hpp
    includes/PositronEngineCore/Primitives/PlatePrimitive.hpp
    includes/PositronEngineCore/Primitives/SpherePrimitive.hpp

    includes/PositronEngineCore/LightTypeCounter.hpp
    includes/PositronEngineCore/LightSources/DirectionLight.hpp
    includes/PositronEngineCore/LightSources/PointLight.hpp
    includes/PositronEngineCore/LightSources/SpotLight.hpp
)

set(ENGINE_PRIVATE_INCLUDES
    src/PositronEngineCore/Window.hpp
    src/PositronEngineCore/Modules/GUImodule.hpp
)

set(ENGINE_PRIVATE_SOURSES
    src/PositronEngineCore/Application.cpp
    src/PositronEngineCore/Window.cpp
    src/PositronEngineCore/Camera.cpp
    src/PositronEngineCore/Input.cpp
    src/PositronEngineCore/stb.cpp
    src/PositronEngineCore/Rendering/OpenGL/Texture2D.cpp
    src/PositronEngineCore/Rendering/OpenGL/CubeMapTexture.cpp
    src/PositronEngineCore/Rendering/OpenGL/ShaderProgram.cpp
    src/PositronEngineCore/Rendering/OpenGL/Sphere.cpp
    src/PositronEngineCore/Rendering/OpenGL/VertexBuffer.cpp
    src/PositronEngineCore/Rendering/OpenGL/VertexArray.cpp
    src/PositronEngineCore/Rendering/OpenGL/FrameBuffer.cpp
    src/PositronEngineCore/Rendering/OpenGL/RenderBuffer.cpp
    src/PositronEngineCore/Rendering/OpenGL/IndexBuffer.cpp
    src/PositronEngineCore/Rendering/OpenGL/RenderOpenGL.cpp
    src/PositronEngineCore/Modules/GUImodule.cpp
    src/PositronEngineCore/Scene.cpp

    src/PositronEngineCore/Mesh.cpp
    src/PositronEngineCore/Material.cpp
    src/PositronEngineCore/LightMaterial.cpp

    src/PositronEngineCore/GameObject.cpp
    src/PositronEngineCore/LightObject.cpp

    src/PositronEngineCore/CubeMap.cpp
    src/PositronEngineCore/Primitives/Model.cpp
    src/PositronEngineCore/Primitives/CubePrimitive.cpp
    src/PositronEngineCore/Primitives/PlatePrimitive.cpp
    src/PositronEngineCore/Primitives/SpherePrimitive.cpp

    src/PositronEngineCore/LightTypeCounter.cpp
    src/PositronEngineCore/LightSources/DirectionLight.cpp
    src/PositronEngineCore/LightSources/PointLight.cpp
    src/PositronEngineCore/LightSources/SpotLight.cpp
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


if(WIN32)
    target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE opengl32)
elseif(UNIX)
    target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE GL)
else()
    target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE GL)
endif()

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

add_subdirectory(../submodules/assimp ${CMAKE_CURRENT_BINARY_DIR}/assimp)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE assimp)

add_subdirectory(../submodules/ImGuiFileDialog ${CMAKE_CURRENT_BINARY_DIR}/ImGuiFileDialog)

target_include_directories(ImGuiFileDialog PRIVATE ../submodules/imgui)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE ImGuiFileDialog)



