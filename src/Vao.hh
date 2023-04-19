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

        /**
         * Defines the layout for the currently bound Vertex Buffer Object (vbo)
         * for this Vertex Array Object. This method specifies the location and data format
         * of the buffer containing de data to be rendered
         * @param index specifies the index of the attribute
         * @param size specifies the number of components for the attribute
         * @param pointer specifies the position of the attribute within the buffer for a specific vertex
         * @param stride specifies the amount of total components per vertex
         * @param type this parameter is optional and specifies the type of data for the attribute, the default type used is GL_FLOAT
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
            stride * static_cast<decltype(stride)>(sizeof(float)),          // byte offset between consecutive vertices (the value taken by sizeof should be parametrized, its float right now for simplicity since the default type is GL_FLOAT)
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