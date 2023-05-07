/**
 * @file Vao.hh
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
#include <GL/glew.h>

// Project Libraries
#include "Common.hh"

namespace kT {
    class Vao {
    public:
        enum class Attribute {
            POSITION,
            NORMAL,
            TEXTURE,
            NONE,
        };

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
         * @param other other Vao from which we move data
         * */
        Vao(Vao&& other) noexcept;

        /**
         * Move constructor assignment
         * @param other other Vao from which we move data
         * @retunr *this
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
         * @param stride specifies the total count of components per vertex
         * @param type this parameter is optional and specifies the type of data for the attribute, the default type used is GL_FLOAT
         * */
        auto layout(std::uint32_t index, std::int32_t size, Attribute pointer, GLenum type = GL_FLOAT) const -> void;

        ~Vao();

    private:
        /**
         * Returns the offset of the given attribute type in the kT::Vertex structure
         * @param attribute vertex attribute
         * @return byte offset from this attribute within the kT::Vertex structure
         * */
        static auto getAttributeOffset(Attribute attribute) -> std::int32_t;
        std::uint32_t m_Id{};
    };
}

#endif	// END VAO_HH