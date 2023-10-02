#include<PositronEngineCore/Application.hpp>

#include <memory>
#include <imgui/imgui.h>
#include <iostream>

class PositronEditor : public PositronEngine::Application
{
    public:
        virtual void onUpdate() override
        {
            //std::cout << "Editor application working and showing frame number->" << std::endl;
        }

        virtual void onGUIdraw() override
        {
            ImGui::Begin("Camera transform");
            ImGui::SetWindowSize("Camera transform", ImVec2(400,100));
            ImGui::SliderFloat3("Location", camera_location, -10.0f, 10.0f);
            ImGui::SliderFloat3("Rotate", camera_rotation, -360.0f, 360.0f);
            ImGui::Checkbox("Perspective mode", &is_perspective_mode);
            ImGui::End();
        }
};

int main()
{
    auto application = std::make_unique<PositronEditor>();

    int editor_code = application->start(1280,720,"Positron Engine Editor");

    std::cin.get();
    return editor_code;
 }
