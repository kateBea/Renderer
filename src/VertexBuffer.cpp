#include "OpenGL/VertexBuffer.hh"

namespace kT {
    VertexBuffer::VertexBuffer(const std::vector<float>& vertices, const BufferLayout& bufferLayout, GLenum usage) {
        glCreateBuffers(1, &m_Id);
        m_ValidId = m_Id != 0;
        m_Layout = bufferLayout;

        load(vertices, usage);
    }


    VertexBuffer::VertexBuffer(VertexBuffer && other) noexcept
        :   m_Id{ other.m_Id }, m_Layout{ std::move(other.m_Layout) }, m_ValidId{ other.m_ValidId }, m_Size{ other.m_Size }
    {

        other.m_Id = 0;
        other.m_Size = 0;
    }

    auto VertexBuffer::operator=(VertexBuffer && other) noexcept -> VertexBuffer & {
        m_Id = other.getId();
        m_Size = other.getSize();

        other.m_Id = 0;
        other.m_Size = 0;
        return *this;
    }

    auto VertexBuffer::load(const std::vector<float> &vertices, GLenum usage) -> void {
        if (!m_ValidId) {
            glCreateBuffers(1, &m_Id);
            m_ValidId = m_Id != 0;
        }

        if (!vertices.empty()) {
            bind();
            m_Size = vertices.size() * sizeof(float);
            glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_Size), vertices.data(), usage);
            unbind();
        }
    }

}