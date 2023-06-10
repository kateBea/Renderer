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
#include "GL/glew.h"
#include "GLFW/glfw3.h"

// Project Libraries
#include "Core/Common.hh"
#include "Texture.hh"
#include "Shader.hh"
#include "VertexArray.hh"
#include "VertexBuffer.hh"
#include "ElementBuffer.hh"

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
        explicit Mesh(const std::vector<float> &vertices, const std::vector<std::uint32_t> &indices, std::vector<Texture> &&textures);

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

        auto getVertexBuffer() const -> const VertexBuffer& { return m_VertexBuffer; }
        auto getIndexBuffer() const -> const ElementBuffer& { return m_ElementBuffer; }
        auto getTextures() const -> const std::vector<Texture>& { return m_Textures; }

        auto getVertexCount() const -> std::size_t { return m_VertexBuffer.getCount(); }
        auto getIndexCount() const -> std::size_t { return m_ElementBuffer.getCount(); }
        auto getTextureCount() const -> std::size_t { return m_Textures.size(); }

        /**
         * Frees resources owned by this mesh
         * */
        ~Mesh() = default;
        
    private:
        // standard mesh data Layout
        inline static BufferLayout s_Layout{
                {ShaderDataType::FLOAT3_TYPE, "Attribute_Position"},
                {ShaderDataType::FLOAT3_TYPE, "Attribute_Normal"},
                {ShaderDataType::FLOAT2_TYPE, "Attribute_Texture_Coordinates"},
        };

        std::vector<Texture> m_Textures{};
        VertexBuffer m_VertexBuffer{};
        ElementBuffer  m_ElementBuffer{};

    };
}

#endif