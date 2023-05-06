#include "../include/BasicLighting.hh"

namespace kT {

    BasicLighting::BasicLighting() noexcept
        :   m_Id{}, m_LightingColor{ glm::vec3(1.0f, 1.0f, 1.0f) }, m_Vao{}, m_Vbo{}
    {
        glGenBuffers(1, &this->m_Id);
    }

    BasicLighting::BasicLighting(const std::vector<Vertex>& vertices)
        :   m_Id{}, m_LightingColor{ glm::vec3(1.0f, 1.0f, 1.0f) }, m_Vao{}, m_Vbo{}
    {
        glGenBuffers(1, &this->m_Id);
        load(vertices);
    }

    auto BasicLighting::getId() const -> std::uint32_t {
        return m_Id;
    }

    auto BasicLighting::load(const std::vector<Vertex> &vertexPositions) -> void {
        m_Vbo.load(vertexPositions);
        m_Vao.layout(0, 3, Vao::Attribute::POSITION);

    }

    BasicLighting::~BasicLighting() {
        glDeleteBuffers(1, &this->m_Id);
    }

    auto BasicLighting::getColor() const -> const glm::vec3 & {
        return m_LightingColor;
    }

    auto BasicLighting::bind() -> void {
        m_Vbo.bind();
        m_Vao.bind();
    }

}
