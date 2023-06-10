/**
 * @file VertexArray.hh
 * @author kT
 * @brief Defines the Vertex array object class
 * @version 1.0
 * @date 2023-03-21
 */

#ifndef VAO_HH
#define VAO_HH

// C++ Standard Library
#include <cstdint>
#include <array>

// Third-Party Libraries
#include "GL/glew.h"

// Project Libraries
#include "Core/Common.hh"
#include "VertexBuffer.hh"

namespace kT {
    class VertexArray {
    public:
        /**
         * Indicates a vertex buffer attribute
         * */
        enum class Attribute {
            POSITION,
            NORMAL,
            TEXTURE,
            NONE,
        };

        explicit VertexArray();

        /**
         * Copy constructor. Marked as delete to avoid VertexArray aliasing
         * Ensure one VertexArray id is held by one VertexArray object
         * */
        VertexArray(const VertexArray& other) = delete;

        /**
         * Copy assigment. Marked as delete to avoid VertexArray aliasing
         * Ensure one VertexArray id is held by one VertexArray object
         * */
        VertexArray& operator=(const VertexArray& other) = delete;

        /**
         * Move constructor
         * @param other other VertexArray from which we move data
         * */
        VertexArray(VertexArray&& other) noexcept;

        /**
         * Move constructor assignment
         * @param other other VertexArray from which we move data
         * @retunr *this
         * */
        VertexArray& operator=(VertexArray&& other) noexcept;

        /**
         * Returns the identifier of this Vertex Array Object
         * @return identifier of this vao
         * */
        [[nodiscard]]
        auto getId() const -> std::uint32_t;

        /**
         * Mark this Vertex Array Object as current
         * */
        auto bind() const -> void;

        /**
         * Unbinds the currently bound Vertex Array Object
         * */
        static auto unbind() -> void;

        auto useVertexBuffer(const VertexBuffer &buffer) -> void;

        ~VertexArray();

    private:
        std::uint32_t m_Id{};
    };
}

#endif	// END VAO_HH