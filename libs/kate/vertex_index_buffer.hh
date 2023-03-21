#ifndef VIO_HH
#define VIO_HH

// C++ Standard Library
#include <span>
#include <cstddef>

// Third-Party Libraries
#include <GL/glew.h>

namespace kate {
    class vio {
    public:
        /**
         * Creates a new vertex index buffer and initializes it with the data
         * from indices. If no data is provided simply creates a new index buffer object with a valid id
         * @param indices buffer containing all the indices values
         * */
        explicit vio(std::span<std::uint32_t> indices);

        /**
         * Mark this vertex index buffer as current
         * */
        auto bind() const -> void;

        /**
         * Copy constructor. Marked as delete to avoid vio aliasing
         * Ensure one vio id is held by one vio object
         * */
        vio(const vio& other) = delete;

        /**
         * Copy assigment. Marked as delete to avoid vio aliasing
         * Ensure one vio id is held by one vio object
         * */
        vio& operator=(const vio& other) = delete;

        /**
         * Returns the total count of indices of this vertex index buffer
         * @return count of indices
         * */
        [[nodiscard]]
        auto get_count() const -> std::size_t;

        /**
         * Returns the identifier of this vertex index buffer
         * @return id of this object
         * */
        [[nodiscard]]
        auto get_id() const -> std::uint32_t;

        /**
         * Releases the currently bound vertex index object
         * */
        static auto unbind() -> void;

        /**
         * Releases resources from this vertex index buffer
         * */
        ~vio();

    private:
        std::uint32_t m_id{};
        std::size_t m_count{};
    };

    inline vio::vio(std::span<std::uint32_t> indices)
        : m_id{}, m_count{ indices.size() }
    {
        glGenBuffers(1, &this->m_id);

        if (m_count) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_id);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(std::uint32_t), indices.data(), GL_STATIC_DRAW);
        }
    }

    inline auto vio::bind() const -> void {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_id);
    }

    inline auto vio::unbind() -> void {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    inline vio::~vio() {
        glDeleteBuffers(1, &this->m_id);
    }

    auto vio::get_id() const -> std::uint32_t {
        return this->m_id;
    }

    auto vio::get_count() const -> std::size_t {
        return this->m_count;
    }

}

#endif // END VIO_HH