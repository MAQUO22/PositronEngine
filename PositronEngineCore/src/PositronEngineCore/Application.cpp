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
#include "PositronEngineCore/CubeMap.hpp"

#include "PositronEngineCore/FrameBuffer.hpp"
#include "PositronEngineCore/RenderBuffer.hpp"

#include "PositronEngineCore/Primitives/CubePrimitive.hpp"
#include "PositronEngineCore/Primitives/SpherePrimitive.hpp"
#include "PositronEngineCore/Primitives/PlatePrimitive.hpp"

#include "PositronEngineCore/LightSources/DirectionLight.hpp"
#include "PositronEngineCore/LightSources/PointLight.hpp"
#include "PositronEngineCore/LightSources/SpotLight.hpp"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

namespace PositronEngine
{

    int max_light_sources_for_type = 5;
    bool bloom_activate = false;
    bool draw_without_mesh = false;
    bool draw_skybox = true;

    float gamma = 0.280f;
    float exposure = 1.75f;

    bool show = true;
    int frame = 0;

    GLuint fullscreenQuadVAO, fullscreenQuadVBO;

    float quadVertices[] =
    {
       //Позиции         Текстурные координаты
        -1.0f,  1.0f,    0.0f, 1.0f,
        -1.0f, -1.0f,    0.0f, 0.0f,
         1.0f,  1.0f,    1.0f, 1.0f,
         1.0f, -1.0f,    1.0f, 0.0f,
    };

