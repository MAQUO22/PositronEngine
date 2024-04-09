#include "PositronEngineCore/FrameBuffer.hpp"
#include "PositronEngineCore/Log.hpp"
#include <glad/glad.h>

namespace PositronEngine
{
    FrameBuffer::FrameBuffer()
    {
        glGenFramebuffers(1, &_id);
    }

    FrameBuffer::~FrameBuffer()
    {
        glDeleteFramebuffers(1, &_id);
    }

    void FrameBuffer::bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _id);
    }

    void FrameBuffer::checkFrameBufferErrors()
    {
        auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
            LOG_CRITICAL("Framebuffer not complete! error: {0}", fboStatus);
    }

    void FrameBuffer::connectTexture(unsigned int attachment, unsigned int id)
    {
        bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, id, 0);
    }

    void FrameBuffer::connectCubeMapTexture(unsigned int attachment, unsigned int id)
    {
        bind();
        glFramebufferTexture(GL_FRAMEBUFFER, attachment, id, 0);
    }
}
