#pragma once
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include <memory>

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

            void addVertexBuffer(std::shared_ptr<VertexBuffer>& vertex_buffer);
            void setIndexBuffer(std::shared_ptr<IndexBuffer>& _index_buffer);

            std::shared_ptr<IndexBuffer> getIndexBuffer() { return _index_buffer; }
            std::shared_ptr<VertexBuffer> getVertexBuffer() { return _vertex_buffer; }

            void bind() const;
            static void unbind();
            size_t getIndicesCount() const { return _indices_count; }


        private:
            unsigned int _id;
            unsigned int _elements_count;
            size_t _indices_count = 0;
            std::shared_ptr<IndexBuffer> _index_buffer;
            std::shared_ptr<VertexBuffer> _vertex_buffer;
    };
}
