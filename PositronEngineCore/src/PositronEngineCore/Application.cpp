#include "PositronEngineCore/Application.hpp"
#include "PositronEngineCore/Log.hpp"
#include "PositronEngineCore/Window.hpp"
#include "PositronEngineCore/Event.hpp"
#include "PositronEngineCore/Camera.hpp"
#include "PositronEngineCore/Input.hpp"

#include "PositronEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "PositronEngineCore/Rendering/OpenGL/RenderOpenGL.hpp"
#include "PositronEngineCore/Modules/GUImodule.hpp"
#include "PositronEngineCore/Rendering/OpenGL/Planet.hpp"

#include <imgui/imgui.h>

namespace PositronEngine
{
    const char* vertex_shader = R"(
    #version 460

    // uniforms
    uniform mat4 model_matrix;
    uniform mat4 view_projection_matrix;

    // vertex attribs (input)
    layout(location=0) in vec3 vertex_position;
    layout(location=1) in vec3 vertex_normal;
    layout(location=2) in vec2 vertex_tex_coord;


    // varyings (output)
    out vec3 frag_normal;
    out vec3 frag_position;
    out vec2 texCoord;



    void main()
    {
        frag_normal = mat3(transpose(inverse(model_matrix))) * vertex_normal;
        vec4 world_vertex_position = model_matrix * vec4(vertex_position, 1.0);
        frag_position = world_vertex_position.xyz;
        texCoord = vertex_tex_coord;

        gl_Position = view_projection_matrix * world_vertex_position;
    }
    )";

    const char* fragment_shader = R"(
        #version 460

        // uniforms
        uniform vec3 light_color;
        uniform vec3 light_position;
        uniform float ambient_factor;
        uniform float diffuse_factor;
        layout(binding=0) uniform sampler2D in_texture;

        // varyings (input)
        in vec3 frag_normal;
        in vec3 frag_position;
        in vec2 texCoord;

        // output
        out vec4 frag_color;

        void main() {
            //ambient
            vec3 ambient = ambient_factor * light_color;

            //diffuse
            vec3 normal = normalize(frag_normal);
            vec3 ligth_direction = normalize(light_position - frag_position);
            vec3 diffuse = diffuse_factor * light_color * max(dot(normal, ligth_direction), 0.0);

            //specular
            vec3 specular = vec3(0.0f);

            frag_color = texture(in_texture, texCoord) * vec4(ambient + diffuse + specular, 1.0);
        }
    )";

    const char* ligth_vertex_shader = R"(
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

    const char* ligth_fragment_shader = R"(
        #version 460

        // uniforms
        uniform vec3 light_color;
        layout(binding=0) uniform sampler2D in_texture;

        // varyings (input)
        in vec3 esNormal;
        in vec2 texCoord;

        // output
        out vec4 frag_color;

        void main() {
            frag_color = texture(in_texture, texCoord) * vec4(light_color, 1.0f);
        }
    )";




    Planet space(1.0f, 36, 18, true, 3);
    Planet earth(1.0f, 36, 18, true, 3);
    Planet moon(1.0f, 36, 18, true, 3);
    Planet sun(1.0f, 36, 18, true, 3);

    ShaderProgram* shader_program = nullptr;
    ShaderProgram* ligth_shader_program = nullptr;

    float light_color[3] = {1.0f, 1.0f, 1.0f};
    float ambient_factor = 0.08f;
    float diffuse_factor = 1.5f;

    Application::~Application()
    {
        delete shader_program;
        delete ligth_shader_program;

        LOG_INFORMATION("Closing application");
    }

    Application::Application()
    {
        LOG_INFORMATION("Stating application");
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

        shader_program = new ShaderProgram(vertex_shader, fragment_shader);
        if(!shader_program->isCompile())
        {
            return -4;
        }

        ligth_shader_program = new ShaderProgram(ligth_vertex_shader, ligth_fragment_shader);
        if(!ligth_shader_program->isCompile())
        {
            return -4;
        }


        space.setScale(150.0f, 150.0f, 150.0f);

        sun.setScale(5.0f, 5.0f, 5.0f);

        earth.setLocation(0.0f, -26.0f, 0.0f);
        earth.setScale(0.9f, 0.9f, 0.9f);

        moon.setLocation(0.0f, -27.5f, 0.0f);
        moon.setScale(0.23f, 0.23f, 0.23f);

        space.setVertexArrayObject();

        earth.setTexture("/home/n0rr/Desctop/C++/3D Engine Linux/PositronEngine/textures/earth.bmp");
        moon.setTexture("/home/n0rr/Desctop/C++/3D Engine Linux/PositronEngine/textures/moon.bmp");
        sun.setTexture("/home/n0rr/Desctop/C++/3D Engine Linux/PositronEngine/textures/sun.bmp");
        space.setTexture("/home/n0rr/Desctop/C++/3D Engine Linux/PositronEngine/textures/stars.bmp");

        /*=========================================================*/

        RenderOpenGL::enableDepth();

        while(_is_window_alive)
        {
            RenderOpenGL::clear();

            shader_program->bind();

            camera.setProjection(is_perspective_mode ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
            shader_program->setMatrix4("view_projection_matrix", camera.getProjectionMatrix() * camera.getViewMatrix());
            shader_program->setVec3("light_color", glm::vec3(light_color[0], light_color[1], light_color[2]));
            shader_program->setFloat("ambient_factor", ambient_factor);
            shader_program->setFloat("diffuse_factor", diffuse_factor);
            shader_program->setVec3("light_position", glm::vec3(sun.getLocation()[0], sun.getLocation()[1], sun.getLocation()[2]));

            space.getTexture()->bind(0);
            space.updateMatrix();
            shader_program->setMatrix4("model_matrix", space.getModelMatrix());
            RenderOpenGL::draw(*space.getVertexArrayObject());

            earth.getTexture()->bind(0);
            earth.updateMatrix();
            shader_program->setMatrix4("model_matrix", earth.getModelMatrix());
            RenderOpenGL::draw(*space.getVertexArrayObject());

            moon.getTexture()->bind(0);
            moon.updateMatrix();
            shader_program->setMatrix4("model_matrix", moon.getModelMatrix());
            RenderOpenGL::draw(*space.getVertexArrayObject());

            ligth_shader_program->bind();
            sun.getTexture()->bind(0);
            sun.updateMatrix();
            ligth_shader_program->setMatrix4("view_projection_matrix", camera.getProjectionMatrix() * camera.getViewMatrix());
            ligth_shader_program->setMatrix4("model_matrix", sun.getModelMatrix());
            ligth_shader_program->setVec3("light_color", glm::vec3(light_color[0], light_color[1], light_color[2]));
            RenderOpenGL::draw(*space.getVertexArrayObject());


            GUImodule::onWindowStartUpdate();
            bool show = true;
            GUImodule::ShowExampleAppDockSpace(&show);

            ImGui::Begin("light_color");
            ImGui::ColorEdit3("light_color", light_color);
            ImGui::SliderFloat("ambient_factor", &ambient_factor, 0.0f, 1.0f);
            ImGui::SliderFloat("diffuse_factor", &diffuse_factor, 0.0f, 3.0f);
            ImGui::End();
            ImGui::Begin("Earth - Local transform");
            ImGui::SetWindowSize("Earth - Local transform", ImVec2(400,100));
            ImGui::SliderFloat3("Location", earth.getLocation(), -10.0f, 10.0f);
            ImGui::SliderFloat3("Rotate", earth.getRotation(), -360.0f, 360.0f);
            ImGui::SliderFloat3("Scale", earth.getScale(), -2.0f, 2.0f);
            ImGui::End();

            ImGui::Begin("Moon - Local transform");
            ImGui::SetWindowSize("Moon - Local transform", ImVec2(400,100));
            ImGui::SliderFloat3("Location", moon.getLocation(), -10.0f, 10.0f);
            ImGui::SliderFloat3("Rotate", moon.getRotation(), -360.0f, 360.0f);
            ImGui::SliderFloat3("Scale", moon.getScale(), -2.0f, 2.0f);
            ImGui::End();

            ImGui::Begin("Sun - Local transform");
            ImGui::SetWindowSize("Sun - Local transform", ImVec2(400,100));
            ImGui::SliderFloat3("Location", sun.getLocation(), -10.0f, 10.0f);
            ImGui::SliderFloat3("Rotate", sun.getRotation(), -360.0f, 360.0f);
            ImGui::SliderFloat3("Scale", sun.getScale(), -2.0f, 2.0f);
            ImGui::End();

            ImGui::Begin("Sky - Local transform");
            ImGui::SetWindowSize("Sky - Local transform", ImVec2(400,100));
            ImGui::SliderFloat3("Location", space.getLocation(), -10.0f, 10.0f);
            ImGui::SliderFloat3("Rotate", space.getRotation(), -360.0f, 360.0f);
            ImGui::SliderFloat3("Scale", space.getScale(), -100.0f, 100.0f);
            ImGui::End();

            onGUIdraw();

            GUImodule::onWindowUpdateDraw();

            earth.getRotation()[2] += 0.25f;
            moon.getRotation()[2] += 0.35f;
            sun.getRotation()[2] += 0.08f;
            _window->onUpdate();
            onUpdate();
        }

        _window = nullptr;

        return 0;
    }
}
