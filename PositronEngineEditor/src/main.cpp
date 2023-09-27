#include<iostream>
#include<PositronEngineCore/Application.hpp>
#include<memory>

class EditorApplication : public PositronEngine::Application
{
    int frame;

    virtual void onUpdate() override
    {
        //std::cout << "Editor application working and showing frame number-> " << frame++ << std::endl;
    }
};

int main()
{
    auto app = std::make_unique<EditorApplication>();
    int editor_start_code = app->start(1280,720,"Positron Engine Editor");

    std::cin.get();
    std::cout << editor_start_code << std::endl;
    return editor_start_code;
 }
