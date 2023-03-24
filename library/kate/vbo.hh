/**
 * @file vbo.hh
 * @author kate
 * @brief Defines the vertex buffer object class
 * @version 1.0
 * @date 2023-03-21
 */


#ifndef VBO_HH
#define VBO_HH

// C++ Standard Library
#include <span>
#include <cstddef>

// Third-Party Libraries
#include <GL/glew.h>

namespace kate {
    class vbo {
    public:
        explicit vbo();
        /**
         * Creates a new vertex buffer and initializes it with the data
         * from indices. If no data is provided simply creates a new index
         * buffer object with a valid id
         * @param vertices buffer containing all the vertices
         * */
        explicit vbo(std::span<float> vertices, GLenum usage = GL_STATIC_DRAW) noexcept;

        /**
         * Copy constructor. Marked as delete to avoid vbo aliasing
         * Ensure one vbo id is held by one vbo object
         * */
        vbo(const vbo& other) = delete;

        /**
         * Copy assigment. Marked as delete to avoid vbo aliasing
         * Ensure one vbo id is held by one vbo object
         * */
        vbo& operator=(const vbo& other) = delete;

        /**
         * Move constructor
         * */
        vbo(vbo&& other) noexcept;

        /**
         * Move constructor assignment
         * */
        vbo& operator=(vbo&& other) noexcept;

        /**
         * Mark this vertex buffer as current
         * */
        auto bind() const -> void;

        /**
         * Returns the identifier of this vertex buffer
         * @return id of this object
         * */
        [[nodiscard]]
        auto get_id() const -> std::uint32_t;

        /**
         * Returns the total size in bytes of the contents of this vertex buffer
         * @return total size in bytes of the vertices of this buffer
         * */
        [[nodiscard]]
        auto get_size() const -> std::size_t;

        /**
         * Returns the total count of vertices of this vertex buffer
         * @return count of vertices
         * */
        [[nodiscard]]
        auto get_count() const -> std::size_t;

        /**
         * Releases the currently bound vertex buffer object
         * */
        static auto unbind() -> void;

        auto load_data(std::span<float> vertices, GLenum usage = GL_STATIC_DRAW) -> void;
        /**
         * Releases resources from this vertex buffer
         * */
        ~vbo();

    private:
        std::uint32_t m_id{};   // Identifier of this vertex buffer object
        std::size_t m_size{};   // size in BYTES of the total count of vertices
    };

    inline vbo::vbo(std::span<float> vertices, GLenum usage) noexcept
        :   m_id{}, m_size{ vertices.size() * sizeof(decltype(vertices)::value_type) }
    {
        if (m_size) {
            glBindBuffer(GL_ARRAY_BUFFER, this->m_id);
            glBufferData(GL_ARRAY_BUFFER, m_size, vertices.data(), usage);
        }
    }

    inline auto vbo::bind() const -> void {
        glBindBuffer(GL_ARRAY_BUFFER, this->m_id);
    }

    inline auto vbo::unbind() -> void {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    inline vbo::~vbo() {
        glDeleteBuffers(1, &this->m_id);
    }

    inline auto vbo::get_id() const -> std::uint32_t {
        return this->m_id;
    }

    inline auto vbo::get_size() const -> std::size_t {
        return this->m_size;
    }

    inline auto vbo::get_count() const -> std::size_t {
        return this->m_size / sizeof (float);
    }

    inline vbo::vbo(vbo&& other) noexcept
        :   m_id{ other.get_id() }, m_size{ other.get_size() }
    {
        other.m_id = 0;
        other.m_size = 0;
    }

    inline vbo& vbo::operator=(vbo&& other) noexcept {
        this->m_id = other.get_id();
        this->m_size = other.get_size();

        other.m_id = 0;
        other.m_size = 0;

        return *this;
    }

    inline vbo::vbo() {
        glGenBuffers(1, &this->m_id);
    }

    inline auto vbo::load_data(std::span<float> vertices, GLenum usage) -> void {

        if (!vertices.empty()) {
            m_size = vertices.size() * sizeof(decltype(vertices)::value_type);
            glBindBuffer(GL_ARRAY_BUFFER, this->m_id);
            glBufferData(GL_ARRAY_BUFFER, m_size, vertices.data(), usage);
        }
    }

}

#endif	// END VBO_HH