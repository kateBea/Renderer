#ifndef BASIC_LIGHTING_HH
#define BASIC_LIGHTING_HH

// C++ Standard Library
#include <vector>
#include <cstdint>

// Third-Party Libraries
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vao.hh"
#include "Vbo.hh"
#include "Common.hh"

namespace kT {
    class BasicLighting {
    public:
        explicit BasicLighting() noexcept;

        explicit BasicLighting(const std::vector<Vertex>& vertices);

        [[nodiscard]]
        auto getId() const -> std::uint32_t;

        [[nodiscard]]
        auto getColor() const -> const glm::vec3&;


        auto load(const std::vector<Vertex>& vertices) -> void;

        auto bind() -> void;

        ~BasicLighting();


    private:
        std::uint32_t m_Id{};
        glm::vec3 m_LightingColor{};

        kT::Vao m_Vao{};
        kT::Vbo m_Vbo{};

    };
}

#endif // BASIC_LIGHTING_HH
