/**
 * @file Mesh.hh
 * @author Kate
 * @brief Defines the Mesh class
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
#include <Texture.hh>
#include <Vao.hh>
#include <Vbo.hh>
#include <Vib.hh>

#ifndef MESH_HH
#define MESH_HH

namespace Kate {
    class Mesh {
    public:
        explicit Mesh() = default;

        explicit Mesh(std::vector<Kate::Vertex>& vertices,
                      std::vector<std::uint32_t>& indices,
                      std::vector<Kate::Texture>& textures);

        ~Mesh() = default;
        
    private:
        auto setup() -> void;

        // Mesh data
        std::vector<Kate::Vertex> m_Vertices{};
        std::vector<std::uint32_t> m_Indices{};
        std::vector<Kate::Texture> m_Textures{};

        // Render data
        Kate::Vbo m_Vbo{};
        Kate::Vao m_Vao{};
        Kate::Vib m_Vib{};

    };
}

#endif