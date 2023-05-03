#include "../include/Vao.hh"
#include "Common.hh"
#include <cstddef>

namespace Kate {
    Vao::Vao(Vao&& other) noexcept
            :   m_id{ other.getId() }
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

    auto Vao::layout(std::uint32_t index, std::int32_t size, Attribute offs, GLenum type) const -> void
    {
        bind();
        glEnableVertexAttribArray(index);

        switch (offs) {
            case Attribute::POSITION:
                glVertexAttribPointer(
                        index,      // Attribute index
                        size,       // Count of elements per attribute
                        type,       // type of data of the attribute
                        GL_FALSE,   // normalized?
                        sizeof(Kate::Vertex),          // byte offset between consecutive vertices (the value taken by sizeof should be parametrized, its float right now for simplicity since the default type is GL_FLOAT)
                        reinterpret_cast<const void*>(offsetof(Kate::Vertex, m_Pos))  // pointer to the attribute within the buffer
                );
                break;
            case Attribute::NORMAL:
                glVertexAttribPointer(
                        index,      // Attribute index
                        size,       // Count of elements per attribute (e.g 3 floats per Vertex positions)
                        type,       // type of data of the attribute
                        GL_FALSE,   // normalized?
                        sizeof(Kate::Vertex),          // byte offset between consecutive vertices (the value taken by sizeof should be parametrized, its float right now for simplicity since the default type is GL_FLOAT)
                        reinterpret_cast<const void*>(offsetof(Kate::Vertex, m_Norm))  // pointer to the attribute within the buffer
                );
                break;
            case Attribute::TEXTURE:
                glVertexAttribPointer(
                        index,      // Attribute index
                        size,       // Count of elements per attribute (e.g 3 floats per Vertex positions)
                        type,       // type of data of the attribute
                        GL_FALSE,   // normalized?
                        sizeof(Kate::Vertex),          // byte offset between consecutive vertices (the value taken by sizeof should be parametrized, its float right now for simplicity since the default type is GL_FLOAT)
                        reinterpret_cast<const void*>(offsetof(Kate::Vertex, m_Texture))  // pointer to the attribute within the buffer
                );
                break;
            case Attribute::NONE:
                std::cerr << "Invalid attribute...\n";
                break;
        }

        unbind();
    }

    Vao::~Vao() {
        glDeleteVertexArrays(1, &m_id);
    }

    Vao::Vao() {
        glGenVertexArrays(1, &m_id);
    }

}