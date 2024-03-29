#include "PositronEngineCore/VertexArray.hpp"

#include <glad/glad.h>
#include <iostream>

namespace PositronEngine
{
    VertexArray::VertexArray()
        : _id(0), _elements_count(0)
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

    void VertexArray::addVertexBuffer(std::shared_ptr<VertexBuffer>& vertex_buffer)
    {
        bind();
        vertex_buffer->bind();

        _vertex_buffer = vertex_buffer;

        for(const BufferElement current_element : vertex_buffer->getLayout().getElement())
        {
            glEnableVertexAttribArray(_elements_count);
            glVertexAttribPointer(_elements_count,
                                  static_cast<GLint>(current_element.component_count),
                                  current_element.component_type,
                                  GL_FALSE, // <-- normalize in grid (-1 : 1)
                                  static_cast<GLsizei>(vertex_buffer->getLayout().getStride()),
                                  reinterpret_cast<const void*>(current_element.offset) // <-- need pointer
            );
            ++_elements_count;
        }
    }

    void VertexArray::setIndexBuffer(std::shared_ptr<IndexBuffer>& index_buffer)
    {
        bind();
        index_buffer->bind();
        _index_buffer = index_buffer;
        _indices_count = index_buffer->getCount();
    }
}
