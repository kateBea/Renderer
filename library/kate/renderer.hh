/**
 * @file renderer.hh
 * @author kate
 * @brief Defines the renderer interface
 * @version 1.0
 * @date 2023-03-16
 */

#ifndef RENDERER_HH
#define RENDERER_HH

// C++ Standard Library
#include <array>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <string_view>
#include <string>
#include <vector>

// Third-Party Libraries
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Project Libraries
#include <camera.hh>
#include <shader.hh>
#include <obj_parser.hh>
#include <vertex_array_object.hh>
#include <vertex_buffer_object.hh>
#include <vertex_index_buffer.hh>

namespace kate {
    class renderer {
    public:

        /**
         * Initializes both the GLEW and the GLFW library and provides with a valid
         * OpenGL context. On success it sets the Init bit to true, otherwise it sets it to false
         * @param appName Title for the renderer window
         * */
        explicit renderer(std::string_view appName = "renderer", std::int32_t width = 1280, std::int32_t height = 720);

        [[nodiscard]]
        auto init_ok() const -> bool;

        auto run() -> void;
        auto start_up() -> void;

        ~renderer();

    private:
        // Member variables
        GLFWwindow*         m_window{};     // main window
        kate::shader        m_dshader{};    // shader program id for fragment and vertex shaders
        std::uint32_t       m_vbo{};        // Vertex buffer object identifier
        std::uint32_t       m_vao{};        // Vertex array object identifier

        // flags that define the state of the renderer
        bool m_init{};
    };


    // IMPLEMENTATION
    inline renderer::renderer(std::string_view appName, std::int32_t width, std::int32_t height) {
        // Init GLFW Library
        if (!glfwInit()) {
            std::cerr << "Failed to initialize glfw...\n"
                         "----------------------------\n";

            this->m_init = false;
            return;
        }

        // Set OpenGL major and minor version numbers for OpenGL 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

        // Create valid context
        m_window = glfwCreateWindow(width, height, appName.data(), nullptr, nullptr);

        if (!m_window) {
            std::cerr << "There was an error creating the Window\n"
                         "--------------------------------------\n";
            this->m_init = false;
            return;
        }

        // Create a valid context for initializing GLEW
        glfwMakeContextCurrent(this->m_window);

        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            std::cerr << "Failed to initialize GLEW!\n"
                         "--------------------------\n";
            this->m_init = false;
            return;
        }

        // start-up succeeded
        this->m_init = true;
        std::cerr << "Start-up succeeded...\n"
                     "---------------------\n";
    }

    inline auto renderer::run() -> void {
        // main loop
        while (!glfwWindowShouldClose(this->m_window)) {
            glfwPollEvents();

            m_dshader.use();

            glClearColor(0.2f, 0.5f, 0.4f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // enable first attribute: vertex positions
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<const void*>(nullptr));

            glDrawArrays(GL_TRIANGLES, 0, 3);
            glDisableVertexAttribArray(0);

            glfwSwapBuffers(this->m_window);
        }
    }

    inline auto renderer::start_up() -> void {
        // setup vertices
        std::vector vertex_positions {
            // Positions
            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
            0.0f, -0.5f, 0.0f,
        };

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glEnableVertexAttribArray(0);

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, vertex_positions.size() * sizeof(decltype(vertex_positions)::value_type), vertex_positions.data(), GL_STATIC_DRAW);

        m_dshader = std::move(kate::shader("assets/shaders/defaultVertexShader.glsl", "assets/shaders/defaultPixelShader.glsl"));

    }

    inline renderer::~renderer() {
        glDeleteBuffers(1, &m_vbo);
        glDeleteVertexArrays(1, &m_vao);

        glfwTerminate();
    }

    inline auto renderer::init_ok() const -> bool {
        return this->m_init;
    }
}


#endif // RENDERER_HH
