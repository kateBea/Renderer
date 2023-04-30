#include "../include/BasicLighting.hh"

namespace Kate {

    BasicLighting::BasicLighting() noexcept
        :   m_Id{}, m_LightingColor{ glm::vec3(1.0f, 1.0f, 1.0f) }, m_Vao{}, m_Vbo{}
    {
        glGenBuffers(1, &this->m_Id);
    }

    BasicLighting::BasicLighting(const std::vector<float> &vertexPositions)
        :   m_Id{}, m_LightingColor{ glm::vec3(1.0f, 1.0f, 1.0f) }, m_Vao{}, m_Vbo{}
    {
        glGenBuffers(1, &this->m_Id);
        load(vertexPositions);
    }

    auto BasicLighting::getId() const -> std::uint32_t {
        return m_Id;
    }

    auto BasicLighting::load(const std::vector<float> &vertexPositions) -> void {
        m_Vbo.load(vertexPositions);
        m_Vao.layout(0, 3, 0, 3);

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
