#include "IndexBuffer.hpp"
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

    IndexBuffer::IndexBuffer(const void* data, const size_t count, const VertexBuffer::EUsage usage)
        : _count(count)
    {
        glGenBuffers(1, &_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, convertUsageToGLenum(usage));
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &_id);
    }

    IndexBuffer& IndexBuffer::operator= (IndexBuffer && index_buffer) noexcept
    {
        _id = index_buffer._id;
        _count = index_buffer._count;

        index_buffer._id = 0;
        index_buffer._count = 0;

        return *this;
    }


    IndexBuffer::IndexBuffer(IndexBuffer& index_buffer) noexcept
    {
        _id = index_buffer._id;
        _count = index_buffer._count;

        index_buffer._id = 0;
        index_buffer._count = 0;
    }

    void IndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
    }

    void IndexBuffer::unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
