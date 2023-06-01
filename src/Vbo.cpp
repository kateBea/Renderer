#include "OpenGL/Vbo.hh"

namespace kT {
    Vbo::Vbo(const std::vector<kT::Vertex>& vertices, GLenum usage) noexcept
        :   m_Id{}, m_Size{}
    {
        glGenBuffers(1, &this->m_Id);
        load(vertices, usage);
    }

    auto Vbo::bind() const -> void {
        glBindBuffer(GL_ARRAY_BUFFER, this->m_Id);
    }

    auto Vbo::unbind() -> void {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    Vbo::~Vbo() {
        glDeleteBuffers(1, &this->m_Id);
    }

    auto Vbo::getId() const -> std::uint32_t {
        return this->m_Id;
    }

    auto Vbo::getSize() const -> std::size_t {
        return this->m_Size;
    }

    auto Vbo::getCount() const -> std::size_t {
        return this->m_Size / sizeof (float);
    }

    Vbo::Vbo(Vbo&& other) noexcept
            : m_Id{ other.getId() }, m_Size{ other.getSize() }
    {
        other.m_Id = 0;
        other.m_Size = 0;
    }

    Vbo& Vbo::operator=(Vbo&& other) noexcept {
        this->m_Id = other.getId();
        this->m_Size = other.getSize();

        other.m_Id = 0;
        other.m_Size = 0;

        return *this;
    }

    Vbo::Vbo() {
        glGenBuffers(1, &this->m_Id);
    }

    auto Vbo::load(const std::vector<kT::Vertex>& vertices, GLenum usage) -> void {
        if (!vertices.empty()) {
            bind();
            m_Size = vertices.size() * sizeof(kT::Vertex);
            glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_Size), vertices.data(), usage);
            unbind();
        }
    }

}