    Application::~Application()
    {
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

        std::shared_ptr<ShaderProgram> frame_buffer_program = std::make_shared<ShaderProgram>("post_processing.vert", "post_processing.frag");
        if(!frame_buffer_program->isCompile())
        {
            LOG_CRITICAL("FRAME BUFFER PROGRAM IS NOT COMPILED!");
            return -2;
        }

        std::shared_ptr<ShaderProgram> blur_program = std::make_shared<ShaderProgram>("post_processing.vert", "gaussian_blur.frag");
        if(!frame_buffer_program->isCompile())
        {
            LOG_CRITICAL("BLUR PROGRAM IS NOT COMPILED!");
            return -2;
        }

        std::shared_ptr<ShaderProgram> shadow_map_program = std::make_shared<ShaderProgram>("shadow_map.vert", "shadow_map.frag");
        if(!shadow_map_program->isCompile())
        {
            LOG_CRITICAL("SHADOW PROGRAM IS NOT COMPILED!");
            return -2;
        }

        SpherePrimitive sphere("sphere1");
        CubePrimitive cube("cube1");
        PlatePrimitive plate("plate1");
        PlatePrimitive plate1("plate2");
        PlatePrimitive plate2("plate3");
        PlatePrimitive plate3("plate4");
        PlatePrimitive plate4("plate5");
        PlatePrimitive plate5("plate6");


        DirectionLight dir_light;

        std::vector<Texture2D> textures_stones;
        textures_stones.push_back(Texture2D("stones_diffuse.jpg", TextureType::diffuse));
        textures_stones.push_back(Texture2D("stones_specular.jpg", TextureType::specular));
        textures_stones.push_back(Texture2D("stones_normal.jpg", TextureType::normal));

        std::vector<Texture2D> textures_wood;
        textures_wood.push_back(Texture2D("wood.jpg", TextureType::diffuse));
        textures_wood.push_back(Texture2D("wood_specular.jpg", TextureType::specular));
        textures_wood.push_back(Texture2D("wood_normal.jpg", TextureType::normal));

        std::vector<Texture2D> textures_stone;
        textures_stone.push_back(Texture2D("stone.jpg", TextureType::diffuse));
        textures_stone.push_back(Texture2D("stone_specular.jpg", TextureType::specular));
        textures_stone.push_back(Texture2D("stone_normal.jpg", TextureType::normal));


        std::vector<Texture2D> textures_light;

        LightReactionConfig stones_config
        {
            0.314f,  // ambient
            1.14f,   // diffuse
            0.5f,   // specular
            32.0f     // shininess
        };

        LightReactionConfig wood_config
        {
            0.84f,  // ambient
            1.5f,   // diffuse
            0.77f,   // specular
            14.0f     // shininess
        };

        std::shared_ptr<Material> stones = std::make_shared<Material>(textures_stones, stones_config);
        std::shared_ptr<Material> wood = std::make_shared<Material>(textures_wood, wood_config);
        std::shared_ptr<Material> stone = std::make_shared<Material>(textures_stone, stones_config);

        std::shared_ptr<LightMaterial> light_material = std::make_shared<LightMaterial>(textures_light);

        cube.setMaterial(stone);

        plate.setMaterial(wood);
        plate1.setMaterial(stones);
        plate2.setMaterial(wood);
        plate3.setMaterial(stones);
        plate4.setMaterial(wood);
        plate5.setMaterial(stones);

        sphere.setMaterial(stone);


        std::vector<std::unique_ptr<GameObject>> objects;
        objects.emplace_back(std::make_unique<SpherePrimitive>(std::move(sphere)));
        objects[0]->setLocation(-2.5, -2.1, 0.0);

        objects.emplace_back(std::make_unique<CubePrimitive>(std::move(cube)));
        objects[1]->setLocation(2.1, -2.1, 0.0);

        objects.emplace_back(std::make_unique<PlatePrimitive>(std::move(plate)));
        objects[2]->setLocation(0.0, 0.0, 2.0);
        objects[2]->setRotation(0.0, -90.0, 0.0);
        objects.emplace_back(std::make_unique<PlatePrimitive>(std::move(plate1)));
        objects[3]->setLocation(0.0, 2.0, 2.0);
        objects[3]->setRotation(0.0, -90.0, 0.0);
        objects.emplace_back(std::make_unique<PlatePrimitive>(std::move(plate2)));
        objects[4]->setLocation(0.0, -2.0, 2.0);
        objects[4]->setRotation(0.0, -90.0, 0.0);
        objects.emplace_back(std::make_unique<PlatePrimitive>(std::move(plate3)));
        objects[5]->setLocation(2.0, 2.0, 2.0);
        objects[5]->setRotation(0.0, -90.0, 0.0);
        objects.emplace_back(std::make_unique<PlatePrimitive>(std::move(plate4)));
        objects[6]->setLocation(2.0, 0.0, 2.0);
        objects[6]->setRotation(0.0, -90.0, 0.0);
        objects.emplace_back(std::make_unique<PlatePrimitive>(std::move(plate5)));
        objects[7]->setLocation(2.0, -2.0, 2.0);
        objects[7]->setRotation(0.0, -90.0, 0.0);





        std::vector<std::unique_ptr<LightObject>> light_objects;
        light_objects.emplace_back(std::make_unique<DirectionLight>());
        //light_objects.emplace_back(std::make_unique<PointLight>("point1"));
        // light_objects.emplace_back(std::make_unique<PointLight>("point2"));
        // light_objects.emplace_back(std::make_unique<SpotLight>("spot1"));
        // light_objects.emplace_back(std::make_unique<SpotLight>("spot2"));

        for(int i = 0; i < light_objects.size(); i++)
        {
            if(light_objects[i])
                light_objects[i]->setLightMaterial(light_material);
        }


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

        CubeMap skybox(cubemapTexture);

        FrameBuffer framebuffer;

        Texture2D post_processing_texture(_window->getWidth(), _window->getHeight(), TextureType::buffer);
        framebuffer.connectTexture(GL_COLOR_ATTACHMENT0, post_processing_texture.getID());

        Texture2D bloomTexture(_window->getWidth(), _window->getHeight(), TextureType::buffer);
        framebuffer.connectTexture(GL_COLOR_ATTACHMENT1, bloomTexture.getID());

        FrameBuffer::checkFrameBufferErrors();

        Texture2D image(_window->getWidth(), _window->getHeight(), TextureType::buffer);

        unsigned int attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
        glDrawBuffers(2, attachments);

        RenderBuffer render_buffer_scene(_window->getWidth(), _window->getHeight());

        FrameBuffer::checkFrameBufferErrors();


        FrameBuffer pingpongFBO;
        Texture2D pingpongBuffer(_window->getWidth(), _window->getHeight(), TextureType::buffer);
        pingpongFBO.connectTexture(GL_COLOR_ATTACHMENT0, pingpongBuffer.getID());

        FrameBuffer::checkFrameBufferErrors();

        FrameBuffer pingpongFBO1;
        Texture2D pingpongBuffer1(_window->getWidth(), _window->getHeight(), TextureType::buffer);
        pingpongFBO1.connectTexture(GL_COLOR_ATTACHMENT0, pingpongBuffer1.getID());

        FrameBuffer::checkFrameBufferErrors();

        FrameBuffer viewport;

        Texture2D resultTextureID(_window->getWidth(), _window->getHeight(), TextureType::buffer);
        viewport.connectTexture(GL_COLOR_ATTACHMENT0, resultTextureID.getID());

        FrameBuffer shadowMapFBO;

        Texture2D shadowMapDirectionTexture(2048, 2048, TextureType::shadow);
        shadowMapFBO.connectTexture(GL_DEPTH_ATTACHMENT, shadowMapDirectionTexture.getID());

        // glDrawBuffer(GL_NONE);
        // glReadBuffer(GL_NONE);
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);

