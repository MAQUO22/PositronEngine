#include "PositronEngineCore/Texture2D.hpp"
#include "PositronEngineCore/Log.hpp"
#include <glad/glad.h>

namespace PositronEngine
{
    Texture2D::Texture2D(const char* path)
    {
        if(!_image_bmp.read(path))
            LOG_ERROR("Texture is unloaded");

        int width = _image_bmp.getWidth();
        int height = _image_bmp.getHeight();
        const unsigned char* data = _image_bmp.getDataRGB();
        GLenum type = GL_UNSIGNED_BYTE;

        GLenum format;
        int bpp = _image_bmp.getBitCount();
        if(bpp == 8)
            format = GL_RGB8;
        else if(bpp == 24)
            format = GL_RGB;
        else
            format = GL_RGBA;

        glGenTextures(1, &_id);

        glBindTexture(GL_TEXTURE_2D, _id);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    Texture2D & PositronEngine::Texture2D::operator=(Texture2D && texture) noexcept
    {
        glDeleteTextures(1, &_id);
        _id = texture._id;
        _image_bmp = texture._image_bmp;
        texture._id = 0;
        return *this;

    }

    Texture2D::~Texture2D()
    {
        glDeleteTextures(1, &_id);
    }

    void Texture2D::bind(const unsigned int unit) const
    {
        glBindTextureUnit(unit, _id);
    }

    Texture2D::Texture2D(Texture2D && texture) noexcept
    {
        _id = texture._id;
        _image_bmp = texture._image_bmp;
        texture._id = 0;
    }
}
