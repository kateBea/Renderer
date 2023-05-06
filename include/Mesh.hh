/**
 * @file Mesh.hh
 * @author kT
 * @brief Defines the Mesh class
 * @version 1.0
 * @date 2023-04-11
 */

// C++ Standard Library
#include <vector>
#include <cstdint>
#include <span>
#include <algorithm>
#include <utility>

// Third-Party Libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Project Libraries
#include "Common.hh"
#include "Texture.hh"
#include "Vao.hh"
#include "Vbo.hh"
#include "Vib.hh"
#include "Shader.hh"

#ifndef MESH_HH
#define MESH_HH

namespace kT {
    class Mesh {
    public:
        /**
         *
         * */
        explicit Mesh() = default;

        /**
         * Initializes this Mesh' vertex, index and texture objects with the
         * data we pass ass parameters. The parameters data are moved, that's why
         * it is passed as a reference.
         * */
        explicit Mesh(std::vector<kT::Vertex>& vertices, std::vector<std::uint32_t>& indices, std::vector<kT::Texture>& textures);

        /**
         *
         * */
        Mesh(Mesh&& other) noexcept;

        /**
         *
         * */
        auto draw(const Shader& shader) const -> void;

        auto getVertexCount() -> std::size_t;

        ~Mesh() = default;
        
    private:
        auto setup() -> void;

        // Mesh data
        std::vector<kT::Vertex> m_Vertices{};
        std::vector<std::uint32_t> m_Indices{};
        std::vector<kT::Texture> m_Textures{};

        // Render data
        kT::Vbo m_Vbo{};
        kT::Vao m_Vao{};
        kT::Vib m_Vib{};

    };
}

#endif