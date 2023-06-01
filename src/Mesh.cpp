// C++ Standard Library
#include <string>
#include <string_view>

// Project Libraries
#include "OpenGL/Mesh.hh"

namespace kT {
    Mesh::Mesh(std::vector<kT::Vertex>&& vertices, std::vector<std::uint32_t>&& indices, std::vector<kT::Texture>&& textures)
        :   m_Vertices{ std::move(vertices) }, m_Indices{ std::move(indices) }, m_Textures{ std::move(textures) }
        ,   m_Vbo{}, m_Vao{}, m_Vib{}
    {
        setup();
    }

    auto Mesh::setup() -> void {
        m_Vbo.load(m_Vertices);
        m_Vib.load(m_Indices);

        m_Vbo.bind();
        // Vertex position attribute
        m_Vao.layout(0, 3, kT::Vao::Attribute::POSITION);
        // Vertex Normals attribute
        m_Vao.layout(1, 3, kT::Vao::Attribute::NORMAL);
        // Vertex Texture attribute
        m_Vao.layout(2, 2, kT::Vao::Attribute::TEXTURE);
    }

    auto Mesh::draw(const Shader& shader) const -> void {
        std::uint32_t diffuseCount{ 1 };
        std::uint32_t specularCount{ 1 };
        std::uint32_t normalCount{ 1 };

        for(std::int32_t i = 0; i < m_Textures.size(); i++) {
            std::string number{};

            Texture::bindUnit(i);

            switch(m_Textures[i].getType()) {

                case Texture::TextureType::DIFFUSE:
                    number = std::to_string(diffuseCount++);
                    break;
                case Texture::TextureType::SPECULAR:
                    number = std::to_string(specularCount++);
                    break;
                case Texture::TextureType::NORMAL:
                    number = std::to_string(normalCount++);
                    break;
            }

            shader.setUniformInt("material." + std::string(Texture::getStrType(m_Textures[i].getType())), i);
            m_Textures[i].bind();
        }

        m_Vbo.bind();
        m_Vao.bind();
        m_Vib.bind();

        glDrawElements(GL_TRIANGLES, static_cast<std::int32_t>(m_Indices.size()), GL_UNSIGNED_INT, nullptr);

        kT::Vao::unbind();
        kT::Vbo::unbind();
        kT::Vib::unbind();
    }

    Mesh::Mesh(Mesh&& other) noexcept
        :   m_Vertices{ std::move(other.m_Vertices) }
        ,   m_Indices{ std::move(other.m_Indices) }
        ,   m_Textures{ std::move(other.m_Textures) }
        ,   m_Vbo{ std::move(other.m_Vbo) }
        ,   m_Vao{ std::move(other.m_Vao) }
        ,   m_Vib{ std::move(other.m_Vib) }
    {}

    auto Mesh::operator=(Mesh&& other) noexcept -> Mesh& {
        m_Vertices = std::move(other.m_Vertices);
        m_Indices = std::move(other.m_Indices);
        m_Textures = std::move(other.m_Textures);
        m_Vbo = std::move(other.m_Vbo);
        m_Vao = std::move(other.m_Vao);
        m_Vib = std::move(other.m_Vib);

        return *this;
    }

    auto Mesh::getVertexCount() const -> std::size_t {
        return m_Vertices.size();
    }

    auto Mesh::getIndexCount() const -> std::size_t {
        return m_Indices.size();
    }

    auto Mesh::getTextureCount() const -> std::size_t {
        return m_Textures.size();
    }
}