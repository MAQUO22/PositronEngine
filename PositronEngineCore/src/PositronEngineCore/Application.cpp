#include "PositronEngineCore/Application.hpp"
#include "PositronEngineCore/Log.hpp"
#include "PositronEngineCore/Window.hpp"
#include "PositronEngineCore/Event.hpp"
#include "PositronEngineCore/Camera.hpp"
#include "PositronEngineCore/Input.hpp"

#include "PositronEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "PositronEngineCore/Rendering/OpenGL/Bmp.h"
#include "PositronEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "PositronEngineCore/Rendering/OpenGL/VertexArray.hpp"
#include "PositronEngineCore/Rendering/OpenGL/IndexBuffer.hpp"
#include "PositronEngineCore/Rendering/OpenGL/RenderOpenGL.hpp"
#include "PositronEngineCore/Modules/GUImodule.hpp"
#include "PositronEngineCore/Rendering/OpenGL/Sphere.h"

#include <imgui/imgui.h>
#include <glm/trigonometric.hpp>
#include <glad/glad.h>


namespace PositronEngine
{
    const char* vertex_shader = R"(
    #version 460

    // uniforms
    uniform mat4 model_matrix;
    uniform mat4 normal_matrix;
    uniform mat4 view_projection_matrix;

    // vertex attribs (input)
    layout(location=0) in vec3 vertex_position;
    layout(location=1) in vec3 vertex_normal;
    layout(location=2) in vec2 vertex_tex_coord;


    // varyings (output)
    out vec3 esNormal;
    out vec3 color;
    out vec2 texCoord;


