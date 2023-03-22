/**
 * @file shader.hh
 * @author kate
 * @brief Defines the shader interface
 * @version 1.0
 * @date 2023-03-21
 */

#ifndef SHADER_HH
#define SHADER_HH

// C++ Standard Library
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string_view>
#include <cstddef>

// Third-Party Libraries
#include <GL/glew.h>

namespace kate {
    class shader {
    public:
        /**
         * Default initialization for shader
         * */
        shader() = default;

        /**
         * Copy constructor. Marked as delete to avoid shader aliasing
         * Ensure one shader id is held by one shader object
         * */
        shader(const shader& other) = delete;

        /**
         * Copy assigment. Marked as delete to avoid shader aliasing
         * Ensure one shader id is held by one shader object
         * */
        shader& operator=(const shader& other) = delete;

        /**
         * Move constructor
         * */
         shader(shader&& other) noexcept
            : m_program_id{other.m_program_id }
        {
            // when other is a temporary 0 is ignored if
            // passed to glDeleteProgram(). We avoid deleting a valid program this way
            other.m_program_id = 0;
        }

        /**
         * Move constructor assignment
         * */
        shader& operator=(shader&& other) noexcept {
            m_program_id = other.m_program_id;
            other.m_program_id = 0;

            return *this;
        }

        /**
         * Construct shader program from path to vertex shader source file directory
         * and pixel shader source file directory
         * @param vertexSourceDir directory to the vertex shader source file
         * @param fragmentSourceDir directory to the pixel shader source file
         * */
        shader(std::string_view vertexSourceDir, std::string_view fragmentSourceDir);

        /**
         * Use this shader program
         * */
        auto use() const -> void;

        /**
         * Get shader program ID
         * @return shader program ID
         * */
        [[nodiscard]]
        auto getProgramId() const -> std::uint32_t;

        /**
         * Perform cleanup
         * */
        ~shader();

    private:
        /*
         * Helper function to retrieve shader compilation/linking status
         * */
        static auto show_status_shader(std::uint32_t objectId, const char* str, GLenum name) -> void{
            std::array<char, 512> outStr{};
            std::int32_t success{};
            std::int32_t length{};

            glGetShaderiv(objectId, GL_COMPILE_STATUS, &success);
            glGetShaderiv(objectId, GL_INFO_LOG_LENGTH, &length);
            if (length > 0) {
                glGetShaderInfoLog(objectId, outStr.size(), nullptr, outStr.data());
                std::printf("%s: %s\n", str, outStr.data());
            }
        }

        /*
         * Helper function to retrieve shader compilation/linking status
         * */
        static auto show_status_program(std::uint32_t objectId, const char* str, GLenum name) -> void{
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
        std::uint32_t m_program_id{};
    };

    inline shader::shader(std::string_view vertexSourceDir, std::string_view fragmentSourceDir) {
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

            if (!vertexShaderFile.is_open())
                throw std::runtime_error("could not open vertex shader file...");

            if (!pixelShaderFile.is_open())
                throw std::runtime_error("could not open fragment shader file...");

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
        show_status_shader(vertexShaderID, "Error on vertex shader compilation: ", GL_COMPILE_STATUS);

        // Load and compile the fragment shader
        std::uint32_t pixelShaderID{};
        pixelShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(pixelShaderID, 1, pixelShaderCstring, nullptr);
        glCompileShader(pixelShaderID);
        show_status_shader(pixelShaderID, "Error on fragment shader compilation: ", GL_COMPILE_STATUS);

        // Create and link program against compiled shader binaries
        this->m_program_id = glCreateProgram();
        glAttachShader(this->m_program_id, vertexShaderID);
        glAttachShader(this->m_program_id, pixelShaderID);
        glLinkProgram(this->m_program_id);
        show_status_program(this->m_program_id, "Error on program linking: ", GL_LINK_STATUS);

        glDetachShader(this->m_program_id, vertexShaderID);
        glDetachShader(this->m_program_id, pixelShaderID);

        // cleanup
        glDeleteShader(vertexShaderID);
        glDeleteShader(pixelShaderID);
    }

    inline void shader::use() const {
        glUseProgram(this->m_program_id);
    }

    inline std::uint32_t shader::getProgramId() const {
        return this->m_program_id;
    }

    inline shader::~shader() {
        glDeleteProgram(this->m_program_id);
    }

}

#endif // END SHADER_HH