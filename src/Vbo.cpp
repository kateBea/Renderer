//
// Created by zanet on 4/25/2023.
//

#include "../include/Vbo.hh"

namespace Kate {
    Vbo::Vbo(const std::vector<float>& vertices, GLenum usage) noexcept
        :   m_id{}, m_size{}
    {
        load(vertices, usage);
    }

    auto Vbo::bind() const -> void {
        glBindBuffer(GL_ARRAY_BUFFER, this->m_id);
    }

    auto Vbo::unbind() -> void {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    Vbo::~Vbo() {
        glDeleteBuffers(1, &this->m_id);
    }

    auto Vbo::getId() const -> std::uint32_t {
        return this->m_id;
    }

    auto Vbo::getSize() const -> std::size_t {
        return this->m_size;
    }

    auto Vbo::getCount() const -> std::size_t {
        return this->m_size / sizeof (float);
    }

    Vbo::Vbo(Vbo&& other) noexcept
            : m_id{other.getId() }, m_size{other.getSize() }
    {
        other.m_id = 0;
        other.m_size = 0;
    }

    Vbo& Vbo::operator=(Vbo&& other) noexcept {
        this->m_id = other.getId();
        this->m_size = other.getSize();

        other.m_id = 0;
        other.m_size = 0;

        return *this;
    }

    Vbo::Vbo() {
        glGenBuffers(1, &this->m_id);
    }

    auto Vbo::load(const std::vector<float>& vertices, GLenum usage) -> void {
        if (!vertices.empty()) {
            m_size = vertices.size() * sizeof(float);
            glBindBuffer(GL_ARRAY_BUFFER, this->m_id);
            glBufferData(GL_ARRAY_BUFFER, m_size, vertices.data(), usage);
        }
    }

}