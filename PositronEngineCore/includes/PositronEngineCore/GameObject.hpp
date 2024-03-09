#pragma once

#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>

namespace PositronEngine
{
    class ShaderProgram;
    class Camera;

    class GameObject
    {
        public:

            virtual void draw() = 0;

            virtual ~GameObject() {}

            void updateModelMatrix()
            {
                    glm::mat4 location_matrix(1,                    0,                  0,               0,
                                            0,                    1,                  0,               0,
                                            0,                    0,                  1,               0,
                                            location[0],          location[1],      location[2],    1);


                    glm::mat4 _rotate_matrix_x(1,    0,                                   0,                                   0,
                                            0,    cos(glm::radians(rotation[0])),     sin(glm::radians(rotation[0])),     0,
                                            0,    -sin(glm::radians(rotation[0])),    cos(glm::radians(rotation[0])),     0,
                                            0,    0,                                   0,                                   1);


                    glm::mat4 _rotate_matrix_y(cos(glm::radians(rotation[1])),       0,      -sin(glm::radians(rotation[1])),    0,
                                            0,                                     1,      0,                                   0,
                                            sin(glm::radians(rotation[1])),       0,      cos(glm::radians(rotation[1])),     0,
                                            0,                                     0,      0,                                   1);


                    glm::mat4 _rotate_matrix_z(cos(glm::radians(rotation[2])),       sin(glm::radians(rotation[2])),     0,      0,
                                            -sin(glm::radians(rotation[2])),      cos(glm::radians(rotation[2])),     0,      0,
                                            0,                                     0,                                   1,      0,
                                            0,                                     0,                                   0,      1);

                    glm::mat4x4 _scale_matrix(scale[0],  0,              0,          0,
                                            0,          scale[1],      0,          0,
                                            0,          0,              scale[2],  0,
                                            0,          0,              0,          1);

                    model_matrix = location_matrix * _rotate_matrix_x * _rotate_matrix_y * _rotate_matrix_z * _scale_matrix;
            }

            float location[3] = {0.0f, 0.0f, 0.0f};
            float rotation[3] = {0.0f, 0.0f, 0.0f};
            float scale[3] = {1.0f, 1.0f, 1.0f};

            glm::mat4 model_matrix;
    };
}
