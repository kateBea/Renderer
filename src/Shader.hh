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

// Third-Party Libraries
#include <GL/glew.h>

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

    inline Shader::Shader(const std::filesystem::path &vertexSourceDir, const std::filesystem::path &fragmentSourceDir) {
        load(vertexSourceDir, fragmentSourceDir);
    }

    inline void Shader::use() const {
        glUseProgram(this->m_id);
    }

    inline std::uint32_t Shader::getProgram() const {
        return this->m_id;
    }

    inline Shader::~Shader() {
        glDeleteProgram(this->m_id);
    }

    inline Shader::Shader() {
        m_id = glCreateProgram();

        if (!m_id)
            std::cerr << "Error when creating shader program\n";
    }

    inline auto Shader::load(const std::filesystem::path& vShaderPath, const std::filesystem::path& fShaderPath) const -> void {
        std::string vTemp{};
        std::string fTemp{};

        std::ifstream vertexShaderFile{ vShaderPath };
        std::ifstream pixelShaderFile{ fShaderPath };

        if (!vertexShaderFile.is_open())
            throw std::runtime_error("could not open vertex Shader file...");

        if (!pixelShaderFile.is_open())
            throw std::runtime_error("could not open fragment Shader file...");

        std::ostringstream vertexShaderStream{};
        std::ostringstream pixelShaderStream{};

        // read file buffer into the streams
        vertexShaderStream << vertexShaderFile.rdbuf();
        pixelShaderStream << pixelShaderFile.rdbuf();

        // close files
        vertexShaderFile.close();
        pixelShaderFile.close();

        // convert streams buffer contents into strings
        vTemp = vertexShaderStream.str();
        fTemp = pixelShaderStream.str();

        const char* vShaderContents{ vTemp.c_str() };
        const char* fShaderContents{ fTemp.c_str() };

        build(vShaderContents, fShaderContents);
    }

    auto Shader::build(const char* vShader, const char* fShader) const -> void {
        // Load and compile Vertex Shader
        std::uint32_t vertexShaderID{};
        vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShaderID, 1, &vShader, nullptr);
        glCompileShader(vertexShaderID);
        showShaderStatus(vertexShaderID, "Error on Vertex Shader compilation: ", GL_COMPILE_STATUS);

        // Load and compile the fragment Shader
        std::uint32_t pixelShaderID{};
        pixelShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(pixelShaderID, 1, &fShader, nullptr);
        glCompileShader(pixelShaderID);
        showShaderStatus(pixelShaderID, "Error on fragment Shader compilation: ", GL_COMPILE_STATUS);

        // Create and link program against compiled Shader binaries
        glAttachShader(getProgram(), vertexShaderID);
        glAttachShader(getProgram(), pixelShaderID);
        glLinkProgram(getProgram());
        showProgramStatus(getProgram(), "Error on program linking: ", GL_LINK_STATUS);

        glDetachShader(getProgram(), vertexShaderID);
        glDetachShader(getProgram(), pixelShaderID);

        // cleanup
        glDeleteShader(vertexShaderID);
        glDeleteShader(pixelShaderID);
    }

    auto Shader::setUniformBool(std::string_view name, bool value) const -> void {
        use();
        auto ret{ glGetUniformLocation(getProgram(), name.data()) };

        if (ret == -1)
            std::cerr << "Not a valid uniform name for this program shader\n";
        else
            glUniform1i(ret, static_cast<std::int32_t>(value));
    }

    auto Shader::setUniformInt(std::string_view name, std::int32_t value) const -> void {
        use();
        auto ret{ glGetUniformLocation(getProgram(), name.data()) };

        if (ret == -1)
            std::cerr << "Not a valid uniform name for this program shader\n";
        else
            glUniform1i(ret, value);
    }

    auto Shader::setUniformFloat(std::string_view name, float value) const -> void {
        use();
        auto ret{ glGetUniformLocation(getProgram(), name.data()) };

        if (ret == -1)
            std::cerr << "Not a valid uniform name for this program shader\n";
        else
            glUniform1f(ret, value);
    }

    inline auto Shader::showShaderStatus(std::uint32_t objectId, std::string_view str, GLenum name) -> void {
        std::array<char, 512> outStr{};
        std::int32_t success{};
        std::int32_t length{};

        glGetShaderiv(objectId, name, &success);
        glGetShaderiv(objectId, GL_INFO_LOG_LENGTH, &length);
        if (length > 0) {
            glGetShaderInfoLog(objectId, outStr.size(), nullptr, outStr.data());
            std::printf("%s: %s\n", str.data(), outStr.data());
        }
    }

    inline auto Shader::showProgramStatus(std::uint32_t objectId, std::string_view str, GLenum name) -> void {
        std::array<char, 1024> outStr{};
        std::int32_t success{};
        std::int32_t length{};

        glGetProgramiv(objectId, name, &success);
        glGetProgramiv(objectId, GL_INFO_LOG_LENGTH, &length);
        if (length > 0) {
            glGetProgramInfoLog(objectId, outStr.size(), nullptr, outStr.data());
            std::printf("%s: %s\n", str.data(), outStr.data());
        }
    }

    auto Shader::setUniformMat4(std::string_view name, const glm::mat4& mat) const -> void {
        use();
        auto ret{ glGetUniformLocation(getProgram(), name.data()) };

        if (ret == -1)
            std::cerr << "Not a valid uniform name for this program shader\n";
        else
            glUniformMatrix4fv(ret, 1, GL_FALSE, glm::value_ptr(mat));
    }
}

#endif // END SHADER_HH