#include <PositronEngineCore/Application.hpp>
#include <PositronEngineCore/Input.hpp>
#include <PositronEngineCore/RenderOpenGL.hpp>
#include <PositronEngineCore/Planet.hpp>
#include <PositronEngineCore/Star.hpp>
#include <PositronEngineCore/ShaderProgram.hpp>

#include <memory>
#include <imgui/imgui.h>
#include <iostream>

int kekes = 0;

PositronEngine::Planet space(1.0f, 36, 18, true, 3);
PositronEngine::Planet earth(1.0f, 36, 18, true, 3);
PositronEngine::Planet moon(1.0f, 36, 18, true, 3);
PositronEngine::Star sun(1.0f, 36, 18, true, 3);

PositronEngine::ShaderProgram* shader_program = nullptr;
PositronEngine::ShaderProgram* ligth_shader_program = nullptr;
PositronEngine::ShaderProgram* no_atmoshpere_program = nullptr;



class PositronEditor : public PositronEngine::Application
{
    public:
        double initial_mouse_position_x = 0.0f;
        double initial_mouse_position_y = 0.0f;

        ~PositronEditor()
        {
            delete shader_program;
            delete no_atmoshpere_program;
            delete ligth_shader_program;
        }

        virtual bool compileShaders() override
        {
            shader_program = new PositronEngine::ShaderProgram("with_atphmos.vert", "with_atphmos.frag");
            if(!shader_program->isCompile())
            {
                return -4;
            }

            ligth_shader_program  = new PositronEngine::ShaderProgram("light.vert", "light.frag");
            if(!ligth_shader_program->isCompile())
            {
                return -4;
            }

            no_atmoshpere_program = new PositronEngine::ShaderProgram("no_atphmos.vert", "no_atphmos.frag");
            if(!no_atmoshpere_program->isCompile())
            {
                return -4;
            }

            return 1;
        }

        virtual void onInputUpdate() override
        {

            glm::vec3 movement_delta{0, 0, 0};
            glm::vec3 rotation_delta{0, 0, 0};

            if(PositronEngine::Input::isKeyPressed(PositronEngine::KeyCode::KEY_W))
            {
                movement_delta.x += camera_speed;
            }

            if(PositronEngine::Input::isKeyPressed(PositronEngine::KeyCode::KEY_S))
            {
                movement_delta.x -= camera_speed;
            }

            if(PositronEngine::Input::isKeyPressed(PositronEngine::KeyCode::KEY_D))
            {
                movement_delta.y += camera_speed;
            }

            if(PositronEngine::Input::isKeyPressed(PositronEngine::KeyCode::KEY_A))
            {
                movement_delta.y -= camera_speed;
            }


            if(PositronEngine::Input::isMouseButtonPressed(PositronEngine::MouseButtonCode::MOUSE_BUTTON_RIGHT))
            {
                glm::vec2 current_cursor_position = getCurrentCursorPosition();

                if(PositronEngine::Input::isMouseButtonPressed(PositronEngine::MouseButtonCode::MOUSE_BUTTON_LEFT))
                {
                    camera.moveRight(static_cast<float>(current_cursor_position.x - initial_mouse_position_x) / 100.0f);
                    camera.moveUp(static_cast<float>(initial_mouse_position_y - current_cursor_position.y) / 100.0f);
                }
                else
                {
                    rotation_delta.z += static_cast<float>(initial_mouse_position_x - current_cursor_position.x) / 5.0f;
                    rotation_delta.y -= static_cast<float>(initial_mouse_position_y - current_cursor_position.y) / 5.0f;
                }

                initial_mouse_position_x = current_cursor_position.x;
                initial_mouse_position_y = current_cursor_position.y;
            }

            camera.moveAndRotation(movement_delta, rotation_delta);
        }

