/**
 * @file Mesh.hh
 * @author kate
 * @brief Defines the Mesh class
 * @version 1.0
 * @date 2023-04-11
 */

#ifndef MESH_HH
#define MESH_HH

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
#include "Shader.hh"
#include "Vao.hh"
#include "Vbo.hh"
#include "Vib.hh"

namespace kT {
    class Mesh {
    public:
        /**
         * Default constructs this Mesh
         * */
        explicit Mesh() = default;

        /**
         * Initializes this Mesh' vertex, index and texture objects with the
         * data we pass ass parameters. The parameters data are moved
         * @param vertices contains the vertex data for this mesh, like positions, texture coordinates, etc.
         * @param indices contains the indices for indexed drawing
         * @param textures contains the texture data for this mesh, see kT::Texture for more
         * */
        explicit Mesh(std::vector<kT::Vertex>&& vertices, std::vector<std::uint32_t>&& indices, std::vector<kT::Texture>&& textures);

        /**
         * Constructs and initializes this mesh with the contents of the other
         * mesh using move semantics, the other mesh is invalid after this operation
         * @param other moved from Mesh
         * */
        Mesh(Mesh&& other) noexcept;

        /**
         * Assigns to this mesh with the contents of the other
         * mesh using move semantics, the other mesh is invalid after this operation
         * @param other moved from Mesh
         * @returns *this
         * */
        auto operator=(Mesh&& other) noexcept -> Mesh&;

        /**
         * Renders this mesh
         * @param shader shader to be used for rendering
         * */
        auto draw(const Shader& shader) const -> void;

        /**
         * Frees resources owned by this mesh
         * */
        ~Mesh() = default;
        
    private:
        /**
         * Sets up this mesh, specifying the vao layout,
         * amongst other properties
         * */
        auto setup() -> void;

        std::vector<kT::Vertex>     m_Vertices{};
        std::vector<kT::Texture>    m_Textures{};
        std::vector<std::uint32_t>  m_Indices{};
        kT::Vbo                     m_Vbo{};
        kT::Vao                     m_Vao{};
        kT::Vib                     m_Vib{};

    };
}

#endif