#include<iostream>
#include<PositroneEngineCore/Application.hpp>
#include<memory>

class EditorApplication : public PositronEngine::Application{
    int frame;

    virtual void on_update() override{
        //std::cout << "Editor application working and showing frame number-> " << frame++ << std::endl;
    }
};

int main(){
    auto app = std::make_unique<EditorApplication>();
    int editor_start_code = app->start(1280,720,"Positron Engine");

    std::cin.get();
    return editor_start_code;
 }
