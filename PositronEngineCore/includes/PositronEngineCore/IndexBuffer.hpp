#pragma once

#include "VertexBuffer.hpp"

namespace PositronEngine
{
    class IndexBuffer
    {
        public:
            IndexBuffer(const IndexBuffer&) = delete;
            IndexBuffer& operator= (const IndexBuffer&) = delete;

            IndexBuffer(const void* data, const size_t count, const VertexBuffer::EUsage usage = VertexBuffer::EUsage::Static);
            ~IndexBuffer();
            IndexBuffer& operator= (IndexBuffer && index_buffer) noexcept;
            IndexBuffer(IndexBuffer& index_buffer) noexcept;

            void bind() const;
            static void unbind();
            size_t getCount() const { return _count; }

        private:
            unsigned int _id = 0;
            size_t _count;
    };
}
