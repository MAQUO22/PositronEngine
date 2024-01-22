#include "PositronEngineCore/Application.hpp"
#include "PositronEngineCore/Log.hpp"
#include "PositronEngineCore/Window.hpp"
#include "PositronEngineCore/Event.hpp"
#include "PositronEngineCore/Camera.hpp"
#include "PositronEngineCore/Input.hpp"
#include "PositronEngineCore/RenderOpenGL.hpp"
#include "PositronEngineCore/Modules/GUImodule.hpp"
#include "PositronEngineCore/ShaderProgram.hpp"
#include "PositronEngineCore/Planet.hpp"
#include "PositronEngineCore/Star.hpp"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

namespace PositronEngine
{
    float gamma = 0.280f;
    float exposure = 1.75f;
    float bloom_radius = 0.657;
    float blur_factor = 0.291;
    float kekw = 0.112f;
    bool show = true;
    int frame = 0;

    // Рисуем свое содержимое внутри окна ImGui


    PositronEngine::Planet space(1.0f, 36, 18, true, 3);
    PositronEngine::Planet earth(1.0f, 36, 18, true, 3);
    PositronEngine::Planet moon(1.0f, 36, 18, true, 3);
    PositronEngine::Star sun(1.0f, 36, 18, true, 3);

    PositronEngine::ShaderProgram* shader_program = nullptr;
    PositronEngine::ShaderProgram* ligth_shader_program = nullptr;
    PositronEngine::ShaderProgram* blur_program = nullptr;
    PositronEngine::ShaderProgram* roflan_chinilka;

    GLuint fullscreenQuadVAO, fullscreenQuadVBO;

    // Данные для полноэкранного квада (прямоугольника)
    float quadVertices[] = {
        // Позиции           Текстурные координаты
        -1.0f,  1.0f,        0.0f, 1.0f,
        -1.0f, -1.0f,        0.0f, 0.0f,
        1.0f,  1.0f,        1.0f, 1.0f,
        1.0f, -1.0f,        1.0f, 0.0f,
    };

    ShaderProgram* frame_buffer_program = nullptr;

    Application::~Application()
    {
        LOG_INFORMATION("Closing application");
        delete shader_program;
        delete ligth_shader_program;
        delete blur_program;
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

        frame_buffer_program = new ShaderProgram("post_processing.vert", "post_processing.frag");
        if(!frame_buffer_program->isCompile())
        {
            LOG_CRITICAL("FRAME BUFFER PROGRAM IS NOT COMPILED!");
            return -2;
        }

        blur_program = new ShaderProgram("post_processing.vert", "gaussian_blur.frag");
        if(!frame_buffer_program->isCompile())
        {
            LOG_CRITICAL("FRAME BUFFER PROGRAM IS NOT COMPILED!");
            return -2;
        }

        shader_program = new PositronEngine::ShaderProgram("planet_shader.vert", "planet_shader.frag");
        if(!shader_program->isCompile())
        {
            return -4;
        }

        ligth_shader_program  = new PositronEngine::ShaderProgram("light.vert", "light.frag");
        if(!ligth_shader_program->isCompile())
        {
            return -4;
        }

        //compileShaders();
        //initializeSpheres();

        space.setScale(150.0f, 150.0f, 150.0f);

        sun.setScale(7.0f, 7.0f, 7.0f);

        earth.setOrbirRadius(40.0f);
        earth.setLocation(earth.getOrbitRadius(), 0.0f, 0.0f);
        earth.setScale(2.0f, 2.0f, 2.0f);

        moon.setOrbirRadius(4.0f);
        moon.setLocation(moon.getOrbitRadius(), 7.0f, 0.0f);
        moon.setScale(0.5f, 0.5f, 0.5f);
        moon.setOrbitSpeed(0.006f);

        space.setVertexArrayObject();

        earth.addTexture("earth.bmp");
        earth.addTexture("earth_clouds.bmp");
        earth.addTexture("earth_nightmap.bmp");

        sun.addTexture("sun.bmp");

        moon.addTexture("moon.bmp");

        moon.setOrbirRadius(4.0f);
        moon.setLocation(moon.getOrbitRadius(), 7.0f, 0.0f);
        moon.setScale(0.5f, 0.5f, 0.5f);
        moon.setOrbitSpeed(0.006f);

        space.addTexture("stars.bmp");
        space.getTexture(0)->bind(0);

        unsigned int framebuffer;
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        unsigned int post_processing_texture;
        glGenTextures(1, &post_processing_texture);
        glBindTexture(GL_TEXTURE_2D, post_processing_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _window->getWidth(), _window->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, post_processing_texture, 0);

        unsigned int bloomTexture;
        glGenTextures(1, &bloomTexture);
        glBindTexture(GL_TEXTURE_2D, bloomTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _window->getWidth(), _window->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bloomTexture, 0);

        auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
            LOG_CRITICAL("Framebuffer not complete! error: {0}", fboStatus);

        GLuint image;
        glGenTextures(1, &image);

        // Привязываем текстуру
        glBindTexture(GL_TEXTURE_2D, image);

        // Выделяем память для текстуры
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _window->getWidth(), _window->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        // Устанавливаем параметры фильтрации
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unsigned int attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
        glDrawBuffers(2, attachments);


        unsigned int rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _window->getWidth(), _window->getHeight());
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
            LOG_CRITICAL("Framebuffer not complete! error: {0}", fboStatus);

