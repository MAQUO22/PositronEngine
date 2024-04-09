#include "PositronEngineCore/CubeMapTexture.hpp"
#include "PositronEngineCore/Log.hpp"
#include <glad/glad.h>

namespace PositronEngine
{
    const std::string PATH_TO_TEXTURES = "../../ResourceFiles/Textures/";

    CubeMapTexture::CubeMapTexture(CubeMapTexture& texture)
    {
        _id = texture._id;
        _data = texture._data;
        texture._id = 0;
    }

    CubeMapTexture::CubeMapTexture(const std::string* paths)
    {
        glGenTextures(1, &_id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        for (unsigned int i = 0; i < 6; i++)
        {
            int width, height, nrChannels;
            unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);
            if(data)
            {
                stbi_set_flip_vertically_on_load(false);
                glTexImage2D
                (
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0,
                    GL_RGB,
                    width,
                    height,
                    0,
                    GL_RGB,
                    GL_UNSIGNED_BYTE,
                    data
                );

                _data = data;

                stbi_image_free(data);
            }
            else
            {
                LOG_CRITICAL("Failed to load image: {0}", paths[i]);
                stbi_image_free(data);
            }
        }
    }

    CubeMapTexture::CubeMapTexture(const unsigned int width, const unsigned int height)
    {
        glGenTextures(1, &_id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    CubeMapTexture & CubeMapTexture::CubeMapTexture::operator=(CubeMapTexture && texture) noexcept
    {
        glDeleteTextures(1, &_id);
        _id = texture._id;
        _data = texture._data;
        texture._id = 0;
        return *this;

    }

    CubeMapTexture::~CubeMapTexture()
    {
        glDeleteTextures(1, &_id);
    }

    unsigned int CubeMapTexture::getID()
    {
        return _id;
    }

    void CubeMapTexture::bind()
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
    }

    void CubeMapTexture::bindUnit(const unsigned int unit) const
    {
        glBindTextureUnit(unit, _id);
    }

    void CubeMapTexture::unbindUnit() const
    {
        glBindTextureUnit(_id, 0);
    }

    CubeMapTexture::CubeMapTexture(CubeMapTexture && texture) noexcept
    {
        _id = texture._id;
        _data = texture._data;
        texture._id = 0;
    }
}

