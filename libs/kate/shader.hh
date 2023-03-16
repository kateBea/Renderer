#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string_view>
#include <cstdint>

#include <GL/glew.h>

namespace Kate {
    class Shader {
    public:
        /**
         * Default initialization for Shader
         * */
        Shader() = default;

        /**
         * Copy constructor. Marked as delete to shader aliasing
         * */
        Shader(const Shader& other) = delete;

        /**
         * Move constructor
         * */
         Shader(Shader&& other) noexcept
            :   m_ProgramId{ other.m_ProgramId }
        {
            // when other is a temporary 0 is ignored if
            // passed to glDeleteProgram(). We avoid deleting a valid program this way
            other.m_ProgramId = 0;
        }

        /**
         * Move constructor assignment
         * */
        Shader& operator=(Shader&& other) noexcept {
            m_ProgramId = other.m_ProgramId;
            other.m_ProgramId = 0;

            return *this;
        }

        /**
         * Construct shader program from path to vertex shader source file directory
         * and pixel shader source file directory
         * @param vertexSourceDir directory to the vertex shader source file
         * @param fragmentSourceDir directory to the pixel shader source file
         * */
        Shader(std::string_view vertexSourceDir, std::string_view fragmentSourceDir);


        /**
         * Use this shader program
         * */
        auto use() const -> void;

        /**
         * Get shader program ID
         * @return Shader program ID
         * */
        [[nodiscard]]
        auto getProgramId() const -> std::uint32_t;

        /**
         * Perform cleanup
         * */
        ~Shader();

    private:
        /*
         * Helper function to retrieve shader compilation/linking status
         * */
        static auto showStatusShader(std::uint32_t objectId, const char* str, GLenum name) -> void{
            std::array<char, 512> outStr{};
            std::int32_t success{};
            std::int32_t length{};

            glGetShaderiv(objectId, name, &success);
            glGetShaderiv(objectId, GL_INFO_LOG_LENGTH, &length);
            if (length > 0) {
                glGetShaderInfoLog(objectId, outStr.size(), nullptr, outStr.data());
                std::printf("%s: %s\n", str, outStr.data());
            }
        }

        /*
         * Helper function to retrieve shader compilation/linking status
         * */
        static auto showStatusProgram(std::uint32_t objectId, const char* str, GLenum name) -> void{
            std::array<char, 1024> outStr{};
            std::int32_t success{};
            std::int32_t length{};

            glGetProgramiv(objectId, GL_LINK_STATUS, &success);
            glGetProgramiv(objectId, GL_INFO_LOG_LENGTH, &length);
            if (length > 0) {
                glGetProgramInfoLog(objectId, outStr.size(), nullptr, outStr.data());
                std::printf("%s: %s\n", str, outStr.data());
            }
        }

        /**
         * Identifier of this shader program
         * */
        std::uint32_t m_ProgramId{};
    };

    inline Shader::Shader(std::string_view vertexSourceDir, std::string_view fragmentSourceDir) {
        std::string vertexShaderSource{};
        std::string pixelShaderSource{};

        std::ifstream vertexShaderFile{};
        std::ifstream pixelShaderFile{};

        std::ostringstream vertexShaderStream{};
        std::ostringstream pixelShaderStream{};

        try {
            // open vertex shader and fragment shader source files
            vertexShaderFile.open(vertexSourceDir.data());
            pixelShaderFile.open(fragmentSourceDir.data());

            // read file buffer into the streams
            vertexShaderStream << vertexShaderFile.rdbuf();
            pixelShaderStream << pixelShaderFile.rdbuf();

            // close files
            vertexShaderFile.close();
            pixelShaderFile.close();

            // convert streams buffer contents into strings
            vertexShaderSource = vertexShaderStream.str();
            pixelShaderSource = pixelShaderStream.str();
        }
        catch (const std::exception& e) {
            std::cerr << "Failed to read from files...\n";
            std::cerr << e.what() << '\n';
        }

        const char* vertexShaderCstring[]{ vertexShaderSource.c_str() };
        const char* pixelShaderCstring[]{ pixelShaderSource.c_str() };

        // Load and compile vertex shader
        std::uint32_t vertexShaderID{};
        vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShaderID, 1, vertexShaderCstring, nullptr);
        glCompileShader(vertexShaderID);
        showStatusShader(vertexShaderID, "Error on vertex shader compilation: ", GL_COMPILE_STATUS);

        // Load and compile the fragment shader
        std::uint32_t pixelShaderID{};
        pixelShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(pixelShaderID, 1, pixelShaderCstring, nullptr);
        glCompileShader(pixelShaderID);
        showStatusShader(pixelShaderID, "Error on fragment shader compilation: ", GL_COMPILE_STATUS);

        // Create and link program against compiled shader binaries
        this->m_ProgramId = glCreateProgram();
        glAttachShader(this->m_ProgramId, vertexShaderID);
        glAttachShader(this->m_ProgramId, pixelShaderID);
        glLinkProgram(this->m_ProgramId);
        showStatusProgram(this->m_ProgramId, "Error on program linking: ", GL_LINK_STATUS);

        glDetachShader(this->m_ProgramId, vertexShaderID);
        glDetachShader(this->m_ProgramId, pixelShaderID);

        // cleanup
        glDeleteShader(vertexShaderID);
        glDeleteShader(pixelShaderID);
    }

    inline void Shader::use() const {
        glUseProgram(this->m_ProgramId);
    }

    inline std::uint32_t Shader::getProgramId() const {
        return this->m_ProgramId;
    }

    inline Shader::~Shader() {
        glDeleteProgram(this->m_ProgramId);
    }

} // END NAMESPACE KATE

#endif // END SHADER_H