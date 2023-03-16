#ifndef VERTEX_INDEX_OBJECT_CLASS_HH 
#define VERTEX_INDEX_OBJECT_CLASS_HH

#include <cstdint>
#include <GL/glew.h>

namespace Kate {
    class VertexIndexBuffer {
    public:
        VertexIndexBuffer(const std::uint32_t *data, std::size_t count);

        auto                bind() const -> void;

        static auto         unbind() -> void;

        [[nodiscard]] auto  getCount() const -> std::size_t;

        ~VertexIndexBuffer();

    private:
        std::uint32_t m_id{};
        std::size_t m_count{};
    };

    inline VertexIndexBuffer::VertexIndexBuffer(const std::uint32_t *data, std::size_t count)
            : m_id{}, m_count{count}
    {
        // count refers to the amount of elements (and not bytes) inside "data"
        glGenBuffers(1, &this->m_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(std::uint32_t), data, GL_STATIC_DRAW);
    }

    inline auto VertexIndexBuffer::bind() const -> void {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_id);
    }

    inline auto VertexIndexBuffer::unbind() -> void {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    inline VertexIndexBuffer::~VertexIndexBuffer() {
        glDeleteBuffers(1, &this->m_id);
    }

    inline auto VertexIndexBuffer::getCount() const -> std::size_t {
        return this->m_count;
    }

} // END NAMESPACE KATE

#endif // END VERTEX_INDEX_OBJECT_CLASS_HH