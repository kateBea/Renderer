/**
 * @file ElementBuffer.hh
 * @author kT
 * @brief Defines the Vertex index buffer class
 * @version 1.0
 * @date 2023-03-21
 */

#ifndef VIB_HH
#define VIB_HH

// C++ Standard Library
#include <vector>
#include <cstdint>

// Third-Party Libraries
#include "GL/glew.h"

namespace kT {
    class ElementBuffer {
    public:
        explicit ElementBuffer();
        /**
         * Creates a new Vertex index buffer and initializes it with the data
         * from indices. If no data is provided simply creates a new index buffer object with a valid id
         * @param indices buffer containing all the indices values
         * */
        explicit ElementBuffer(const std::vector<std::uint32_t> &indices, GLenum usage = GL_STATIC_DRAW);

        /**
         * Mark this Vertex index buffer as current
         * */
        auto bind() const -> void;

        /**
         * Move constructor
         * */
        ElementBuffer(ElementBuffer&& other) noexcept;

        /**
         * Move assigment
         * */
        auto operator=(ElementBuffer&& other) noexcept -> ElementBuffer&;

        /**
         * Copy constructor. Marked as delete to avoid ElementBuffer aliasing
         * Ensure one ElementBuffer id is held by one ElementBuffer object
         * */
        ElementBuffer(const ElementBuffer& other) = delete;

        /**
         * Copy assigment. Marked as delete to avoid ElementBuffer aliasing
         * Ensure one ElementBuffer id is held by one ElementBuffer object
         * */
        ElementBuffer& operator=(const ElementBuffer& other) = delete;

        /**
         * Returns the total count of indices of this Vertex index buffer
         * @return count of indices
         * */
        [[nodiscard]]
        auto getCount() const -> std::size_t;

        /**
         * Returns the identifier of this Vertex index buffer
         * @return id of this object
         * */
        [[nodiscard]]
        auto getId() const -> std::uint32_t;

        /**
         * Releases the currently bound Vertex index object
         * */
        static auto unbind() -> void;

        auto load(const std::vector<std::uint32_t> &indices, GLenum usage = GL_STATIC_DRAW) -> void;

        /**
         * Releases resources from this Vertex index buffer
         * */
        ~ElementBuffer();

    private:
        std::uint32_t m_Id{};
        std::size_t m_Count{};
    };
}

#endif // END VIB_HH