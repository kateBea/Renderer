#include "../include/Shader.hh"

namespace Kate {
    Shader::Shader(const std::filesystem::path &vertexSourceDir, const std::filesystem::path &fragmentSourceDir) {
        m_id = glCreateProgram();
        load(vertexSourceDir, fragmentSourceDir);
    }

    void Shader::use() const {
        glUseProgram(this->m_id);
    }

    std::uint32_t Shader::getProgram() const {
        return this->m_id;
    }

    Shader::~Shader() {
        glDeleteProgram(this->m_id);
    }

    Shader::Shader() {
        m_id = glCreateProgram();

        if (!m_id)
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
            std::cerr << "Error:" "[ "<< name << " ] is not a valid uniform name for this program shader\n";
        else
            glUniform1i(ret, static_cast<std::int32_t>(value));
    }

    auto Shader::setUniformInt(std::string_view name, std::int32_t value) const -> void {
        use();
        auto ret{ glGetUniformLocation(getProgram(), name.data()) };

        if (ret == -1)
            std::cerr << "Error:" "[ "<< name << " ] is not a valid uniform name for this program shader\n";
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

    auto Shader::showShaderStatus(std::uint32_t objectId, std::string_view str, GLenum name) -> void {
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

    auto Shader::showProgramStatus(std::uint32_t objectId, std::string_view str, GLenum name) -> void {
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
            // we pass we 1 because the uniform is not expected to
            // an array
            glUniform3fv(ret, 1, glm::value_ptr(vec));
        }
    }

    auto Shader::setUniformVec4(std::string_view name, const glm::vec4& vec) const -> void {
        use();
        auto ret{ glGetUniformLocation(getProgram(), name.data()) };

        if (ret == -1)
            std::cerr << "Error:" "[ "<< name << " ] is not a valid uniform name for this program shader\n";
        else {
            // we pass we 1 because the uniform is not expected to
            // an array
            glUniform4fv(ret, 1, glm::value_ptr(vec));
        }
    }
}