        virtual void onEditorUpdate() override
        {
            shader_program->bind();

            shader_program->setMatrix4("view_projection_matrix", camera.getProjectionMatrix() * camera.getViewMatrix());
            shader_program->setVec3("light_color", glm::vec3(sun.getLightColor()[0], sun.getLightColor()[1], sun.getLightColor()[2]));
            shader_program->setFloat("ambient_factor", sun.getAmbientFactor());
            shader_program->setFloat("diffuse_factor", sun.getDiffuseFactor());
            shader_program->setVec3("camera_position", glm::vec3(camera.getLocation()[0],camera.getLocation()[1],camera.getLocation()[2]));
            shader_program->setInt("current_frame", kekes);
            shader_program->setVec3("light_position", glm::vec3(sun.getLocation()[0], sun.getLocation()[1], sun.getLocation()[2]));

            earth.getTexture(0)->bind(0);
            earth.updateMatrix();
            shader_program->setMatrix4("model_matrix", earth.getModelMatrix());
            PositronEngine::RenderOpenGL::draw(*space.getVertexArrayObject());

            no_atmoshpere_program->bind();
            no_atmoshpere_program->setMatrix4("view_projection_matrix", camera.getProjectionMatrix() * camera.getViewMatrix());
            no_atmoshpere_program->setVec3("light_color", glm::vec3(sun.getLightColor()[0], sun.getLightColor()[1], sun.getLightColor()[2]));
            no_atmoshpere_program->setFloat("ambient_factor", sun.getAmbientFactor());
            no_atmoshpere_program->setFloat("diffuse_factor", sun.getDiffuseFactor());
            no_atmoshpere_program->setVec3("light_position", glm::vec3(sun.getLocation()[0], sun.getLocation()[1], sun.getLocation()[2]));

            moon.getTexture(0)->bind(0);
            moon.updateMatrix();
            no_atmoshpere_program->setMatrix4("model_matrix", moon.getModelMatrix());
            PositronEngine::RenderOpenGL::draw(*space.getVertexArrayObject());


            sun.getTexture(0)->bind(0);
            ligth_shader_program->bind();
            sun.updateMatrix();

            ligth_shader_program->setMatrix4("view_projection_matrix", camera.getProjectionMatrix() * camera.getViewMatrix());
            ligth_shader_program->setMatrix4("model_matrix", sun.getModelMatrix());
            ligth_shader_program->setVec3("light_color", glm::vec3(sun.getLightColor()[0], sun.getLightColor()[1], sun.getLightColor()[2]));

            PositronEngine::RenderOpenGL::draw(*space.getVertexArrayObject());

            sun.addRotation(0.015f);

            earth.addRotation(0.004f);
            earth.doOrbitalMotion(sun.getLocation());
            earth.addAngle();

            moon.doOrbitalMotion(earth.getLocation());
            moon.addAngle();

            kekes++;

        }

        void onMouseButtonEvent(const PositronEngine::MouseButtonCode mouse_button, const double x, const double y, bool pressed) override
        {
            initial_mouse_position_x = x;
            initial_mouse_position_y = y;
        }

        virtual void initializeSpheres() override
        {
            space.setScale(150.0f, 150.0f, 150.0f);

            sun.setScale(5.0f, 5.0f, 5.0f);

            earth.setOrbirRadius(20.0f);
            earth.setLocation(earth.getOrbitRadius(), 0.0f, 0.0f);
            earth.setScale(2.0f, 2.0f, 2.0f);

            moon.setOrbirRadius(4.0f);
            moon.setLocation(moon.getOrbitRadius(), 7.0f, 0.0f);
            moon.setScale(0.5f, 0.5f, 0.5f);
            moon.setOrbitSpeed(0.006f);

            space.setVertexArrayObject();

            earth.addTexture("earth.bmp");
            earth.getTexture(0)->bind(0);
            earth.addTexture("earth_clouds.bmp");
            earth.getTexture(1)->bind(1);
            earth.addTexture("earth_nightmap.bmp");
            earth.getTexture(2)->bind(2);

            sun.addTexture("sun.bmp");
            sun.getTexture(0)->bind(0);

            moon.addTexture("moon.bmp");
            moon.getTexture(0)->bind(0);

            moon.setOrbirRadius(4.0f);
            moon.setLocation(moon.getOrbitRadius(), 7.0f, 0.0f);
            moon.setScale(0.5f, 0.5f, 0.5f);
            moon.setOrbitSpeed(0.006f);

            space.addTexture("stars.bmp");
            space.getTexture(0)->bind(0);
        }

        virtual void onGUIdraw() override
        {
            camera_location[0] = camera.getLocation().x;
            camera_location[1] = camera.getLocation().y;
            camera_location[2] = camera.getLocation().z;

            camera_rotation[0] = camera.getRotation().x;
            camera_rotation[1] = camera.getRotation().y;
            camera_rotation[2] = camera.getRotation().z;

            ImGui::Begin("Camera transform");
            ImGui::SetWindowSize("Camera transform", ImVec2(400,100));
            if(ImGui::SliderFloat3("Location", camera_location, -10.0f, 10.0f))
            {
                camera.setLocation(glm::vec3(camera_location[0], camera_location[1], camera_location[2]));
            }
            if(ImGui::SliderFloat3("Rotate", camera_rotation, -360.0f, 360.0f))
            {
                camera.setRotation(glm::vec3(camera_rotation[0], camera_rotation[1], camera_rotation[2]));
            }
            ImGui::SliderFloat("Speed", &camera_speed, 0.05f, 20.0f);
            ImGui::Checkbox("Perspective camera mode", &is_perspective_mode);
            ImGui::End();

            ImGui::Begin("light_color");
            ImGui::ColorEdit3("light_color", sun.getLightColor());
            //ImGui::SliderFloat("ambient_factor", &sun.()), 0.0f, 2.0f);
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
        }
};

int main()
{
    auto application = std::make_unique<PositronEditor>();




    application->start(1280,720,"Positron Engine Editor");



    std::cin.get();
    return 0;
 }
