#include "VertexArray.hpp"

#include <glad/glad.h>
#include <iostream>

namespace PositronEngine
{
    VertexArray::VertexArray():
        _id(0), _elements_count(0)
    {
        glGenVertexArrays(1, &_id);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &_id);
    }

    VertexArray& VertexArray::operator=(VertexArray&& vertex_array) noexcept
    {
        _id = vertex_array._id;
        _elements_count = vertex_array._elements_count;

        vertex_array._id = 0;
        vertex_array._elements_count = 0;

        return *this;
    }

    VertexArray::VertexArray(VertexArray&& vertex_array) noexcept
    {
        _id = vertex_array._id;
        _elements_count = vertex_array._elements_count;

        vertex_array._id = 0;
        vertex_array._elements_count = 0;
    }

    void VertexArray::bind() const
    {
        glBindVertexArray(_id);
    }

    void VertexArray::unbind()
    {
        glBindVertexArray(0);
    }

    void VertexArray::addBuffer(const VertexBuffer& vertex_buffer)
    {
        bind();
        vertex_buffer.bind();

        glEnableVertexAttribArray(_elements_count);
        glVertexAttribPointer(_elements_count, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        ++_elements_count;
    }
}
