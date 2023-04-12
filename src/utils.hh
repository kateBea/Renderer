/**
 * @file utils.hh
 * @author kate
 * @brief Defines a list of utils
 * @version 1.0
 * @date 2023-04-11
 */

// Third-Party Libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef UTILS_HH
#define UTILS_HH

namespace kate {
    struct vertex {
        glm::vec3 m_pos{};
        glm::vec3 m_norm{};
        glm::vec2 m_tex{};

        vertex() = default;
        ~vertex() = default;

        /**
         * Return reference to the vertex positions
        */
        auto get_positions() -> glm::vec3& { return m_pos; }
        /**
         * Return reference to the vertex positions
        */
        auto get_positions() const -> const glm::vec3& { return m_pos; }

        /**
         * Return reference to the vertex normals
        */
        auto get_normals() -> glm::vec3& { return m_norm; }
        /**
         * Return reference to the vertex normals
        */
        auto get_normals() const -> const glm::vec3& { return m_norm; }

        /**
         * Return reference to the vertex textures
        */
        auto get_textures() -> glm::vec2& { return m_tex; }
        /**
         * Return reference to the vertex textures
        */
        auto get_textures() const -> const glm::vec2& { return m_tex; } 


        /**
         * Set the vertex positions to the given data
        */
        auto set_positions(const glm::vec3& data) -> void { m_pos = data; }
        /**
         * Set the vertex normals to the given data
        */
        auto set_normals(const glm::vec3& data) -> void { m_norm = data; }
        /**
         * Set the vertex textures to the given data
        */
        auto set_textures(const glm::vec3& data) -> void { m_tex = data; }
    };

    enum class texture_t {
        DIFFUSE,
        SPECULAR,
    };
    
}
#endif