    void main()
    {
        esNormal = vec3(normal_matrix * vec4(vertex_normal, 1.0));
        texCoord = vertex_tex_coord;

        gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position, 1.0);
    }
    )";

    const char* fragment_shader = R"(
        #version 460

        // uniforms
        layout(binding=0) uniform sampler2D in_texture;

        // varyings (input)
        in vec2 texCoord;
        in vec3 esNormal;

        // output
        out vec4 frag_color;

        void main() {
            frag_color = texture(in_texture, texCoord);
        }
    )";


    Sphere sphere(1.0f, 36, 18, true, 3);


    ShaderProgram* shader_program = nullptr;

    VertexBuffer* vertex_buffer_position_sphere = nullptr;
    VertexBuffer* vertex_buffer_normal_sphere = nullptr;
    VertexBuffer* vertex_buffer_texCoords_sphere = nullptr;

    IndexBuffer* index_buffer = nullptr;

    VertexArray* vertex_array_object = nullptr;

    GLuint earthTexture;

    GLuint attribVert;
    GLuint attribNorm;
    GLuint attribTex;

    float location[3] = {0.0f, 0.0f, 0.0f};
    float rotation[3] = {0.0f, 0.0f, 0.0f};
    float scale[3] = {1.0f, 1.0f, 1.0f};

    float _background_color[4] = {0.0f, 0.0f, 0.0f, 0.0f };


    Application::Application()
    {
        LOG_INFORMATION("Stating application");
    }

    Application::~Application()
    {
        delete shader_program;
        delete vertex_buffer_position_sphere;
        delete vertex_buffer_normal_sphere;
        delete vertex_buffer_texCoords_sphere;
        delete index_buffer;
        delete vertex_array_object;

        LOG_INFORMATION("Closing application");
    }

    glm::vec2 Application::getCurrentCursorPosition() const
    {
        return _window->getCurrentCursorPosition();
    }

    int Application::start(unsigned int window_width,unsigned int window_height, const char* window_title)
    {
        _window = std::make_unique<Window>(window_title, window_width, window_height);

        _event_dispatcher.addEventListener<EventMouseMoved>(
            [](EventMouseMoved& event)
            {
                //LOG_INFORMATION("Event MOUSE_MOVE is triggered. New mouse position is x:{0},y{1}", event.x, event.y);
            }
        );

        _event_dispatcher.addEventListener<EventWindowResized>(
            [](EventWindowResized& event)
            {
                LOG_INFORMATION("Event WINDOW_RESIZED is triggered. New window size is {0}x{1}", event.width, event.height);
            }
        );

        _event_dispatcher.addEventListener<EventWindowClose>(
            [&](EventWindowClose& event)
            {
                LOG_INFORMATION("Event WINDOW_CLOSING is triggered. Window is closing");
                _is_window_alive = false;

            }
        );

        _event_dispatcher.addEventListener<EventKeyPressed>(
            [](EventKeyPressed& event)
            {
                Input::pressKey(event.key_code);
                LOG_INFORMATION("Event KEY_PRESSED : Key '{0} is pressed'",static_cast<char>(event.key_code));
            }
        );

        _event_dispatcher.addEventListener<EventKeyReleased>(
            [](EventKeyReleased& event)
            {
                Input::releaseKey(event.key_code);
                LOG_INFORMATION("Event KEY_RELEASED : Key '{0} is released'",static_cast<char>(event.key_code));
            }
        );

        _event_dispatcher.addEventListener<EventMouseButtonPressed>(
            [&](EventMouseButtonPressed& event)
            {
                LOG_INFORMATION("Event MOUSE_BUTTON_PRESSED : Button '{0} is pressed (pos_x: {1}, pos_y: {2})'", static_cast<int>(event.mouse_button_code),
                event.x,
                event.y);

                Input::pressMouseButton(event.mouse_button_code);
                onMouseButtonEvent(event.mouse_button_code, event.x, event.y, true);
            }
        );

        _event_dispatcher.addEventListener<EventMouseButtonReleased>(
            [&](EventMouseButtonReleased& event)
            {
                LOG_INFORMATION("Event MOUSE_BUTTON_RELEASED : Button '{0} is released (pos_x: {1}, pos_y: {2})'", static_cast<int>(event.mouse_button_code),
                event.x,
                event.y);

                Input::releaseMouseButton(event.mouse_button_code);
                onMouseButtonEvent(event.mouse_button_code, event.x, event.y, false);
            }
        );

        _window->setEventCallback(
            [&](BaseEvent& event)
            {
                _event_dispatcher.dispatch(event);
            }
        );

        Image::Bmp bmp;
        if(!bmp.read("/home/n0rr/Desctop/C++/3D Engine Linux/PositronEngine/textures/earth2048.bmp"))
            LOG_ERROR("Texture is unloaded");    // exit if failed load image

        // get bmp info
        int width = bmp.getWidth();
        int height = bmp.getHeight();
        const unsigned char* data = bmp.getDataRGB();
        GLenum type = GL_UNSIGNED_BYTE;    // only allow BMP with 8-bit per channel

        // We assume the image is 8-bit, 24-bit or 32-bit BMP
        GLenum format;
        int bpp = bmp.getBitCount();
        if(bpp == 8)
            format = GL_RGB8;
        else if(bpp == 24)
            format = GL_RGB;
        else if(bpp == 32)
            format = GL_RGBA;
        else
            return 0;

        GLuint texture;
        glGenTextures(1, &texture);

        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        /*убери потом это в объект сцены*/
        shader_program = new ShaderProgram(vertex_shader, fragment_shader);
        if(!shader_program->isCompile())
        {
            return -4;
        }


        BufferLayout buffer_layout_two_elements_vector3
        {
            ShaderDataType::Float3,
            ShaderDataType::Float3,
            ShaderDataType::Float2
        };

        vertex_array_object = new VertexArray();

        vertex_buffer_position_sphere = new VertexBuffer(sphere.getInterleavedVertices(),
                                                sphere.getInterleavedVertexSize(),
                                                buffer_layout_two_elements_vector3);

        vertex_buffer_normal_sphere = new VertexBuffer(sphere.getNormals(),
                                                       sphere.getNormalSize(),
                                                       buffer_layout_two_elements_vector3);

        vertex_buffer_texCoords_sphere = new VertexBuffer(sphere.getTexCoords(),
                                                       sphere.getTexCoordSize(),
                                                       buffer_layout_two_elements_vector3);

        index_buffer = new IndexBuffer(sphere.getIndices(), sphere.getLineIndexSize());

        vertex_array_object->addVertexBuffer(*vertex_buffer_position_sphere);
        vertex_array_object->addVertexBuffer(*vertex_buffer_normal_sphere);
        vertex_array_object->addVertexBuffer(*vertex_buffer_texCoords_sphere);
        vertex_array_object->setIndexBuffer(*index_buffer);

        /*=========================================================*/

        RenderOpenGL::enableDepth();

        while(_is_window_alive)
        {
            RenderOpenGL::setBackgroundColor(_background_color);
            RenderOpenGL::clear();

            shader_program->bind();

            glm::mat4 location_matrix(1,                    0,                  0,               0,
                                    0,                    1,                  0,               0,
                                    0,                    0,                  1,               0,
                                    location[0],          location[1],        location[2],     1);


            glm::mat4 rotate_matrix_x(1,    0,                                  0,                                  0,
                                    0,    cos(glm::radians(rotation[0])),     sin(glm::radians(rotation[0])),     0,
                                    0,    -sin(glm::radians(rotation[0])),    cos(glm::radians(rotation[0])),     0,
                                    0,    0,                                  0,                                  1);


            glm::mat4 rotate_matrix_y(cos(glm::radians(rotation[1])),       0,      -sin(glm::radians(rotation[1])),    0,
                                    0,                                    1,      0,                                  0,
                                    sin(glm::radians(rotation[1])),       0,      cos(glm::radians(rotation[1])),     0,
                                    0,                                    0,      0,                                  1);


            glm::mat4 rotate_matrix_z( cos(glm::radians(rotation[2])),      sin(glm::radians(rotation[2])),     0,      0,
                                    -sin(glm::radians(rotation[2])),       cos(glm::radians(rotation[2])),     0,      0,
                                    0,                                     0,                                  1,      0,
                                    0,                                     0,                                  0,      1);

            glm::mat4x4 scale_matrix = {scale[0], 0, 0, 0,
                                        0, scale[1], 0, 0,
                                        0, 0, scale[2], 0,
                                        0,  0,  0,  1};


            camera.setProjection(is_perspective_mode ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);

            glm::mat4x4 model_matrix = location_matrix * rotate_matrix_x * rotate_matrix_y * rotate_matrix_z * scale_matrix;

            shader_program->setMatrix4("model_matrix", model_matrix);
            shader_program->setMatrix4("view_projection_matrix", camera.getProjectionMatrix() * camera.getViewMatrix());

            RenderOpenGL::draw(*vertex_array_object);

            GUImodule::onWindowStartUpdate();
            bool show = true;
            GUImodule::ShowExampleAppDockSpace(&show);

            ImGui::Begin("Color Picker");
            ImGui::SetWindowSize("Color Picker", ImVec2(350,60));
            ImGui::ColorEdit4("Color", _background_color);
            ImGui::End();

            ImGui::Begin("Local transform");
            ImGui::SetWindowSize("Local transform", ImVec2(400,100));
            ImGui::SliderFloat3("Location", location, -10.0f, 10.0f);
            ImGui::SliderFloat3("Rotate", rotation, -360.0f, 360.0f);
            ImGui::SliderFloat3("Scale", scale, -2.0f, 2.0f);
            ImGui::End();

            onGUIdraw();

            GUImodule::onWindowUpdateDraw();

            rotation[2] += 0.5f;
            _window->onUpdate();
            onUpdate();
        }

        glDeleteTextures(1, &texture);

        _window = nullptr;

        return 0;
    }
}
