#include "PositronEngineCore/Application.hpp"
#include "PositronEngineCore/Log.hpp"
#include "PositronEngineCore/Window.hpp"
#include "PositronEngineCore/Event.hpp"
#include "PositronEngineCore/Camera.hpp"
#include "PositronEngineCore/Input.hpp"
#include "PositronEngineCore/RenderOpenGL.hpp"
#include "PositronEngineCore/Modules/GUImodule.hpp"
#include "PositronEngineCore/ShaderProgram.hpp"
#include "PositronEngineCore/stb_image.h"
#include "PositronEngineCore/Mesh.hpp"

#include "PositronEngineCore/CubeMapTexture.hpp"
#include "PositronEngineCore/FrameBuffer.hpp"
#include "PositronEngineCore/RenderBuffer.hpp"
#include "PositronEngineCore/Primitives/CubePrimitive.hpp"
#include "PositronEngineCore/Primitives/SpherePrimitive.hpp"
#include "PositronEngineCore/Primitives/PlatePrimitive.hpp"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

namespace PositronEngine
{
    float light_color[3] = {1.0f, 1.0f, 1.0f};
    float light_position[3] = {1.0f, 1.0f, 1.0f};

    float ambient_factor = 0.314f;
    float diffuse_factor = 1.14f;

    float shininess = 32.0f;
    float specular_factor = 0.5f;

    bool bloom_activate = false;


    float gamma = 0.280f;
    float exposure = 1.75f;
    float bloom_radius = 0.657;
    float blur_factor = 0.291;
    float kekw = 0.112f;
    bool show = true;
    int frame = 0;

    ShaderProgram* frame_buffer_program = nullptr;
    ShaderProgram* blur_program = nullptr;
    ShaderProgram* shader_program = nullptr;
    ShaderProgram* ligth_shader_program = nullptr;
    ShaderProgram* skybox_program = nullptr;

    GLuint fullscreenQuadVAO, fullscreenQuadVBO;

    float quadVertices[] =
    {
       //Позиции         Текстурные координаты
        -1.0f,  1.0f,    0.0f, 1.0f,
        -1.0f, -1.0f,    0.0f, 0.0f,
         1.0f,  1.0f,    1.0f, 1.0f,
         1.0f, -1.0f,    1.0f, 0.0f,
    };


    float skyboxVertices[] =
    {
        -1.0f,  -1.0f,  1.0f,
         1.0f,  -1.0f,  1.0f,
         1.0f,  -1.0f, -1.0f,
        -1.0f,  -1.0f, -1.0f,
        -1.0f,   1.0f,  1.0f,
         1.0f,   1.0f,  1.0f,
         1.0f,   1.0f, -1.0f,
        -1.0f,   1.0f, -1.0f
    };

    unsigned int skyboxIndices[] =
    {
        1, 2, 6,
        6, 5, 1,
        0, 4, 7,
        7, 3, 0,
        4, 5, 6,
        6, 7, 4,
        0, 3, 2,
        2, 1, 0,
        0, 1, 5,
        5, 4, 0,
        3, 7, 6,
        6, 2, 3
    };

