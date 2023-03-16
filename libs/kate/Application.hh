//
// Created by kate on 3/16/2023.
//

#ifndef RENDERER_APPLICATION_HH
#define RENDERER_APPLICATION_HH

// C++ Standard Library
#include <iostream>
#include <cstdint>
#include <iterator>
#include <array>
#include <string_view>
#include <string>
#include <filesystem>

// Third-Party Libraries
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Project Libraries
#include <shader.hh>
#include <vertexBufferObject.hh>
#include <vertexIndexBuffer.hh>
#include <vertexArrayObject.hh>

namespace Kate {
    class Application {
    public:

        /**
         * Initializes both the GLEW and the GLFW library and provides with a valid
         * OpenGL context. On success it sets the Init bit to true, otherwise it sets it to false
         * @param appName Title for the application window
         * */
        explicit Application(std::string_view appName = "Application");

        auto        run() -> void;
        auto        startUp() -> void;
        static auto showCurrentWorkingDirectory() -> void;

        ~Application();


    private:
        // window dimensions
        static constexpr std::int32_t s_Width{ 1280 };
        static constexpr std::int32_t s_Height{ 720 };

        // helper functions
        static auto getVerticesData(const std::filesystem::path& path) -> std::vector<float>;

        // Member variables
        GLFWwindow*         m_Window{};
        Kate::Shader        m_ShaderProgram{};
        std::vector<float>  m_IndicesData{};
        std::uint32_t       m_VertexBufferId{};
        std::uint32_t       m_VertexArrayId{};

        // flags that define the state of the application
        bool m_Init{};
    };


    // IMPLEMENTATION
    inline auto Application::getVerticesData(const std::filesystem::path& path) -> std::vector<float> {
        std::ifstream file{ path };
        std::vector<float> retVal{};
        std::string temp{};

        if (file.is_open()) {
            while (std::getline(file, temp, ','))
                if (not temp.starts_with("//"))
                    retVal.push_back(std::stof(temp));
        }
        else
            std::cerr << "Could not open file containing vertices data...\n";

        return retVal;
    }

    inline Application::Application(std::string_view appName) {
        // Init GLFW Library
        if (!glfwInit()) {
            std::cerr << "Failed to initialize glfw...\n"
                         "----------------------------\n";

            this->m_Init = true;
            return;
        }

        // Set OpenGL major and minor version numbers for OpenGL 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

        // Create valid context
        m_Window = glfwCreateWindow(s_Width, s_Height, appName.data(), nullptr, nullptr);

        if (!m_Window) {
            std::cerr << "There was an error creating the Window\n"
                         "--------------------------------------\n";
            this->m_Init = false;
            return;
        }

        // Create a valid context for initializing GLEW
        glfwMakeContextCurrent(this->m_Window);

        glewExperimental = GL_TRUE;

        if (glewInit() != GLEW_OK) {
            std::cerr << "Failed to initialize GLEW!\n"
                         "--------------------------\n";
            this->m_Init = false;
            return;
        }

        // start-up succeeded
        this->m_Init = true;
        std::cerr << "Start-up succeeded...\n";
        std::cerr << "---------------------\n";
    }

    inline auto Application::run() -> void {
        // main loop
        while (!glfwWindowShouldClose(this->m_Window)) {
            glfwPollEvents();

            m_ShaderProgram.use();

            glClear(GL_COLOR_BUFFER_BIT);

            // 1st attribute buffer : vertices
            glEnableVertexAttribArray(0)

            glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<const void*>(nullptr));

            glDrawArrays(GL_TRIANGLES, 0, 3);
            glDisableVertexAttribArray(0);

            glfwSwapBuffers(this->m_Window);
        }
    }

    inline auto Application::startUp() -> void {
        // setup vertices
        m_IndicesData = std::move(std::vector {
            // Positions
            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
            0.0f, -0.5f, 0.0f,
        });

        glClearColor(0.2f, 0.5f, 0.4f, 0.0f);

        glGenVertexArrays(1, &m_VertexArrayId);
        glBindVertexArray(m_VertexArrayId);

        glEnableVertexAttribArray(0);

        glGenBuffers(1, &m_VertexBufferId);
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
        glBufferData(GL_ARRAY_BUFFER, m_IndicesData.size() * sizeof(decltype(m_IndicesData)::value_type), m_IndicesData.data(), GL_STATIC_DRAW);

        m_ShaderProgram = std::move(Kate::Shader("assets/shaders/defaultVertexShader.glsl", "assets/shaders/defaultPixelShader.glsl"));
    }

    inline auto Application::showCurrentWorkingDirectory() -> void {
        //Print the current working directory
        std::filesystem::path cwd = std::filesystem::current_path();
        std::cout << cwd << std::endl;
    }

    inline Application::~Application() {
        glDeleteBuffers(1, &m_VertexBufferId);
        glDeleteVertexArrays(1, &m_VertexArrayId);
        glDeleteProgram(m_ShaderProgram.getProgramId());

        glfwTerminate();
    }
}


#endif //RENDERER_APPLICATION_HH
