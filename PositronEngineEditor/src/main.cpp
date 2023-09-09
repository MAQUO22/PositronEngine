#include<iostream>
#include<PositroneEngineCore/Utilis/positron.hpp>

int main(){
    std::cout << "Hello my PositronEngine!" << std::endl;
    PositronEngine::hello_and_glfw();
    std::cin.get();
    return 0;
 }
