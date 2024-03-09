#pragma once
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace PositronEngine
{
    class VertexArray
    {
        public:
            VertexArray(const VertexArray&) = delete;
            VertexArray& operator=(const VertexArray&) = delete;

            VertexArray();
            ~VertexArray();
            VertexArray& operator=(VertexArray&& vertex_array) noexcept;
            VertexArray(VertexArray&& vertex_array) noexcept;

            void addVertexBuffer(VertexBuffer& vertex_buffer);
            void setIndexBuffer(IndexBuffer& index_buffer);

            IndexBuffer* getIndexBuffer() { return _index_buffer; }
            VertexBuffer* getVertexBuffer() { return _vertex_buffer; }

            void bind() const;
            static void unbind();
            size_t getIndicesCount() const { return _indices_count; }


        private:
            unsigned int _id;
            unsigned int _elements_count;
            size_t _indices_count = 0;
            IndexBuffer* _index_buffer = nullptr;
            VertexBuffer* _vertex_buffer = nullptr;
    };
}
