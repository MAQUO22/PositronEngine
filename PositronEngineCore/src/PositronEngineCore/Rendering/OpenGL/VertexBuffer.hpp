#pragma once

#include <cstddef>

namespace PositronEngine
{
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

            VertexBuffer(const void* data, const size_t buffer_size, const EUsage usage = VertexBuffer::EUsage::Static);
            ~VertexBuffer();
            VertexBuffer& operator= (VertexBuffer && vertex_buffer) noexcept;
            VertexBuffer(VertexBuffer& vertex_buffer) noexcept;

            void bind() const;
            static void unbind() ;

        private:
            unsigned int _id = 0;
    };
}
