#include "VertexBuffer.hpp"
#include "PositronEngineCore/Log.hpp"

#include <glad/glad.h>

namespace PositronEngine
{

    constexpr GLenum convertUsageToGLenum(const VertexBuffer::EUsage usage)
    {
        switch(usage)
        {
            case VertexBuffer::EUsage::Static:
                return GL_STATIC_DRAW;
                break;

            case VertexBuffer::EUsage::Dynamic:
                return GL_DYNAMIC_DRAW;
                break;

            case VertexBuffer::EUsage::Stream:
                return GL_STREAM_DRAW;
                break;

            default:
                LOG_ERROR("Unknow VertexBuffer usage !");
                return GL_STREAM_DRAW;
                break;
        }
    }

    VertexBuffer::VertexBuffer(const void* data, const size_t buffer_size, const EUsage usage)
    {
        glGenBuffers(1, &_id);
        glBindBuffer(GL_ARRAY_BUFFER, _id);
        glBufferData(GL_ARRAY_BUFFER, buffer_size, data, convertUsageToGLenum(usage));

    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &_id);
    }

    VertexBuffer& VertexBuffer::operator=(VertexBuffer && vertex_buffer) noexcept
    {
        _id = vertex_buffer._id;
        vertex_buffer._id = 0;
        return *this;
    }

    VertexBuffer::VertexBuffer(VertexBuffer& vertex_buffer) noexcept
    {
        _id = vertex_buffer._id;
        vertex_buffer._id = 0;
    }

    void VertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, _id);
    }

    void VertexBuffer::unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
