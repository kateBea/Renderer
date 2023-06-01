/**
 * @file Shader.hh
 * @author kT
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
#include <cstdint>
#include <filesystem>

// Third-Party Libraries
#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


namespace kT {
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
         Shader(Shader&& other) noexcept;

        /**
         * Move assignment
         * @return *this
         * */
        Shader& operator=(Shader&& other) noexcept;

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
         * it has no effect if this Shader has no uniform with given name. This function
         * ensures this shader is being used before passing the data to the shader uniform, so
         * a previous call to Shader::use() is unnecessary
         * @param name name of the uniform
         * @param value value to be set
         * */
        auto setUniformBool(std::string_view name, bool value) const -> void;

        /**
         * Sets the given integer value to the uniform identified by "name",
         * it has no effect if this Shader has no uniform with given identifier. This function
         * ensures this shader is being used before passing the data to the shader uniform, so
         * a previous call to Shader::use() is unnecessary
         * @param name name of the uniform.
         * @param value value to be set
         * */
        auto setUniformInt(std::string_view name, std::int32_t value) const -> void;

        /**
         * Sets the given floating value to the uniform identified by "name",
         * it has no effect if this Shader has no uniform with given identifier. This function
         * ensures this shader is being used before passing the data to the shader uniform, so
         * a previous call to Shader::use() is unnecessary
         * @param name name of the uniform
         * @param value value to be set
         * */
        auto setUniformFloat(std::string_view name, float value) const -> void;

        /**
         * Sets the given matrix to the uniform matrix specified by the name. This function
         * ensures this shader is being used before passing the data to the shader uniform, so
         * a previous call to Shader::use() is unnecessary
         * @param name name of the uniform
         * @param mat value for the uniform
         * */
        auto setUniformMat4(std::string_view name, const glm::mat4& mat) const -> void;

        /**
         * Sets the given 3D vector to the uniform specified by the name. This function
         * ensures this shader is being used before passing the data to the shader uniform, so
         * a previous call to Shader::use() is unnecessary
         * @param name name of the uniform
         * @param vec value for the uniform
         * */
        auto setUniformVec3(std::string_view name, const glm::vec3& vec) const -> void;

        /**
         * Sets the given 4D vector to the uniform specified by the name. This function
         * ensures this shader is being used before passing the data to the shader uniform, so
         * a previous call to Shader::use() is unnecessary
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
         * Returns an error message indicating the type of shader
         * This is a helper function for showing compilation status on Shader::compile()
         * @param type type of shader
         * */
        static constexpr auto getShaderErrorStr(GLenum type) -> std::string_view;
        /**
         * Compiles the given shader and returns its corresponding identifier
         * @param content file contents of the shader
         * @param shaderType type of shader to be compiled
         * @return identifier of the compiled shader, 0 if there was an error
         * */
        static auto compile(const char* content, GLenum shaderType) -> std::uint32_t;
        /**
         * Compiles and links the given shaders to this program shader.
         * @param vShader file contents of the vertex shader
         * @param fShader file contents of the fragment shader
         * */
        auto build(const char* vShader, const char* fShader) const -> void;

        /**
         * Helper function to retrieve Shader status
         * @param objectId identifier of the shader object
         * @param str error message indicating the type of shader
         * @param status
         * */
        static auto showShaderStatus(std::uint32_t objectId, std::string_view str, GLenum status) -> void;

        /**
         * Helper function to retrieve program status
         * @param objectId identifier of the program object
         * @param str error message indicating the type of shader
         * @param status
         * */
        static auto showProgramStatus(std::uint32_t objectId, GLenum status) -> void;

        /**
         * Identifier of this Shader program
         * */
        std::uint32_t m_Id{};
    };
}

#endif // END SHADER_HH