#include "PositronEngineCore/VertexBuffer.hpp"
#include "PositronEngineCore/Log.hpp"

#include <glad/glad.h>

namespace PositronEngine
{
    constexpr unsigned int CastShaderDataTypeToComponentCount(const ShaderDataType shader_data_type)
    {
        switch(shader_data_type)
        {
            case ShaderDataType::Float:
            case ShaderDataType::Int:
                return 1;

            case ShaderDataType::Float2:
            case ShaderDataType::Int2:
                return 2;

            case ShaderDataType::Float3:
            case ShaderDataType::Int3:
                return 3;

            case ShaderDataType::Float4:
            case ShaderDataType::Int4:
                return 4;

            default:
                LOG_ERROR("CAST_SHADER_DATA_TYPE_TO_COMPONENT_COUNT: unknown ShaderDataType.");
                return 0;
        }
    }

    constexpr size_t getShaderDataTypeSize(const ShaderDataType shader_data_type)
    {
        switch(shader_data_type)
        {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
                return sizeof(GLfloat) * CastShaderDataTypeToComponentCount(shader_data_type);

            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
                return sizeof(GLint) * CastShaderDataTypeToComponentCount(shader_data_type);

            default:
                LOG_ERROR("GET_SHADER_DATA_TYPE_SIZE: unknown ShaderDataType.");
                return 0;
        }
    }

     constexpr unsigned int CastShaderDataTypeToComponentType(const ShaderDataType shader_data_type)
    {
        switch(shader_data_type)
        {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
                return GL_FLOAT;

            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
                return GL_INT;

            default:
                LOG_ERROR("CAST_SHADER_DATA_TYPE_TO_COMPONENT_TYPE: unknown ShaderDataType..");
                return GL_FLOAT;
        }
    }

    BufferElement::BufferElement(const ShaderDataType shader_data_type)
        : shader_data_type(shader_data_type),
        component_type(CastShaderDataTypeToComponentType(shader_data_type)),
        component_count(CastShaderDataTypeToComponentCount(shader_data_type)),
        size(getShaderDataTypeSize(shader_data_type)),
        offset(0)
    {
    }

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

    VertexBuffer::VertexBuffer(const void* data, const size_t buffer_size, BufferLayout buffer_layout, const EUsage usage)
        :_buffer_layout(std::move(buffer_layout))
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
        :_buffer_layout(std::move(vertex_buffer._buffer_layout))
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
