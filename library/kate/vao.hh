/**
 * @file vao.hh
 * @author kate
 * @brief Defines the vertex array object class
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

namespace kate {
    class vao {
    public:
        explicit vao();

        /**
         * Copy constructor. Marked as delete to avoid vao aliasing
         * Ensure one vao id is held by one vao object
         * */
        vao(const vao& other) = delete;

        /**
         * Copy assigment. Marked as delete to avoid vao aliasing
         * Ensure one vao id is held by one vao object
         * */
        vao& operator=(const vao& other) = delete;

        /**
         * Move constructor
         * */
        vao(vao&& other) noexcept;

        /**
         * Move constructor assignment
         * */
        vao& operator=(vao&& other) noexcept;

        [[nodiscard]]
        auto get_id() const -> std::uint32_t;

        auto bind() const -> void;

        static auto unbind() -> void;

        /**
         * Defines a layout for this vertex array object for the
         * currently bound vertex buffer.
         * */
        auto define_layout(std::uint32_t index, std::int32_t size, std::uint32_t pointer, GLenum type = GL_FLOAT) const -> void;

        ~vao();

    private:
        std::uint32_t m_id{};
    };

    // IMPLEMENTATION VERTEX ARRAY OBJECT CLASS

    inline vao::vao(vao&& other) noexcept
        :   m_id{ other.get_id() }
    {
        other.m_id = 0;
    }

    inline vao& vao::operator=(vao&& other) noexcept {
        this->m_id = other.get_id();
        other.m_id = 0;

        return *this;
    }

    inline auto vao::get_id() const -> std::uint32_t {
        return this->m_id;
    }

    inline auto vao::bind() const -> void {
        glBindVertexArray(this->m_id);
    }

    inline auto vao::unbind() -> void {
        glBindVertexArray(0);
    }

    auto vao::define_layout(std::uint32_t index, std::int32_t size, std::uint32_t pointer, GLenum type) const -> void
    {
        bind();
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
            index,      // Attribute index
            size,       // Count of elements per attribute (e.g 3 floats per vertex positions)
            type,       // type of data of the attribute
            GL_FALSE,   // normalized?
            0,          // byte offset between consecutive attributes
            reinterpret_cast<const void*>(pointer * sizeof(float))  // pointer to the attribute within the buffer
        );
    }

    inline vao::~vao() {
        glDeleteVertexArrays(1, &m_id);
    }

    inline vao::vao() {
        glGenVertexArrays(1, &m_id);
    }

}

#endif	// END VAO_HH