#ifndef VERTEX_BUFFER_OBJECT_CLASS_HH
#define VERTEX_BUFFER_OBJECT_CLASS_HH

#include <cstdint>

#include <GL/glew.h>

namespace Kate {
    class VertexBufferObject {
    public:
        VertexBufferObject(const float *data, std::size_t size);

        auto bind() const -> void;

        static auto unbind() -> void;

        ~VertexBufferObject();

    private:
        std::uint32_t m_id{};
    };

    inline VertexBufferObject::VertexBufferObject(const float *data, std::size_t size) {
        // size refers to size in bytes of the content in data
        glGenBuffers(1, &this->m_id);
        glBindBuffer(GL_ARRAY_BUFFER, this->m_id);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    inline auto VertexBufferObject::bind() const -> void {
        glBindBuffer(GL_ARRAY_BUFFER, this->m_id);
    }

    inline auto VertexBufferObject::unbind() -> void {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    inline VertexBufferObject::~VertexBufferObject() {
        glDeleteBuffers(1, &this->m_id);
    } // END CLASS VERTEX_BUFFER_OBJECT

} // END NAMESPACE KATE

#endif	// END VERTEX_BUFFER_OBJECT_CLASS_HH