/**
 * @file mesh.hh
 * @author kate
 * @brief Defines the mesh class
 * @version 1.0
 * @date 2023-04-11
 */

// C++ Standard Library
#include <vector>
#include <cstddef>
#include <span>
#include <algorithm>
#include <utility>

// Project Libraries
#include <utils.hh>
#include <texture.hh>
#include <vao.hh>
#include <vbo.hh>
#include <vib.hh>

#ifndef MESH_HH
#define MESH_HH

namespace kate {
    class mesh {
    public:
        mesh() = default;

        mesh(std::vector<kate::vertex>& vertices, std::vector<std::uint32_t>& indices, std::vector<kate::texture>& textures);

        ~mesh() = default;
        
    private:
        auto setup() -> void;
        // Mesh data
        std::vector<kate::vertex> m_vertices{};
        std::vector<std::uint32_t> m_indices{};
        std::vector<kate::texture> m_textures{};

        // Render data
        kate::vbo m_vbo{};
        kate::vao m_vao{};
        kate::vib m_vib{};

    };


    // IMPLEMENTATION
    mesh::mesh(std::vector<kate::vertex>& vertices, std::vector<std::uint32_t>& indices, std::vector<kate::texture>& textures)  {
        m_vertices = std::move(vertices);
        m_indices = std::move(indices);
        m_textures = std::move(textures);

        setup();
    }

    auto mesh::setup() -> void {

    }
}

#endif