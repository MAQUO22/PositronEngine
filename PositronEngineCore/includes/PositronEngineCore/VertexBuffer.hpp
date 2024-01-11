#pragma once

#include <cstddef>
#include <vector>

namespace PositronEngine
{
    enum class ShaderDataType
    {
        Float,
        Float2,
        Float3,
        Float4,
        Int,
        Int2,
        Int3,
        Int4,
    };

    struct BufferElement
    {
        ShaderDataType shader_data_type;
        unsigned int component_type;
        size_t component_count;
        size_t size;
        size_t offset;

        BufferElement(const ShaderDataType shader_data_type);
    };

    class BufferLayout
    {
        public:
            BufferLayout(std::initializer_list<BufferElement> buffer_elements)
                : _elements(std::move(buffer_elements))
            {
               size_t offset = 0;
               _stride = 0;

               for(auto& element : _elements)
               {
                   element.offset = offset;
                   offset += element.size;
                   _stride += element.size;
               }
            }

            const std::vector<BufferElement>& getElement() const { return _elements; }
            size_t getStride() const { return _stride; }

        private:
            std::vector<BufferElement> _elements;
            size_t _stride = 0;
    };

    class VertexBuffer
    {
        public:
            enum class EUsage
            {
                Static,
                Dynamic,
                Stream
            };


            VertexBuffer() = delete;
            VertexBuffer(const VertexBuffer&) = delete;
            VertexBuffer& operator= (const VertexBuffer&) = delete;

            VertexBuffer(const void* data, const size_t buffer_size, BufferLayout buffer_layout, const EUsage usage = VertexBuffer::EUsage::Static);
            ~VertexBuffer();
            VertexBuffer& operator= (VertexBuffer && vertex_buffer) noexcept;
            VertexBuffer(VertexBuffer& vertex_buffer) noexcept;

            void bind() const;
            static void unbind() ;

            const BufferLayout& getLayout() const { return _buffer_layout; }
        private:
            unsigned int _id = 0;
            BufferLayout _buffer_layout;
    };
}