        FrameBuffer spot_buffer;

        Texture2D spot_shadow(2048, 2048, TextureType::shadow);
        spot_buffer.connectTexture(GL_DEPTH_ATTACHMENT, spot_shadow.getID());

        // glDrawBuffer(GL_NONE);
        // glReadBuffer(GL_NONE);
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);



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

            //===============================================SHADOW_MAP_DIRECTION_LIGH==========================================================

            glEnable(GL_DEPTH_TEST);
            glViewport(0, 0, shadowMapDirectionTexture.getWidth(), shadowMapDirectionTexture.getHeight());
            shadowMapFBO.bind();
            glClear(GL_DEPTH_BUFFER_BIT);

            for(size_t i = 0; i < objects.size(); i++)
            {
                glm::vec3 lightPosition = 15.0f * light_objects[0]->getDirectionVec3();

                glm::mat4 viewMatrix = glm::lookAt(lightPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

                glm::mat4 projectionMatrix = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, 0.1f, 150.0f);

                glm::mat4 lightSpaceMatrix = projectionMatrix * viewMatrix;

                objects[i]->draw(shadow_map_program, lightSpaceMatrix);
            }

            //==============================================SHADOW_MAPS_SPOT_LIGHT============================================================

            glEnable(GL_DEPTH_TEST);
            glViewport(0, 0, spot_shadow.getWidth(), spot_shadow.getHeight());
            spot_buffer.bind();
            glClear(GL_DEPTH_BUFFER_BIT);

            for(size_t i = 0; i < objects.size(); i++)
            {
                for(size_t j = 0; j < light_objects.size(); j++)
                {
                    if(light_objects[j]->getLightType() == LightType::spot)
                    {
                        glm::mat4 viewMatrix = glm::lookAt(light_objects[j]->getLocationVec3(),
                                                    light_objects[j]->getDirectionVec3(),
                                                    glm::vec3(0.0f, 0.0f, 1.0f));

                        glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);

                        glm::mat4 lightSpaceMatrix = projectionMatrix * viewMatrix;

                        objects[i]->draw(shadow_map_program, lightSpaceMatrix);
                    }
                }

            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            //================================================БУФЕР_КАДРА_ПОСТ-ПРОЦЕССИНГА====================================================
            glViewport(0, 0, _window->getWidth(), _window->getHeight());

            framebuffer.bind();
            glClearColor(pow(0.0f, gamma),pow(0.0f, gamma), pow(0.0f, gamma), 1.0f);
            RenderOpenGL::clear();
            RenderOpenGL::enableDepth();

            if(draw_without_mesh)
            {
                for(size_t i = 0; i < light_objects.size(); i++)
                {
                    light_objects[i]->drawWithoutMesh(camera);
                }
            }
            else
            {
                for(size_t i = 0; i < light_objects.size(); i++)
                {
                    light_objects[i]->draw(camera);
                }
            }

            for(size_t i = 0; i < objects.size(); i++)
            {
                for(size_t j = 0, m = 0; j < light_objects.size(); j++)
                {
                    if(light_objects[j]->getLightType() == LightType::direction)
                    {
                        glm::vec3 lightPosition = 15.0f * light_objects[j]->getDirectionVec3();

                        glm::mat4 viewMatrix = glm::lookAt(lightPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

                        glm::mat4 projectionMatrix = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, 0.1f, 150.0f);

                        glm::mat4 lightSpaceMatrix = projectionMatrix * viewMatrix;

                        objects[i]->getMaterial()->getShaderProgram()->bind();
                        objects[i]->getMaterial()->getShaderProgram()->setMatrix4("light_space_matrix", lightSpaceMatrix);

                        shadowMapDirectionTexture.bindUnit(3);

                        objects[i]->draw(camera, light_objects);
                    }
                    else if(light_objects[j]->getLightType() == LightType::spot)
                    {
                        glm::mat4 viewMatrix = glm::lookAt(light_objects[j]->getLocationVec3(),
                                                    light_objects[j]->getDirectionVec3(),
                                                    glm::vec3(0.0f, 0.0f, 1.0f));

                        glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);

                        glm::mat4 lightSpaceMatrix = projectionMatrix * viewMatrix;
                        std::string uniform_space_matrix = "spot_light_space_matix[" + std::to_string(m) + "]";

                        if( i < 3)
                        {
                            objects[i]->getMaterial()->getShaderProgram()->bind();
                            objects[i]->getMaterial()->getShaderProgram()->setMatrix4(uniform_space_matrix.c_str(), lightSpaceMatrix);
                        }


                        spot_shadow.bindUnit(4);

                        m++;

                        objects[i]->draw(camera, light_objects);
                    }
                }



            }

            if(draw_skybox)
            {
                skybox.draw(camera);
            }

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
                if (ImGui::TreeNode("Object 1")) {
                    ImGui::Text("Object details go here");

                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Object 2")) {
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

            // for(int i = 0 ; i < shadow_maps_spot_light.size(); i++)
            // {
            //     ImGui::Begin("SHADOW MAPS SPOT LIGHT");
            //     ImGui::SetWindowSize(ImVec2(1280, 720));
            //     ImGui::Image((void*)(intptr_t)shadow_maps_spot_light[i].getID(),
            //                  ImVec2(shadow_maps_spot_light[i].getWidth(), shadow_maps_spot_light[i].getHeight()));
            //     ImGui::End();
            // }

            ImGui::Begin("SHADOW MAPS SPOT LIGHT");
                ImGui::SetWindowSize(ImVec2(1620, 1000));
                ImGui::Image((void*)(intptr_t)spot_shadow.getID(),
                             ImVec2(spot_shadow.getWidth(), spot_shadow.getHeight()));
                ImGui::End();

            ImGui::Begin("Post-processing");
            ImGui::Checkbox("Bloom activate", &bloom_activate);
            ImGui::SliderFloat("Gamma", &gamma, 0.0f, 3.0f);
            ImGui::SliderFloat("Exposure", &exposure, 0.1f, 2.0f);
            ImGui::End();

            ImGui::Begin("Primitives");
            if(ImGui::Button("Add Cube"))
            {
                objects.push_back(std::make_unique<CubePrimitive>("cube_butt"));
                objects[objects.size() - 1]->setMaterial(stone);
            }
            ImGui::SameLine();

            if(ImGui::Button("Add Sphere"))
            {
                objects.push_back(std::make_unique<SpherePrimitive>("sphere_butt"));
                objects[objects.size() - 1]->setMaterial(stone);
            }
            ImGui::SameLine();

            if(ImGui::Button("Add Plate"))
            {
                objects.push_back(std::make_unique<PlatePrimitive>("plate_butt"));
                objects[objects.size() - 1]->setMaterial(stone);
            }
            ImGui::SameLine();

            ImGui::Checkbox("Draw SkyBox", &draw_skybox);

            ImGui::Spacing();

            for(size_t i = 0; i < objects.size(); i++)
            {
                char label_location[64];
                char label_rotation[64];
                char label_scale[64];
                snprintf(label_location, 64, "%s_LOCATION", objects[i]->getName().c_str());
                snprintf(label_rotation, 64, "%s_ROTATION", objects[i]->getName().c_str());
                snprintf(label_scale, 64, "%s_SCALE", objects[i]->getName().c_str());


                ImGui::SliderFloat3(label_location, objects[i]->getLocation(), -10.0f, 10.0f);
                ImGui::SliderFloat3(label_rotation, objects[i]->getRotation(), -360.0f, 360.0f);
                ImGui::SliderFloat3(label_scale, objects[i]->getScale() , -5.0f, 5.0f);
                ImGui::Spacing();
            }
            
            ImGui::End();

            ImGui::Begin("Material");
            ImGui::SliderFloat("ambient_factor", &stones_config.ambient, 0.0f, 2.0f);
            ImGui::SliderFloat("diffuse_factor", &stones_config.diffuse, 0.0f, 2.0f);
            ImGui::SliderFloat("shininess", &stones_config.shininess, 1.0f, 128.0f);
            ImGui::SliderFloat("specular_factor", &stones_config.specular, 0.0f, 1.0f);
            ImGui::End();


            ImGui::Begin("Light Sources");
            ImGui::Checkbox("Draw points light with NO MESH", &draw_without_mesh);

            if(ImGui::Button("Add Point Light"))
            {
                if(LightTypeCounter::getNumberOfPointLights() < max_light_sources_for_type)
                {
                    light_objects.push_back(std::make_unique<PointLight>("point_butt"));
                    light_objects[light_objects.size() - 1]->setLightMaterial(light_material);
                }
            }
            ImGui::SameLine();

            if(ImGui::Button("Add Spot Light"))
            {
                if(LightTypeCounter::getNumberOfSpotLights() < max_light_sources_for_type)
                {
                    light_objects.push_back(std::make_unique<SpotLight>("spot_butt"));
                    light_objects[light_objects.size() - 1]->setLightMaterial(light_material);
                }
            }

            ImGui::Spacing();


            for(size_t i = 0; i < light_objects.size(); i++)
            {
                char label_color[64];
                char label_location[64];
                char label_direction[64];

                if(light_objects[i]->getLightType() == LightType::point)
                {

                    char label_const_attenation[64];
                    char label_linear_attenation[64];

                    snprintf(label_color, 64, "POINT_LIGHT_COLOR %d", i + 1);
                    snprintf(label_location, 64, "POINT_LIGHT_LOCATION %d", i + 1);
                    snprintf(label_const_attenation, 64, "POINT_LIGHT_CONST_ATTENATION %d", i + 1);
                    snprintf(label_linear_attenation, 64, "POINT_LIGHT_LINEAR_ATTENATION %d", i + 1);

                    ImGui::ColorEdit3(label_color, light_objects[i]->getColor());
                    ImGui::SliderFloat3(label_location, light_objects[i]->getLocation(), -10.0f, 10.0f);
                    ImGui::SliderFloat(label_linear_attenation, light_objects[i]->getPtrLinearAttenuation() ,0.0f, 0.3f);
                    ImGui::SliderFloat(label_const_attenation, light_objects[i]->getPtrConstantAttenuation() ,0.0f, 0.3f);


                    ImGui::Spacing();
                }
                else if(light_objects[i]->getLightType() == LightType::spot)
                {
                    char label_outer_cone[64];
                    char label_inner_cone[64];

                    snprintf(label_color, 64, "SPOT_LIGHT_COLOR %d", i + 1);
                    snprintf(label_location, 64, "SPOT_LIGHT_LOCATION %d", i + 1);
                    snprintf(label_direction, 64, "SPOT_LIGHT_DIRECTION %d", i + 1);
                    snprintf(label_outer_cone, 64, "SPOT_LIGHT_OUTER_CONE %d", i + 1);
                    snprintf(label_inner_cone, 64, "SPOT_LIGHT_INNER_CONE %d", i + 1);


                    ImGui::ColorEdit3(label_color, light_objects[i]->getColor());
                    ImGui::SliderFloat3(label_location, light_objects[i]->getLocation(), -10.0f, 10.0f);
                    ImGui::SliderFloat3(label_direction, light_objects[i]->getDirection(), -10.0f, 10.0f);
                    ImGui::SliderFloat(label_inner_cone, light_objects[i]->getPtrInnerCone(), light_objects[i]->getOuterCone(), 2.0f);
                    ImGui::SliderFloat(label_outer_cone, light_objects[i]->getPtrOuterCone(), 0.0f, light_objects[i]->getInnerCone());

                    ImGui::Spacing();
                }
                else
                {
                    snprintf(label_direction, 64, "DIRECT_LIGHT_DIRECTION");
                    snprintf(label_color, 64, "DIRECT_LIGHT_COLOR");

                    ImGui::ColorEdit3(label_color, light_objects[i]->getColor());
                    ImGui::SliderFloat3(label_direction, light_objects[i]->getDirection(), -10.0f, 10.0f);
                    ImGui::Spacing();
                }
            }

            ImGui::End();








            stones->setLightConfig(stones_config);
            //cube.setMaterial(stones);

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