        unsigned int pingpongFBO[2];
        unsigned int pingpongBuffer[2];
        glGenFramebuffers(2, pingpongFBO);
        glGenTextures(2, pingpongBuffer);
        for(unsigned int i = 0; i < 2; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
            glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _window->getWidth(), _window->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0);

            fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            if(fboStatus != GL_FRAMEBUFFER_COMPLETE)
                LOG_CRITICAL("Ping-pong framebuffer error: {0}", fboStatus);
        }



        unsigned int viewport;
        glGenFramebuffers(1, &viewport);
        glBindFramebuffer(GL_FRAMEBUFFER, viewport);

        GLuint resultTextureID;
        glGenTextures(1, &resultTextureID);
        glBindTexture(GL_TEXTURE_2D, resultTextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _window->getWidth(), _window->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, resultTextureID, 0);

        unsigned int rbo_v;
        glGenRenderbuffers(1, &rbo_v);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo_v);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _window->getWidth(), _window->getHeight());
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_v);

        fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
            LOG_CRITICAL("Framebuffer not complete! error: {0}", fboStatus);


        glGenVertexArrays(1, &fullscreenQuadVAO);
        glGenBuffers(1, &fullscreenQuadVBO);

        glBindVertexArray(fullscreenQuadVAO);

        glBindBuffer(GL_ARRAY_BUFFER, fullscreenQuadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        // Позиции
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Текстурные координаты
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Очистка состояния
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        RenderOpenGL::setElapsedTime(0.0f);
        RenderOpenGL::setRunTime(0.0f);

        glEnable(GL_FRAMEBUFFER_SRGB);

        RenderOpenGL::enableSync();

        while(_is_window_alive)
        {
            double frame_time = RenderOpenGL::getCurrentTime() - RenderOpenGL::getRunTime();
            RenderOpenGL::setRunTime(RenderOpenGL::getCurrentTime());

            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            glClearColor(pow(0.0f, gamma),pow(0.0f, gamma), pow(0.0f, gamma), 1.0f);
            RenderOpenGL::clear();
            RenderOpenGL::enableDepth();

            camera.setProjection(is_perspective_mode ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);


            shader_program->bind();

            shader_program->setMatrix4("view_projection_matrix", camera.getProjectionMatrix() * camera.getViewMatrix());
            shader_program->setVec3("light_color", glm::vec3(sun.getLightColor()[0], sun.getLightColor()[1], sun.getLightColor()[2]));
            shader_program->setFloat("ambient_factor", sun.getAmbientFactor());
            shader_program->setFloat("diffuse_factor", sun.getDiffuseFactor());
            shader_program->setVec3("camera_position", glm::vec3(camera.getLocation()[0],camera.getLocation()[1],camera.getLocation()[2]));
            shader_program->setInt("current_frame", frame);
            shader_program->setVec3("light_position", glm::vec3(sun.getLocation()[0], sun.getLocation()[1], sun.getLocation()[2]));
            shader_program->setBool("atphmosphere", 1);

            earth.getTexture(0)->bind(0);
            earth.getTexture(1)->bind(1);
            earth.getTexture(2)->bind(2);

            earth.updateMatrix();
            shader_program->setMatrix4("model_matrix", earth.getModelMatrix());
            PositronEngine::RenderOpenGL::draw(*space.getVertexArrayObject());
            earth.getTexture(0)->unbind(0);
            earth.getTexture(1)->unbind(1);
            earth.getTexture(2)->unbind(2);

            shader_program->setBool("atphmosphere", 0);
            moon.getTexture(0)->bind(0);
            moon.updateMatrix();
            shader_program->setMatrix4("model_matrix", moon.getModelMatrix());
            PositronEngine::RenderOpenGL::draw(*space.getVertexArrayObject());
            moon.getTexture(0)->unbind(0);


            sun.getTexture(0)->bind(0);
            ligth_shader_program->bind();
            sun.updateMatrix();
            ligth_shader_program->setMatrix4("view_projection_matrix", camera.getProjectionMatrix() * camera.getViewMatrix());
            ligth_shader_program->setMatrix4("model_matrix", sun.getModelMatrix());
            ligth_shader_program->setVec3("light_color", glm::vec3(sun.getLightColor()[0], sun.getLightColor()[1], sun.getLightColor()[2]));

            PositronEngine::RenderOpenGL::draw(*space.getVertexArrayObject());

            sun.getTexture(0)->unbind(0);

            sun.addRotation(0.15f);

            earth.addRotation(0.004f);
            earth.doOrbitalMotion(sun.getLocation());
            earth.addAngle();

            moon.doOrbitalMotion(earth.getLocation());
            moon.addAngle();
            //onEditorUpdate();


            bool horizontal = true, first_iteration = true;
            int amount = 6;
            blur_program->bind();
            blur_program->setInt("screen_texture", 0);
            blur_program->setFloat("baseBlurRadius", bloom_radius);
            blur_program->setFloat("baseBlurFactor", blur_factor);
            float d = sqrtf(pow(camera.getLocation()[0] - sun.getLocation()[0] , 2) +
                            pow(camera.getLocation()[1] - sun.getLocation()[1] , 2) +
                            pow(camera.getLocation()[2] - sun.getLocation()[2] , 2));

            //LOG_INFORMATION("distance - {0}", d);
            blur_program->setFloat("cameraDistance", d);
            blur_program->setFloat("blurDistanceFactor", kekw);


            for(unsigned int i = 0; i < amount; i++)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
                blur_program->setBool("horizontal", horizontal);

                if(first_iteration)
                {
                    glBindTexture(GL_TEXTURE_2D, bloomTexture);
                    first_iteration = false;
                }
                else
                {
                    glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]);
                }

                glBindVertexArray(fullscreenQuadVAO);
                RenderOpenGL::disableDepth();
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

                horizontal = !horizontal;
            }

            glBindFramebuffer(GL_FRAMEBUFFER, viewport);
            glClearColor(pow(1.0f, gamma),pow(1.0f, gamma), pow(1.0f, gamma), 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glBindTextureUnit(0, post_processing_texture);
            glBindTextureUnit(1, pingpongBuffer[!horizontal]);

            frame_buffer_program->bind();
            frame_buffer_program->setFloat("gamma", gamma);
            frame_buffer_program->setFloat("exposure", exposure);


            glBindVertexArray(fullscreenQuadVAO);
            RenderOpenGL::disableDepth();
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClearColor(pow(1.0f, gamma),pow(1.0f, gamma), pow(1.0f, gamma), 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);


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
            ImGui::Image((void*)(intptr_t)resultTextureID, ImVec2(1800.f, 900.f));
            ImGui::End();

            ImGui::Begin("Post-processing");
            ImGui::SliderFloat("Gamma", &gamma, 0.0f, 3.0f);
            ImGui::SliderFloat("bloom_radius", &bloom_radius, 0.0f, 3.0f);
            ImGui::SliderFloat("blur_factor", &blur_factor, 0.0f, 3.0f);
            ImGui::SliderFloat("Exposure", &exposure, 0.1f, 2.0f);
            ImGui::SliderFloat("blurDistanceFactor", &kekw, 0.0f, 1.0f);
            ImGui::End();


            ImGui::Begin("light_color");
            ImGui::ColorEdit3("light_color", sun.getLightColor());
            //ImGui::SliderFloat("ambient_factor", sun.getAmbientFactor(), 0.0f, 2.0f);
            //ImGui::SliderFloat("diffuse_factor", &diffuse_factor, 0.0f, 5.0f);
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
            _window->onUpdate();
            onInputUpdate();

            frame++;
            RenderOpenGL::postFrame(frame_time);
        }
        _window = nullptr;
        return 0;
    }
}
