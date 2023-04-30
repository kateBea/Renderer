/**
 * @file utils.hh
 * @author Kate
 * @brief Defines a list of utils
 * @version 1.0
 * @date 2023-04-11
 */

// C++ Standard Library
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

// Third-Party Libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef UTILS_HH
#define UTILS_HH

namespace Kate {
    // CONSTANTS
    // Define the OpenGL version as x.y where x is
    // g_GLMajor and y is g_GLMinor, e.g.: for g_GLMajor = 4
    // and g_GLMinor = 1 we have OpenGL 4.1 this can be used to initialize
    // the GLFW library setting the OpenGL minor and major hints
    constexpr std::int32_t GLMajor{ 4 };
    constexpr std::int32_t GLMinor{ 1 };

    // CLASS UTILITIES
    struct Vertex {
        glm::vec3 m_Pos{};
        glm::vec3 m_Norm{};
        glm::vec2 m_Texture{};

        Vertex() = default;
        ~Vertex() = default;

        /**
         * Return reference to the Vertex positions
        */
        auto getPositions() -> glm::vec3& { return m_Pos; }

        /**
         * Return reference to the Vertex positions
        */
        [[nodiscard]]
        auto getPositions() const -> const glm::vec3& { return m_Pos; }

        /**
         * Return reference to the Vertex normals
        */
        [[nodiscard]]
        auto getNormals() -> glm::vec3& { return m_Norm; }

        /**
         * Return reference to the Vertex normals
        */
        [[nodiscard]]
        auto getNormals() const -> const glm::vec3& { return m_Norm; }

        /**
         * Return reference to the Vertex textures
        */
        [[nodiscard]]
        auto getTextures() -> glm::vec2& { return m_Texture; }

        /**
         * Return reference to the Vertex textures
        */
        [[nodiscard]]
        auto getTextures() const -> const glm::vec2& { return m_Texture; }


        /**
         * Set the Vertex positions to the given data
         * @param data new vertex positions for this Vertex
        */
        auto setPositions(const glm::vec3& data) -> void { m_Pos = data; }
        /**
         * Set the Vertex normals to the given data
         *
        */
        auto setNormals(const glm::vec3& data) -> void { m_Norm = data; }
        /**
         * Set the Vertex textures to the given data
        */
        auto setTextures(const glm::vec3& data) -> void { m_Texture = data; }
    };

    // FUNCTION UTILITIES

    /**
     * Parses a file containing vertices with very basic formatting
     * @see <a href="https://github.com/kateBea/Renderer/tree/main/assets/vertices">vertices file</a>
     * */
    inline auto parseVerticesFile(const std::filesystem::path& path) -> std::vector<float> {

        // parses files with vertices in the format:
        /**
         *    //   POSITIONS        TEXTURES
         *   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         *    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         *    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         *    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         *   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         *   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         *   ...
         * */

        // This function will only return the values as floats contained with in
        // a vector. User must ensure to specify buffer layout with appropriate function calls
        std::string temp{};
        std::ifstream file{ path };
        std::vector<float> result{};

        if (file.is_open()) {
            while (std::getline(file, temp)) {
                if (!temp.starts_with("//")) {
                    std::istringstream stream{ temp };

                    while(std::getline(stream, temp, ','))
                        result.push_back(std::stof(temp));
                }
            }
        }
        else
            std::cerr << "Could not open file containing vertices data...\n";

        return result;
    }
    
}
#endif