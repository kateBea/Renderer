#include "OpenGL/ElementBuffer.hh"

namespace kT {
    ElementBuffer::ElementBuffer() {
        glGenBuffers(1, &this->m_Id);
    }

    ElementBuffer::ElementBuffer(const std::vector<std::uint32_t>& indices, GLenum usage)
            : m_Id{}, m_Count{}
    {
        glGenBuffers(1, &this->m_Id);
        load(indices, usage);
    }

    auto ElementBuffer::bind() const -> void {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getId());
    }

    auto ElementBuffer::unbind() -> void {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    ElementBuffer::~ElementBuffer() {
        glDeleteBuffers(1, &this->m_Id);
    }

    auto ElementBuffer::getId() const -> std::uint32_t {
        return this->m_Id;
    }

    auto ElementBuffer::getCount() const -> std::size_t {
        return this->m_Count;
    }

    auto ElementBuffer::load(const std::vector<std::uint32_t>& indices, GLenum usage) -> void {
        if (!indices.empty()) {
            bind();
            m_Count = indices.size() * sizeof(std::uint32_t);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count, indices.data(), usage);
            unbind();
        }
    }

    ElementBuffer::ElementBuffer(ElementBuffer &&other) noexcept
        :   m_Id{ other.getId() }, m_Count{ other.getCount() }
    {
        other.m_Id = 0;
        other.m_Count = 0;
    }

    auto ElementBuffer::operator=(ElementBuffer&& other) noexcept -> ElementBuffer& {
        m_Id = other.getId();
        m_Count = other.getCount();

        other.m_Id = 0;
        other.m_Count = 0;
        return *this;
    }
}