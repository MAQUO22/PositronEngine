#pragma once
#include "VertexBuffer.hpp"

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

            void addBuffer(const VertexBuffer& vertex_buffer);
            void bind() const;
            static void unbind();



        private:
            unsigned int _id;
            unsigned int _elements_count;
    };
}
