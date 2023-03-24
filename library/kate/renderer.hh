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
#include <memory>

// Third-Party Libraries
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Project Libraries
#include <camera.hh>
#include <shader.hh>
#include <parser.hh>
#include <window.hh>
#include <vao.hh>
#include <vbo.hh>
#include <vib.hh>

namespace kate {
    class renderer {
    public:

        /**
         * Initializes both the GLEW and the GLFW library and provides with a valid
         * OpenGL context. On success it sets the Init bit to true, otherwise it sets it to false
         * @param appName Title for the renderer window
         * */
        explicit renderer(std::string_view name = "renderer", std::int32_t width = 1280, std::int32_t height = 720);

        auto run() -> void;
        auto start_up() -> void;

        ~renderer();

    private:
        // Member variables
        kate::window   m_window{};     // main window
        kate::shader                    m_dshader{};    // shader program id for fragment and vertex shaders
        kate::vao                  m_vao{};        // Vertex array object identifier
        kate::vbo                   m_vbo{};        // Vertex buffer object identifier

    };


    // IMPLEMENTATION
    inline renderer::renderer(std::string_view name, std::int32_t width, std::int32_t height)
        :   m_window{ name, width, height }
        ,   m_dshader{}
        ,   m_vao{}
        ,   m_vbo{}
    {

    }

    inline auto renderer::run() -> void {
        while (!m_window.should_close()) {
            m_window.resize();
            m_window.show_cursor_pos();

            // Clear background
            glClearColor(0.2f, 0.5f, 0.4f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_dshader.use();

            // vertex position attribute
            m_vbo.bind();
            m_vao.define_layout(0, 3, 0);

            // draw commands
            glDrawArrays(GL_TRIANGLES, 0, 3);
            m_window.draw();
        }
    }

    inline auto renderer::start_up() -> void {
        // setup vertices
        auto v_pos{ kate::get_vertices_data("assets/vertices") };

        m_vbo.load_data(v_pos);
        m_dshader.load_shaders(
            "assets/shaders/defaultVertexShader.glsl",
            "assets/shaders/defaultPixelShader.glsl"
        );
    }

    inline renderer::~renderer() {
        glfwTerminate();
    }

}


#endif // RENDERER_HH
