//
// Created by zanet on 4/25/2023.
//

#include "Vib.hh"

namespace Kate {
    Vib::Vib() {
        glGenBuffers(1, &this->m_id);
    }

    Vib::Vib(const std::vector<std::uint32_t> &indices, GLenum usage)
            : m_id{}, m_count{}
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getId());
        load(indices, usage);
    }

    auto Vib::bind() const -> void {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getId());
    }

    auto Vib::unbind() -> void {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    Vib::~Vib() {
        glDeleteBuffers(1, &this->m_id);
    }

    auto Vib::getId() const -> std::uint32_t {
        return this->m_id;
    }

    auto Vib::getCount() const -> std::size_t {
        return this->m_count;
    }

    auto Vib::load(const std::vector<std::uint32_t> &indices, GLenum usage) -> void {
        if (!indices.empty()) {
            bind();
            m_count = indices.size();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(std::uint32_t), indices.data(), usage);
        }
    }
}