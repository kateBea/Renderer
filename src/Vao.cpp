//
// Created by zanet on 4/25/2023.
//

#include "../include/Vao.hh"

namespace Kate {
    // IMPLEMENTATION VERTEX ARRAY OBJECT CLASS

    Vao::Vao(Vao&& other) noexcept
            :   m_id{other.getId() }
    {
        other.m_id = 0;
    }

    Vao& Vao::operator=(Vao&& other) noexcept {
        this->m_id = other.getId();
        other.m_id = 0;

        return *this;
    }

    auto Vao::getId() const -> std::uint32_t {
        return this->m_id;
    }

    auto Vao::bind() const -> void {
        glBindVertexArray(this->m_id);
    }

    auto Vao::unbind() -> void {
        glBindVertexArray(0);
    }

    auto Vao::layout(std::uint32_t index, std::int32_t size, std::uint32_t pointer, std::int32_t stride, GLenum type) const -> void
    {
        bind();
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
                index,      // Attribute index
                size,       // Count of elements per attribute (e.g 3 floats per Vertex positions)
                type,       // type of data of the attribute
                GL_FALSE,   // normalized?
                stride * static_cast<decltype(stride)>(sizeof(float)),          // byte offset between consecutive vertices (the value taken by sizeof should be parametrized, its float right now for simplicity since the default type is GL_FLOAT)
                reinterpret_cast<const void*>(pointer * sizeof(float))  // pointer to the attribute within the buffer
        );
        unbind();
    }

    Vao::~Vao() {
        glDeleteVertexArrays(1, &m_id);
    }

    Vao::Vao() {
        glGenVertexArrays(1, &m_id);
    }

}