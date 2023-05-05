/**
 * @file Vbo.hh
 * @author Kate
 * @brief Defines the Vertex buffer object class
 * @version 1.0
 * @date 2023-03-21
 */


#ifndef VBO_HH
#define VBO_HH

// C++ Standard Library
#include <vector>
#include <cstdint>

// Third-Party Libraries
#include <GL/glew.h>

// Project Libraries
#include "Common.hh"

namespace Kate {
    class Vbo {
    public:
        explicit Vbo();
        /**
         * Creates a new Vertex buffer and initializes it with the data
         * from indices. If no data is provided simply creates a new index
         * buffer object with a valid id
         * @param vertices buffer containing all the vertices
         * */
        explicit Vbo(const std::vector<Kate::Vertex>& vertices, GLenum usage = GL_STATIC_DRAW) noexcept;

        /**
         * Copy constructor. Marked as delete to avoid Vbo aliasing
         * Ensure one Vbo id is held by one Vbo object
         * */
        Vbo(const Vbo& other) = delete;

        /**
         * Copy assigment. Marked as delete to avoid Vbo aliasing
         * Ensure one Vbo id is held by one Vbo object
         * */
        Vbo& operator=(const Vbo& other) = delete;

        /**
         * Move constructor
         * */
        Vbo(Vbo&& other) noexcept;

        /**
         * Move constructor assignment
         * */
        Vbo& operator=(Vbo&& other) noexcept;

        /**
         * Mark this Vertex buffer as current
         * */
        auto bind() const -> void;

        /**
         * Returns the identifier of this Vertex buffer
         * @return id of this object
         * */
        [[nodiscard]]
        auto getId() const -> std::uint32_t;

        /**
         * Returns the total size in bytes of the contents of this Vertex buffer
         * @return total size in bytes of the vertices of this buffer
         * */
        [[nodiscard]]
        auto getSize() const -> std::size_t;

        /**
         * Returns the total count of vertices of this Vertex buffer
         * @return count of vertices
         * */
        [[nodiscard]]
        auto getCount() const -> std::size_t;

        /**
         * Releases the currently bound Vertex buffer object
         * */
        static auto unbind() -> void;

        auto load(const std::vector<Kate::Vertex>& vertices, GLenum usage = GL_STATIC_DRAW) -> void;
        /**
         * Releases resources from this Vertex buffer
         * */
        ~Vbo();

    private:
        std::uint32_t m_Id{};   // Identifier of this Vertex buffer object
        std::size_t m_Size{};   // size in BYTES of the total count of vertices
    };
}

#endif	// END VBO_HH