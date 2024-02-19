#include "PositronEngineCore/RenderBuffer.hpp"
#include "PositronEngineCore/Log.hpp"
#include <glad/glad.h>

namespace PositronEngine
{
    RenderBuffer::RenderBuffer(const unsigned int size_x, const unsigned int size_y)
    {
        glGenRenderbuffers(1, &_id);
        glBindRenderbuffer(GL_RENDERBUFFER, _id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size_x, size_y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _id);
    }

    RenderBuffer::~RenderBuffer()
    {
        glDeleteRenderbuffers(1, &_id);
    }

    void RenderBuffer::bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _id);
    }
}

