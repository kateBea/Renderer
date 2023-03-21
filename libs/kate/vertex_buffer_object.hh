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
        /**
         * Creates a new vertex buffer and initializes it with the data
         * from indices. If no data is provided simply creates a new index
         * buffer object with a valid id
         * @param vertices buffer containing all the vertices
         * */
        explicit vbo(std::span<float> vertices) noexcept;

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

        /**
         * Releases resources from this vertex buffer
         * */
        ~vbo();

    private:
        std::uint32_t m_id{};
        std::size_t m_size{};
    };

    inline vbo::vbo(std::span<float> vertices) noexcept
        :   m_id{}, m_size{ vertices.size() }
    {
        // size refers to size in bytes of the content in data
        glGenBuffers(1, &this->m_id);

        if (m_size) {
            glBindBuffer(GL_ARRAY_BUFFER, this->m_id);
            glBufferData(GL_ARRAY_BUFFER, m_size, vertices.data(), GL_STATIC_DRAW);
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

    auto vbo::get_count() const -> std::size_t {
        return this->m_size / sizeof (float);
    }

}

#endif	// END VBO_HH