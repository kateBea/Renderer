#include <cstddef>

#include "OpenGL/VertexArray.hh"
#include "Core/Common.hh"

namespace kT {
    VertexArray::VertexArray(VertexArray&& other) noexcept
        :   m_Id{ other.getId() } { other.m_Id = 0; }

    auto VertexArray::operator=(VertexArray&& other) noexcept -> VertexArray& {
        m_Id = other.getId();
        other.m_Id = 0;

        return *this;
    }

    auto VertexArray::getId() const -> std::uint32_t {
        return m_Id;
    }

    auto VertexArray::bind() const -> void {
        glBindVertexArray(getId());
    }

    auto VertexArray::unbind() -> void {
        glBindVertexArray(0);
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &m_Id);
    }

    VertexArray::VertexArray() {
        glGenVertexArrays(1, &m_Id);
    }

    auto VertexArray::useVertexBuffer(const VertexBuffer &buffer) -> void {
        bind();
        buffer.bind();

        std::uint32_t index{};
        for (const auto&i: buffer.getBufferLayout()) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, i.getAttributeCount(), i.getOpenGLAttributeDataType(),
                                  !i.isNormalized() ? GL_FALSE : GL_TRUE, buffer.getBufferLayout().getStride(), (const void*)i.getOffset());
            ++index;
        }
    }

}