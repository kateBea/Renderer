#include "../include/Shader.hh"

namespace kT {
    Shader::Shader(const std::filesystem::path &vertexSourceDir, const std::filesystem::path &fragmentSourceDir) {
        m_Id = glCreateProgram();
        load(vertexSourceDir, fragmentSourceDir);
    }

    auto Shader::use() const -> void {
        glUseProgram(this->m_Id);
    }

    auto Shader::getProgram() const -> std::uint32_t {
        return this->m_Id;
    }

    Shader::~Shader() {
        glDeleteProgram(this->m_Id);
    }

    Shader::Shader() {
        m_Id = glCreateProgram();

        if (m_Id == 0)
            std::cerr << "Error when creating shader program\n";
    }

    auto Shader::load(const std::filesystem::path& vShaderPath, const std::filesystem::path& fShaderPath) const -> void {
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

    auto Shader::compile(const char* content, GLenum shaderType) -> std::uint32_t {
        std::uint32_t shaderId{};
        shaderId = glCreateShader(shaderType);
        glShaderSource(shaderId, 1, &content, nullptr);
        glCompileShader(shaderId);
        showShaderStatus(shaderId, getShaderErrorStr(shaderType), GL_COMPILE_STATUS);

        return shaderId;
    }

    auto Shader::build(const char* vShader, const char* fShader) const -> void {
        std::uint32_t vertexShaderID{ compile(vShader, GL_VERTEX_SHADER) };
        std::uint32_t pixelShaderID{ compile(fShader, GL_FRAGMENT_SHADER) };

        // Create and link program against compiled Shader binaries
        glAttachShader(getProgram(), vertexShaderID);
        glAttachShader(getProgram(), pixelShaderID);
        glLinkProgram(getProgram());
        showProgramStatus(getProgram(), GL_LINK_STATUS);

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
            std::cerr << "Error:" "[ "<< name << " ] is not a valid uniform name for this program shader\n";
        else
            glUniform1i(ret, static_cast<std::int32_t>(value));
    }

    auto Shader::setUniformInt(std::string_view name, std::int32_t value) const -> void {
        use();
        auto ret{ glGetUniformLocation(getProgram(), name.data()) };

        if (ret == -1)
            throw std::runtime_error((std::string("Error:" "[ ") + name.data() + " ] is not a valid uniform name for this program shader").c_str());
        else
            glUniform1i(ret, value);
    }

    auto Shader::setUniformFloat(std::string_view name, float value) const -> void {
        use();
        auto ret{ glGetUniformLocation(getProgram(), name.data()) };

        if (ret == -1)
            std::cerr << "Error:" "[ "<< name << " ] is not a valid uniform name for this program shader\n";
        else
            glUniform1f(ret, value);
    }

    auto Shader::showShaderStatus(std::uint32_t objectId, std::string_view str, GLenum status) -> void {
        std::array<char, 512> outStr{};
        std::int32_t success{};

        glGetShaderiv(objectId, status, &success);
        switch (status) {
            case GL_COMPILE_STATUS:
                if (success != GL_TRUE) {
                    glGetShaderInfoLog(objectId, outStr.size(), nullptr, outStr.data());
                    std::printf("%s:\n %s\n", str.data(), outStr.data());
                }
                break;
        }
    }

    auto Shader::showProgramStatus(std::uint32_t objectId, GLenum status) -> void {
        std::array<char, 1024> outStr{};
        std::int32_t success{};

        glGetProgramiv(objectId, status, &success);
        switch (status) {
            case GL_LINK_STATUS:
                if (success != GL_TRUE) {
                    glGetProgramInfoLog(objectId, outStr.size(), nullptr, outStr.data());
                    std::printf("Error on shader program:\n %s\n", outStr.data());
                }
                break;
        }
    }

    auto Shader::setUniformMat4(std::string_view name, const glm::mat4& mat) const -> void {
        use();
        auto ret{ glGetUniformLocation(getProgram(), name.data()) };

        if (ret == -1)
            std::cerr << "Error:" "[ "<< name << " ] is not a valid uniform name for this program shader\n";
        else
            /*
             * If transpose is GL_FALSE, each matrix is assumed to be supplied in column major order.
             * If transpose is GL_TRUE, each matrix is assumed to be supplied in row major order.
             * The count argument indicates the number of matrices to be passed. A count of 1
             * should be used if modifying the value of a single matrix, and a count greater
             * than 1 can be used to modify an array of matrices.
             * from: https://docs.gl/gl4/glUniform
             *
             * we pass GL_FALSE because glm::mat4 has each row stored contiguously in memory by default
             * meaning the elements of the first row are stored first, followed by the
             * elements of the second row, and so on.
             * */
            glUniformMatrix4fv(ret, 1, GL_FALSE, glm::value_ptr(mat));
    }

    auto Shader::setUniformVec3(std::string_view name, const glm::vec3 &vec) const -> void {
        use();
        auto ret{ glGetUniformLocation(getProgram(), name.data()) };

        if (ret == -1)
            std::cerr << "Error:" "[ "<< name << " ] is not a valid uniform name for this program shader\n";
        else {
            // we pass we 1 because the shader uniform is not expected to be an array
            glUniform3fv(ret, 1, glm::value_ptr(vec));
        }
    }

    auto Shader::setUniformVec4(std::string_view name, const glm::vec4& vec) const -> void {
        use();
        auto ret{ glGetUniformLocation(getProgram(), name.data()) };

        if (ret == -1)
            std::cerr << "Error:" "[ "<< name << " ] is not a valid uniform name for this program shader\n";
        else {
            // we pass we 1 because the shader uniform is not expected to be an array
            glUniform4fv(ret, 1, glm::value_ptr(vec));
        }
    }

    Shader::Shader(Shader &&other) noexcept
        :   m_Id{ other.getProgram() }
    {
        // assign 0 so that it can be safely passed to glDeleteProgram()
        // when the destructor is called. We avoid deleting a valid program this way
        other.m_Id = 0;
    }

    Shader& Shader::operator=(Shader &&other) noexcept {
        m_Id = other.getProgram();
        other.m_Id = 0;

        return *this;
    }

    constexpr auto Shader::getShaderErrorStr(GLenum type) -> std::string_view {
        switch (type) {
            case GL_VERTEX_SHADER: return "Error on vertex Shader compilation";
            case GL_FRAGMENT_SHADER:  return "Error on fragment Shader compilation";
            default: return "Unknown type of shader";
        }
    }
}