    Application::~Application()
    {
        LOG_INFORMATION("Closing application");
        delete frame_buffer_program;
        delete blur_program ;
        delete shader_program ;
        delete ligth_shader_program;
        delete skybox_program;

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

        camera.setViewportSize(static_cast<float>(window_width), static_cast<float>(window_height));

        _event_dispatcher.addEventListener<EventMouseMoved>(
            [](EventMouseMoved& event)
            {
                //LOG_INFORMATION("Event MOUSE_MOVE is triggered. New mouse position is x:{0},y{1}", event.x, event.y);
            }
        );

        _event_dispatcher.addEventListener<EventWindowResized>(
            [&](EventWindowResized& event)
            {
                LOG_INFORMATION("Event WINDOW_RESIZED is triggered. New window size is {0}x{1}", event.width, event.height);
                camera.setViewportSize(event.width, event.height);
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

        ShaderProgram* frame_buffer_program = new ShaderProgram("post_processing.vert", "post_processing.frag");
        if(!frame_buffer_program->isCompile())
        {
            LOG_CRITICAL("FRAME BUFFER PROGRAM IS NOT COMPILED!");
            return -2;
        }

        ShaderProgram* blur_program = new ShaderProgram("post_processing.vert", "gaussian_blur.frag");
        if(!frame_buffer_program->isCompile())
        {
            LOG_CRITICAL("FRAME BUFFER PROGRAM IS NOT COMPILED!");
            return -2;
        }

        ShaderProgram* shader_program = new ShaderProgram("default.vert", "default.frag", "default.geom");
        if(!shader_program->isCompile())
        {
            return -2;
        }

        ShaderProgram* ligth_shader_program = new ShaderProgram("light.vert", "light.frag");
        if(!ligth_shader_program->isCompile())
        {
            return -2;
        }

        ShaderProgram* skybox_program = new ShaderProgram("skybox.vert", "skybox.frag");
        if(!skybox_program->isCompile())
        {
            return -2;
        }

        SpherePrimitive sphere("sphere1");
        CubePrimitive cube("cube1");
        PlatePrimitive plate("plate1");

        unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glGenBuffers(1, &skyboxEBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        std::string facesCubemap[6] =
        {
            "../../ResourceFiles/Textures/SkyBox/Cloud/right.jpg",
             "../../ResourceFiles/Textures/SkyBox/Cloud/left.jpg",
             "../../ResourceFiles/Textures/SkyBox/Cloud/up.jpg",
             "../../ResourceFiles/Textures/SkyBox/Cloud/down.jpg",
            "../../ResourceFiles/Textures/SkyBox/Cloud/front.jpg",
            "../../ResourceFiles/Textures/SkyBox/Cloud/back.jpg"
        };

        CubeMapTexture cubemapTexture(facesCubemap);

        FrameBuffer framebuffer;

        Texture2D post_processing_texture(_window->getWidth(), _window->getHeight());
        framebuffer.connectTexture(GL_COLOR_ATTACHMENT0, post_processing_texture.getID());

        Texture2D bloomTexture(_window->getWidth(), _window->getHeight());
        framebuffer.connectTexture(GL_COLOR_ATTACHMENT1, bloomTexture.getID());

        FrameBuffer::checkFrameBufferErrors();

        Texture2D image(_window->getWidth(), _window->getHeight());

        unsigned int attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
        glDrawBuffers(2, attachments);

        RenderBuffer render_buffer_scene(_window->getWidth(), _window->getHeight());

        FrameBuffer::checkFrameBufferErrors();


        FrameBuffer pingpongFBO;
        Texture2D pingpongBuffer(_window->getWidth(), _window->getHeight());
        pingpongFBO.connectTexture(GL_COLOR_ATTACHMENT0, pingpongBuffer.getID());

        FrameBuffer::checkFrameBufferErrors();

        FrameBuffer pingpongFBO1;
        Texture2D pingpongBuffer1(_window->getWidth(), _window->getHeight());
        pingpongFBO1.connectTexture(GL_COLOR_ATTACHMENT0, pingpongBuffer1.getID());

        FrameBuffer::checkFrameBufferErrors();

        FrameBuffer viewport;

        Texture2D resultTextureID(_window->getWidth(), _window->getHeight());
        viewport.connectTexture(GL_COLOR_ATTACHMENT0, resultTextureID.getID());

        RenderBuffer render_buffer_viewport(_window->getWidth(), _window->getHeight());

        FrameBuffer::checkFrameBufferErrors();

        glGenVertexArrays(1, &fullscreenQuadVAO);
        glGenBuffers(1, &fullscreenQuadVBO);
        glBindVertexArray(fullscreenQuadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, fullscreenQuadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        RenderOpenGL::setElapsedTime(0.0f);
        RenderOpenGL::setRunTime(0.0f);

        glEnable(GL_FRAMEBUFFER_SRGB);

        RenderOpenGL::disableFaceCulling();
        RenderOpenGL::enableSync();

        while(_is_window_alive)
        {
            //===============================================ПРЕД_КАДР========================================================================

            double frame_time = RenderOpenGL::getCurrentTime() - RenderOpenGL::getRunTime();
            RenderOpenGL::setRunTime(RenderOpenGL::getCurrentTime());


            //================================================ОБНОВЛЕНИЕ======================================================================

            _window->onUpdate();
            onInputUpdate();

            //================================================БУФЕР_КАДРА_ПОСТ-ПРОЦЕССИНГА====================================================

            framebuffer.bind();
            glClearColor(pow(0.0f, gamma),pow(0.0f, gamma), pow(0.0f, gamma), 1.0f);
            RenderOpenGL::clear();
            RenderOpenGL::enableDepth();

            shader_program->bind();

            shader_program->setMatrix4("view_projection_matrix", camera.getProjectionMatrix() * camera.getViewMatrix());
            shader_program->setVec3("light_color", glm::vec3(light_color[0], light_color[1] ,light_color[2]));
            shader_program->setFloat("ambient_factor", ambient_factor);
            shader_program->setFloat("diffuse_factor", diffuse_factor);
            shader_program->setVec3("camera_position", camera.getLocation());
            shader_program->setVec3("light_position", sphere.getLocatinVec3());
            shader_program->setFloat("shininess", shininess);
            shader_program->setFloat("specular_factor", specular_factor);

            shader_program->setMatrix4("model_matrix", cube.getModerlMatrix());
            cube.draw();

            shader_program->setMatrix4("model_matrix", sphere.getModerlMatrix());
            sphere.draw();

            shader_program->setMatrix4("model_matrix", plate.getModerlMatrix());
            plate.draw();

            glDepthFunc(GL_LEQUAL);
            glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

            skybox_program->bind();

            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 projection = glm::mat4(1.0f);

            view  = glm::mat4(glm::mat3(glm::lookAt(camera.getLocation(), camera.getLocation() + camera.getDirection(), camera.getUp())));
            projection = glm::perspective(glm::radians(45.0f), (float)window_width / window_height, 0.1f, 1000.0f);
            skybox_program->setMatrix4("view", view);
            skybox_program->setMatrix4("projection", projection);

            glBindVertexArray(skyboxVAO);
            glActiveTexture(GL_TEXTURE0);
            cubemapTexture.bind();
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            glDepthFunc(GL_LESS);

            //===============================================================================================================================



            //================================================БУФЕР_КАДРА_ДЛЯ_ПИНГ_ПОНГ_БЛЮРА================================================


            bool horizontal = true, first_iteration = true;
            int amount = 6;
            blur_program->bind();
            blur_program->setBool("bloom", bloom_activate);

            for(unsigned int i = 0; i < amount; i++)
            {
                horizontal ? pingpongFBO1.bind() : pingpongFBO.bind();
                blur_program->setBool("horizontal", horizontal);

                if(first_iteration)
                {
                    bloomTexture.bind();
                    first_iteration = false;
                }
                else
                {
                    horizontal ? pingpongBuffer.bind() : pingpongBuffer1.bind();
                }

                glBindVertexArray(fullscreenQuadVAO);
                RenderOpenGL::disableDepth();
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

                horizontal = !horizontal;
            }

            //===============================================================================================================================


            //================================================БУФЕР_КАДРА_ДЛЯ_ВЬЮПОРТА=======================================================


            viewport.bind();
            glClearColor(pow(1.0f, gamma),pow(1.0f, gamma), pow(1.0f, gamma), 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            post_processing_texture.bindUnit(0);;
            pingpongBuffer.bindUnit(1);

            frame_buffer_program->bind();
            frame_buffer_program->setFloat("gamma", gamma);
            frame_buffer_program->setFloat("exposure", exposure);


            glBindVertexArray(fullscreenQuadVAO);
            RenderOpenGL::disableDepth();
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            //===============================================================================================================================



            //===============================================ОСНОВНОЙ_БУФЕР_КАДРА============================================================

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClearColor(pow(1.0f, gamma),pow(1.0f, gamma), pow(1.0f, gamma), 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            //===============================================ПОЛЬЗОВАТЕЛЬСКИЙ+ИНТЕРФЕЙС======================================================

            GUImodule::onWindowStartUpdate();
            GUImodule::ShowExampleAppDockSpace(&show);
            ImGui::ShowDemoWindow();

            ImGui::Begin("Scene");
            if (ImGui::TreeNode("Root")) {
                // Элемент 1
                if (ImGui::TreeNode("Object 1")) {
                    // Информация об объекте 1
                    ImGui::Text("Object details go here");

                    ImGui::TreePop();
                }

                // Элемент 2
                if (ImGui::TreeNode("Object 2")) {
                    // Информация об объекте 2
                    ImGui::Text("Object details go here");

                    ImGui::TreePop();
                }

                ImGui::TreePop();
            }

            ImGui::End();

            ImGui::Begin("Viewport");
            ImVec2 viewport_size = ImGui::GetWindowSize();
            camera.setViewportSize(viewport_size.x, viewport_size.y);
            ImGui::Image((void*)(intptr_t)resultTextureID.getID(), viewport_size);
            ImGui::End();

            ImGui::Begin("Post-processing");
            ImGui::Checkbox("Bloom activate", &bloom_activate);
            ImGui::SliderFloat("Gamma", &gamma, 0.0f, 3.0f);
            ImGui::SliderFloat("Exposure", &exposure, 0.1f, 2.0f);
            ImGui::End();

            ImGui::Begin("Cube");
            ImGui::SliderFloat3("location", cube.getLocation(), -10.0f, 10.0f);
            ImGui::SliderFloat3("rotation", cube.getRotation(), -360.0f, 360.0f);
            ImGui::SliderFloat3("scale", cube.getScale() , -5.0f, 5.0f);
            ImGui::End();

            ImGui::Begin("Sphere");
            ImGui::SliderFloat3("location", sphere.getLocation(), -10.0f, 10.0f);
            ImGui::SliderFloat3("rotation", sphere.getRotation(), -360.0f, 360.0f);
            ImGui::SliderFloat3("scale", sphere.getScale() , -5.0f, 5.0f);
            ImGui::End();

            ImGui::Begin("Plate");
            ImGui::SliderFloat3("location", plate.getLocation(), -10.0f, 10.0f);
            ImGui::SliderFloat3("rotation", plate.getRotation(), -360.0f, 360.0f);
            ImGui::SliderFloat3("scale", plate.getScale() , -5.0f, 5.0f);
            ImGui::End();

            ImGui::Begin("Direction light");
            ImGui::ColorEdit3("light_color", light_color);
            ImGui::SliderFloat3("light_position", light_position, -10.0f, 10.0f);
            ImGui::SliderFloat("ambient_factor", &ambient_factor, 0.0f, 2.0f);
            ImGui::SliderFloat("diffuse_factor", &diffuse_factor, 0.0f, 2.0f);
            ImGui::SliderFloat("shininess", &shininess, 1.0f, 128.0f);
            ImGui::SliderFloat("specular_factor", &specular_factor, 0.0f, 1.0f);
            ImGui::End();

            onGUIdraw();

            GUImodule::onWindowUpdateDraw();

            //===============================================================================================================================

            //===============================================ПОСТ_КАДР=======================================================================
            frame++;
            RenderOpenGL::postFrame(frame_time);
        }
        _window = nullptr;
        return 0;
    }
}
