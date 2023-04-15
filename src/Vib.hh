/**
 * @file Vib.hh
 * @author Kate
 * @brief Defines the Vertex index buffer class
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

namespace Kate {
    class Vib {
    public:
        explicit Vib();
        /**
         * Creates a new Vertex index buffer and initializes it with the data
         * from indices. If no data is provided simply creates a new index buffer object with a valid id
         * @param indices buffer containing all the indices values
         * */
        explicit Vib(std::vector<std::uint32_t> indices, GLenum usage = GL_STATIC_DRAW);

        /**
         * Mark this Vertex index buffer as current
         * */
        auto bind() const -> void;

        /**
         * Copy constructor. Marked as delete to avoid Vib aliasing
         * Ensure one Vib id is held by one Vib object
         * */
        Vib(const Vib& other) = delete;

        /**
         * Copy assigment. Marked as delete to avoid Vib aliasing
         * Ensure one Vib id is held by one Vib object
         * */
        Vib& operator=(const Vib& other) = delete;

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

        auto load(std::vector<std::uint32_t> indices, GLenum usage = GL_STATIC_DRAW) -> void;

        /**
         * Releases resources from this Vertex index buffer
         * */
        ~Vib();

    private:
        std::uint32_t m_id{};
        std::size_t m_count{};
    };

    inline Vib::Vib() {
        glGenBuffers(1, &this->m_id);
    }

    inline Vib::Vib(std::vector<std::uint32_t> indices, GLenum usage)
        : m_id{}, m_count{}
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getId());
        load(indices, usage);
    }

    inline auto Vib::bind() const -> void {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getId());
    }

    inline auto Vib::unbind() -> void {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    inline Vib::~Vib() {
        glDeleteBuffers(1, &this->m_id);
    }

    auto Vib::getId() const -> std::uint32_t {
        return this->m_id;
    }

    auto Vib::getCount() const -> std::size_t {
        return this->m_count;
    }

    inline auto Vib::load(std::vector<std::uint32_t> indices, GLenum usage) -> void {
        if (!indices.empty()) {
            bind();
            m_count = indices.size();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(std::uint32_t), indices.data(), usage);
        }
    }

}

#endif // END VIB_HH