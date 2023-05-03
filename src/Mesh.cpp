#include <string>
#include <string_view>
#include <cstdint>

#include "../include/Mesh.hh"

namespace Kate {
    Mesh::Mesh(std::vector<Kate::Vertex>& vertices, std::vector<std::uint32_t>& indices, std::vector<Kate::Texture>& textures)
        :   m_Vertices{}, m_Indices{}, m_Textures{}, m_Vbo{}, m_Vao{}, m_Vib{}
    {
        m_Vertices = std::move(vertices);
        m_Indices = std::move(indices);
        m_Textures = std::move(textures);

        setup();
    }

    auto Mesh::setup() -> void {
        m_Vao.bind();
        m_Vbo.load(m_Vertices);
        m_Vib.load(m_Indices);

        m_Vbo.bind();
        // Vertex position attribute
        m_Vao.layout(0, 3, Kate::Vao::Attribute::POSITION);

        m_Vbo.bind();
        // Vertex Normals attribute
        m_Vao.layout(1, 3, Kate::Vao::Attribute::NORMAL);

        m_Vbo.bind();
        // Vertex Texture attribute
        m_Vao.layout(2, 2, Kate::Vao::Attribute::TEXTURE);
    }

    auto Mesh::draw(const Shader& shader) const -> void {
        std::uint32_t diffuseNr = 1;
        std::uint32_t specularNr = 1;
        for(std::uint32_t i = 0; i < m_Textures.size(); i++) {
            Texture::bindUnit(i); // activate proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            auto textureType = m_Textures[i].getType();
            if(textureType == Texture::TextureType::DIFFUSE)
                number = std::to_string(diffuseNr++);
            else if(textureType == Texture::TextureType::SPECULAR)
                number = std::to_string(specularNr++);

            shader.setUniformInt("material." + std::string(Texture::getStrType(textureType)) + number, i);
            glBindTexture(GL_TEXTURE_2D, m_Textures[i].getId());
        }
        glActiveTexture(GL_TEXTURE0);

        // draw mesh
        m_Vbo.bind();
        m_Vao.bind();
        m_Vib.bind();
        glDrawElements(GL_TRIANGLES, static_cast<std::int32_t >(m_Indices.size()), GL_UNSIGNED_INT, nullptr);
        Kate::Vao::unbind();
    }

    Mesh::Mesh(Mesh &&other) noexcept
        :   m_Vertices{ std::move(other.m_Vertices) }
        ,   m_Indices{ std::move(other.m_Indices) }
        ,   m_Textures{ std::move(other.m_Textures) }
        ,   m_Vbo{ std::move(other.m_Vbo) }
        ,   m_Vao{ std::move(other.m_Vao) }
        ,   m_Vib{ std::move(other.m_Vib) }
    {}

    auto Mesh::getVertexCount() -> std::size_t {
        return m_Vertices.size();
    }
}