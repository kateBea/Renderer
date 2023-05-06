#include "../include/Vib.hh"

namespace kT {
    Vib::Vib() {
        glGenBuffers(1, &this->m_Id);
    }

    Vib::Vib(const std::vector<std::uint32_t>& indices, GLenum usage)
            : m_Id{}, m_Count{}
    {
        glGenBuffers(1, &this->m_Id);
        load(indices, usage);
    }

    auto Vib::bind() const -> void {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getId());
    }

    auto Vib::unbind() -> void {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    Vib::~Vib() {
        glDeleteBuffers(1, &this->m_Id);
    }

    auto Vib::getId() const -> std::uint32_t {
        return this->m_Id;
    }

    auto Vib::getCount() const -> std::size_t {
        return this->m_Count;
    }

    auto Vib::load(const std::vector<std::uint32_t>& indices, GLenum usage) -> void {
        if (!indices.empty()) {
            bind();
            m_Count = indices.size() * sizeof(std::uint32_t);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count, indices.data(), usage);
            unbind();
        }
    }

    Vib::Vib(Vib &&other) noexcept
        :   m_Id{ other.getId() }, m_Count{ other.getCount() }
    {
        other.m_Id = 0;
        other.m_Count = 0;
    }
}