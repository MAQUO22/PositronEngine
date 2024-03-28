#include "PositronEngineCore/CubeMap.hpp"
#include "PositronEngineCore/Log.hpp"
#include "PositronEngineCore/RenderOpenGL.hpp"

namespace PositronEngine
{
    namespace CubeMapMesh
    {
       Vertex verteces[] =
        {
            //position
            Vertex{glm::vec3(-1.0f,  -1.0f,  1.0f)},
            Vertex{glm::vec3(1.0f,  -1.0f,  1.0f)},
            Vertex{glm::vec3(1.0f,  -1.0f, -1.0f)},
            Vertex{glm::vec3(-1.0f,  -1.0f, -1.0f)},

            Vertex{glm::vec3(-1.0f,   1.0f,  1.0f)},
            Vertex{glm::vec3(1.0f,   1.0f,  1.0f)},
            Vertex{glm::vec3(1.0f,   1.0f, -1.0f)},
            Vertex{glm::vec3(-1.0f,   1.0f, -1.0f)},
        };

        GLuint indices[] =
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
    }

    CubeMap::CubeMap(CubeMapTexture* cube_map_texture)
    {
        _cube_map_texture = cube_map_texture;

        std::vector<Vertex> verts(CubeMapMesh::verteces, CubeMapMesh::verteces + sizeof(CubeMapMesh::verteces) / sizeof(Vertex));
        std::vector<GLuint> ind(CubeMapMesh::indices, CubeMapMesh::indices + sizeof(CubeMapMesh::indices) / sizeof(GLuint));

        _mesh = new Mesh(verts, ind);

        _shader_program = new ShaderProgram("skybox.vert", "skybox.frag");
        if(!_shader_program->isCompile())
        {
            LOG_CRITICAL("SKYBOX SHADER PROGRAM IS NOT COMPILED!!!");
        }
    }

    CubeMap::~CubeMap()
    {
        delete _cube_map_texture;
        delete _shader_program;
        delete _mesh;
    }

    CubeMapTexture* CubeMap::getCubeMapTexture()
    {
        return _cube_map_texture;
    }

    void CubeMap::draw(Camera& camera, float window_width, float window_height)
    {
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

        _shader_program->bind();

        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        view = glm::mat4(glm::mat3(glm::lookAt(camera.getLocation(), camera.getLocation() + camera.getDirection(), camera.getUp())));
        projection = glm::perspective(glm::radians(45.0f), (float)window_width / window_height, 0.1f, 1000.0f);
        _shader_program->setMatrix4("view", view);
        _shader_program->setMatrix4("projection", projection);

        _cube_map_texture->bind();

        RenderOpenGL::draw(*_mesh->getVertexArray());

        glDepthFunc(GL_LESS);
    }
}
