/**
 * @file Shader.hh
 * @author Kate
 * @brief Defines the Shader interface
 * @version 1.0
 * @date 2023-03-21
 */

#ifndef SHADER_HH
#define SHADER_HH

// C++ Standard Library
#include <string>
#include <array>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string_view>
#include <cstddef>
#include <filesystem>

// Third-Party Libraries
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Kate {
    class Shader {
    public:
        /**
         * Default initialization for Shader. Creates a valid shader program
         * */
        explicit Shader();

        /**
         * Copy constructor. Marked as delete to avoid Shader aliasing
         * Ensure one Shader resources are held by single Shader object
         * */
        Shader(const Shader& other) = delete;

        /**
         * Copy assigment. Marked as delete to avoid Shader aliasing
         * Ensure one Shader resources are held by single Shader object
         * */
        Shader& operator=(const Shader& other) = delete;

        /**
         * Move constructor
         * */
         Shader(Shader&& other) noexcept
            : m_id{ other.getProgram() }
        {
            // assign 0 so that it can be safely passed to glDeleteProgram().
            // We avoid deleting a valid program this way
            other.m_id = 0;
        }

        /**
         * Move assignment
         * */
        Shader& operator=(Shader&& other) noexcept {
            m_id = other.m_id;
            other.m_id = 0;

            return *this;
        }

        /**
         * Construct Shader program from path to Vertex Shader source file directory
         * and pixel Shader source file directory
         * @param vertexSourceDir directory to the Vertex Shader source file
         * @param fragmentSourceDir directory to the pixel Shader source file
         * */
        Shader(const std::filesystem::path& vertexSourceDir, const std::filesystem::path& fragmentSourceDir);

        /**
         * Use this Shader program
         * */
        auto use() const -> void;

        /**
         * Get Shader program ID
         * @return Shader program ID
         * */
        [[nodiscard]]
        auto getProgram() const -> std::uint32_t;

        /**
         * Loads the shaders specified from paths
         * @param vShaderPath path to vertex shader path
         * @param fShaderPath path to pixel/fragment shader path
         * @throws std::runtime_error exception if any of the shader files could not be opened
         * */
        auto load(const std::filesystem::path& vShaderPath, const std::filesystem::path& fShaderPath) const -> void;

        /**
         * Sets the given boolean value to the uniform identified by "name",
         * it has no effect if this Shader has no uniform with given identifier
         * @param name name of the uniform
         * @param value value to be set
         * */
        auto setUniformBool(std::string_view name, bool value) const -> void;

        /**
         * Sets the given integer value to the uniform identified by "name",
         * it has no effect if this Shader has no uniform with given identifier
         * @param name name of the uniform
         * @param value value to be set
         * */
        auto setUniformInt(std::string_view name, std::int32_t value) const -> void;

        /**
         * Sets the given floating value to the uniform identified by "name",
         * it has no effect if this Shader has no uniform with given identifier
         * @param name name of the uniform
         * @param value value to be set
         * */
        auto setUniformFloat(std::string_view name, float value) const -> void;

        /**
         * Sets the given matrix to the uniform matrix specified by the name
         * @param name name of the uniform
         * @param mat value for the uniform
         * */
        auto setUniformMat4(std::string_view name, const glm::mat4& mat) const -> void;

        /**
         * Sets the given 3D vector to the uniform specified by the name
         * @param name name of the uniform
         * @param vec value for the uniform
         * */
        auto setUniformVec3(std::string_view name, const glm::vec3& vec) const -> void;

        /**
         * Sets the given 4D vector to the uniform specified by the name
         * @param name name of the uniform
         * @param vec value for the uniform
         * */
        auto setUniformVec4(std::string_view name, const glm::vec4& vec) const -> void;

        /**
         * Perform cleanup
         * */
        ~Shader();

    private:
        /**
         * Compiles and links the given shaders to this program shader.
         * @param vShader file contents of the vertex shader
         * @param fShader file contents of the fragment shader
         * */
        auto build(const char* vShader, const char* fShader) const -> void;

        /**
         * Helper function to retrieve Shader compilation/linking status
         * */
        static auto showShaderStatus(std::uint32_t objectId, std::string_view str, GLenum name) -> void;

        /**
         * Helper function to retrieve Shader compilation/linking status
         * */
        static auto showProgramStatus(std::uint32_t objectId, std::string_view str, GLenum name) -> void;

        /**
         * Identifier of this Shader program
         * */
        std::uint32_t m_id{};
    };
}

#endif // END SHADER_HH