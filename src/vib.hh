/**
 * @file vib.hh
 * @author kate
 * @brief Defines the vertex index buffer class
 * @version 1.0
 * @date 2023-03-21
 */

#ifndef VIB_HH
#define VIB_HH

// C++ Standard Library
#include <vector>
#include <cstddef>

// Third-Party Libraries
#include <GL/glew.h>

namespace kate {
    class vib {
    public:
        explicit vib();
        /**
         * Creates a new vertex index buffer and initializes it with the data
         * from indices. If no data is provided simply creates a new index buffer object with a valid id
         * @param indices buffer containing all the indices values
         * */
        explicit vib(std::vector<std::uint32_t> indices, GLenum usage = GL_STATIC_DRAW);

        /**
         * Mark this vertex index buffer as current
         * */
        auto bind() const -> void;

        /**
         * Copy constructor. Marked as delete to avoid vib aliasing
         * Ensure one vib id is held by one vib object
         * */
        vib(const vib& other) = delete;

        /**
         * Copy assigment. Marked as delete to avoid vib aliasing
         * Ensure one vib id is held by one vib object
         * */
        vib& operator=(const vib& other) = delete;

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

        auto load_data(std::vector<std::uint32_t> indices, GLenum usage = GL_STATIC_DRAW) -> void;

        /**
         * Releases resources from this vertex index buffer
         * */
        ~vib();

    private:
        std::uint32_t m_id{};
        std::size_t m_count{};
    };

    inline vib::vib() {
        glGenBuffers(1, &this->m_id);
    }

    inline vib::vib(std::vector<std::uint32_t> indices, GLenum usage)
        : m_id{}, m_count{}
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, get_id());
        load_data(indices, usage);
    }

    inline auto vib::bind() const -> void {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, get_id());
    }

    inline auto vib::unbind() -> void {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    inline vib::~vib() {
        glDeleteBuffers(1, &this->m_id);
    }

    auto vib::get_id() const -> std::uint32_t {
        return this->m_id;
    }

    auto vib::get_count() const -> std::size_t {
        return this->m_count;
    }

    inline auto vib::load_data(std::vector<std::uint32_t> indices, GLenum usage) -> void {
        if (!indices.empty()) {
            bind();
            m_count = indices.size();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(std::uint32_t), indices.data(), usage);
        }
    }

}

#endif // END VIB_HH