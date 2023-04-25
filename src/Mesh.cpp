//
// Created by zanet on 4/25/2023.
//

#include "Mesh.hh"

namespace Kate {
    // IMPLEMENTATION
    Mesh::Mesh(std::vector<Kate::Vertex>& vertices,
                      std::vector<std::uint32_t>& indices,
                      std::vector<Kate::Texture>& textures)
            :   m_Vertices{}, m_Indices{}, m_Textures{}
    {
        m_Vertices = std::move(vertices);
        m_Indices = std::move(indices);
        m_Textures = std::move(textures);

        setup();
    }

    auto Mesh::setup() -> void {

    }
}