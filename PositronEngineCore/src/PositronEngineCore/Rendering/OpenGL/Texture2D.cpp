#include "PositronEngineCore/Texture2D.hpp"
#include "PositronEngineCore/Log.hpp"
#include <glad/glad.h>

namespace PositronEngine
{
    const std::string PATH_TO_TEXTURES = "../../ResourceFiles/Textures/";

    Texture2D::Texture2D(Texture2D& texture)
    {
        _id = texture._id;
        _type = texture._type;
        _data = texture._data;
        texture._id = 0;
    }

    Texture2D::Texture2D(const unsigned int size_x, const unsigned int size_y)
    {
        glGenTextures(1, &_id);
        glBindTexture(GL_TEXTURE_2D, _id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, size_x, size_y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
    }


    Texture2D::Texture2D(const char* path, TextureType type)
    {
        int width, height, nrChannels;
        unsigned char* data = stbi_load((PATH_TO_TEXTURES + std::string(path)).c_str(), &width, &height, &nrChannels, 0);
        if(data)
        {
            stbi_set_flip_vertically_on_load(false);

            if(type == TextureType::diffuse)
            {
                glGenTextures(1, &_id);
                glBindTexture(GL_TEXTURE_2D, _id);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }

            else if(type == TextureType::specular)
            {
                glGenTextures(1, &_id);
                glBindTexture(GL_TEXTURE_2D, _id);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

            }

            else if(type == TextureType::normal)
            {
                glGenTextures(1, &_id);
                glBindTexture(GL_TEXTURE_2D, _id);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

            }

            else if(type == TextureType::height)
            {
                glGenTextures(1, &_id);
                glBindTexture(GL_TEXTURE_2D, _id);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }

            _data = data;
            _type = type;

            stbi_image_free(data);
        }
        else
        {
            LOG_CRITICAL("Failed to load image from path: {0}", path);
            stbi_image_free(data);
        }
    }

    Texture2D & PositronEngine::Texture2D::operator=(Texture2D && texture) noexcept
    {
        glDeleteTextures(1, &_id);
        _id = texture._id;
        _type = texture._type;
        _data = texture._data;
        texture._id = 0;
        return *this;

    }

    Texture2D::~Texture2D()
    {
        glDeleteTextures(1, &_id);
    }

    unsigned int Texture2D::getID()
    {
        return _id;
    }

    void Texture2D::bind()
    {
        glBindTexture(GL_TEXTURE_2D, _id);
    }

    void Texture2D::bindUnit(const unsigned int unit) const
    {
        glBindTextureUnit(unit, _id);
    }

    void Texture2D::unbindUnit() const
    {
        glBindTextureUnit(_id, 0);
    }

    Texture2D::Texture2D(Texture2D && texture) noexcept
    {
        _id = texture._id;
        _type = texture._type;
        _data = texture._data;
        texture._id = 0;
    }
}
