/**
 * @file Vao.hh
 * @author Kate
 * @brief Defines the Vertex array object class
 * @version 1.0
 * @date 2023-03-21
 */

#ifndef VAO_HH
#define VAO_HH

#include <iostream>
#include <cstddef>
#include <array>

// Third-Party Libraries
#include <GL/glew.h>

namespace Kate {
    class Vao {
    public:
        explicit Vao();

        /**
         * Copy constructor. Marked as delete to avoid Vao aliasing
         * Ensure one Vao id is held by one Vao object
         * */
        Vao(const Vao& other) = delete;

        /**
         * Copy assigment. Marked as delete to avoid Vao aliasing
         * Ensure one Vao id is held by one Vao object
         * */
        Vao& operator=(const Vao& other) = delete;

        /**
         * Move constructor
         * */
        Vao(Vao&& other) noexcept;

        /**
         * Move constructor assignment
         * */
        Vao& operator=(Vao&& other) noexcept;

        [[nodiscard]]
        auto getId() const -> std::uint32_t;

        auto bind() const -> void;

        static auto unbind() -> void;

        /**
         * Defines a layout for this Vertex array object for the
         * currently bound Vertex buffer.
         * */
        auto layout(std::uint32_t index, std::int32_t size, std::uint32_t pointer, std::int32_t stride, GLenum type = GL_FLOAT) const -> void;

        ~Vao();

    private:
        std::uint32_t m_id{};
    };

    // IMPLEMENTATION VERTEX ARRAY OBJECT CLASS

    inline Vao::Vao(Vao&& other) noexcept
        :   m_id{other.getId() }
    {
        other.m_id = 0;
    }

    inline Vao& Vao::operator=(Vao&& other) noexcept {
        this->m_id = other.getId();
        other.m_id = 0;

        return *this;
    }

    inline auto Vao::getId() const -> std::uint32_t {
        return this->m_id;
    }

    inline auto Vao::bind() const -> void {
        glBindVertexArray(this->m_id);
    }

    inline auto Vao::unbind() -> void {
        glBindVertexArray(0);
    }

    auto Vao::layout(std::uint32_t index, std::int32_t size, std::uint32_t pointer, std::int32_t stride, GLenum type) const -> void
    {
        bind();
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
            index,      // Attribute index
            size,       // Count of elements per attribute (e.g 3 floats per Vertex positions)
            type,       // type of data of the attribute
            GL_FALSE,   // normalized?
            stride * sizeof(float),          // byte offset between consecutive vertices
            reinterpret_cast<const void*>(pointer * sizeof(float))  // pointer to the attribute within the buffer
        );
        unbind();
    }

    inline Vao::~Vao() {
        glDeleteVertexArrays(1, &m_id);
    }

    inline Vao::Vao() {
        glGenVertexArrays(1, &m_id);
    }

}

#endif	// END VAO_HH