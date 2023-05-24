#include <cstddef>

#include "../include/Vao.hh"
#include "../include/Common.hh"

namespace kT {
    Vao::Vao(Vao&& other) noexcept
        :   m_Id{ other.getId() }
    {
        other.m_Id = 0;
    }

    Vao& Vao::operator=(Vao&& other) noexcept {
        m_Id = other.getId();
        other.m_Id = 0;

        return *this;
    }

    auto Vao::getId() const -> std::uint32_t {
        return this->m_Id;
    }

    auto Vao::bind() const -> void {
        glBindVertexArray(this->m_Id);
    }

    auto Vao::unbind() -> void {
        glBindVertexArray(0);
    }

    auto Vao::layout(std::uint32_t index, std::int32_t size, Attribute offs, GLenum type) const -> void
    {
        bind();
        glEnableVertexAttribArray(index);

        // sizeof(kT::Vertex) is the offset in bytes between consecutive vertices
        // the type could vary, its float right now for simplicity since the default type parameter value is GL_FLOAT
        glVertexAttribPointer(index, size, type, GL_FALSE, sizeof(kT::Vertex),
                              reinterpret_cast<const void*>(getAttributeOffset(offs)));
        unbind();
    }

    Vao::~Vao() {
        glDeleteVertexArrays(1, &m_Id);
    }

    Vao::Vao() {
        glGenVertexArrays(1, &m_Id);
    }

    auto Vao::getAttributeOffset(Vao::Attribute attribute) -> std::int32_t {
        switch (attribute) {
            case Attribute::POSITION: return offsetof(kT::Vertex, m_Pos);
            case Attribute::NORMAL: return offsetof(kT::Vertex, m_Norm);
            case Attribute::TEXTURE: return offsetof(kT::Vertex, m_Texture);
            default: return -1;
        }
    }

}