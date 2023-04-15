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
         * Ensure one Shader id is held by one Shader object
         * */
        Shader(const Shader& other) = delete;

        /**
         * Copy assigment. Marked as delete to avoid Shader aliasing
         * Ensure one Shader id is held by one Shader object
         * */
        Shader& operator=(const Shader& other) = delete;

        /**
         * Move constructor
         * */
         Shader(Shader&& other) noexcept
            : m_id{other.m_id }
        {
            // when other is a temporary 0 is ignored if
            // passed to glDeleteProgram(). We avoid deleting a valid program this way
            other.m_id = 0;
        }

        /**
         * Move constructor assignment
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
         * Perform cleanup
         * */
        ~Shader();

    private:
        /**
         * Compiles and links the given shaders to this program shader.
         * @param vShader file contents of the vertex shader
         * @param fShader file contents of the fragment shader
         * */
        static auto build(const char* vShader, const char* fShader, std::uint32_t id) -> void;
        /*
         * Helper function to retrieve Shader compilation/linking status
         * */
        static auto showShaderStatus(std::uint32_t objectId, std::string_view str, GLenum name) -> void{
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

        /*
         * Helper function to retrieve Shader compilation/linking status
         * */
        static auto showProgramStatus(std::uint32_t objectId, std::string_view str, GLenum name) -> void{
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

        const char* vShaderContents{vTemp.c_str() };
        const char* fShaderContents{fTemp.c_str() };

        build(vShaderContents, fShaderContents, getProgram());
    }

    auto Shader::build(const char* vShader, const char* fShader, std::uint32_t id) -> void {
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
        glAttachShader(id, vertexShaderID);
        glAttachShader(id, pixelShaderID);
        glLinkProgram(id);
        showProgramStatus(id, "Error on program linking: ", GL_LINK_STATUS);

        glDetachShader(id, vertexShaderID);
        glDetachShader(id, pixelShaderID);

        // cleanup
        glDeleteShader(vertexShaderID);
        glDeleteShader(pixelShaderID);
    }

}

#endif // END SHADER_HH