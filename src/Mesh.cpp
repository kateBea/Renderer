// C++ Standard Library
#include <string>
#include <string_view>

// Project Libraries
#include "OpenGL/Mesh.hh"
#include <OpenGL/VertexBuffer.hh>

namespace kT {
    Mesh::Mesh(const std::vector<float> &vertices, const std::vector<std::uint32_t> &indices, std::vector<Texture> &&textures)
        :   m_VertexBuffer{ vertices, s_Layout }, m_ElementBuffer{ indices }, m_Textures{ std::move(textures) } {}

    Mesh::Mesh(Mesh&& other) noexcept
        :   m_VertexBuffer{ std::move(other.m_VertexBuffer) }, m_ElementBuffer{ std::move(other.m_ElementBuffer) }, m_Textures{ std::move(other.m_Textures) } {}

    auto Mesh::operator=(Mesh&& other) noexcept -> Mesh& {
        m_VertexBuffer = std::move(other.m_VertexBuffer);
        m_ElementBuffer = std::move(other.m_ElementBuffer);
        m_Textures = std::move(other.m_Textures);

        return *this;
    }
}