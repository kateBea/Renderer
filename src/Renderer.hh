/**
 * @file Renderer.hh
 * @author Kate
 * @brief Defines the Renderer interface
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
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Project Libraries
#include <Mesh.hh>
#include <Camera.hh>
#include <Shader.hh>
#include <Window.hh>
#include <Vao.hh>
#include <Vbo.hh>
#include <utils.hh>

namespace Kate {
    class Renderer {
    public:

        /**
         * Initializes both the GLEW and the GLFW library and provides with a valid
         * OpenGL context. On success it sets the Init bit to true, otherwise it sets it to false
         * @param appName Title for the Renderer Window
         * */
        explicit Renderer(std::string_view name = "Renderer", std::int32_t width = 900, std::int32_t height = 720);

        auto run() -> void;
        auto startUp() -> void;

        ~Renderer();

    private:
        Kate::Window    m_Window{};     // main Window
        Kate::Shader    m_DefaultShaders{};    // Shader program id for fragment and Vertex shaders
        Kate::Vao       m_Vao{};        // Vertex array object identifier
        Kate::Vbo       m_Vbo{};        // Vertex buffer object identifier
        Kate::Vib       m_Vib{};
        Kate::Camera    m_Camera{};
        Kate::Texture   m_Texture{};
        std::vector<Mesh> m_Meshes{};
    };


    // IMPLEMENTATION
    inline Renderer::Renderer(std::string_view name, std::int32_t width, std::int32_t height)
        : m_Window{name, width, height }
        , m_DefaultShaders{}
        , m_Vao{}
        , m_Vbo{}
        , m_Camera{}
    {

    }

    inline auto Renderer::run() -> void {
        while (!m_Window.shouldClose()) {
            m_Window.resize();
            m_Window.showCursorPos();

            // Clear background
            glClearColor(0.2f, 0.5f, 0.4f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Texture.bind();

            // draw commands
            m_DefaultShaders.use();
            m_Vao.bind();
            m_Vib.bind();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            m_Window.draw();
        }
    }

    inline auto Renderer::startUp() -> void {
        // setup vertices
        auto v_pos{ Kate::parseVerticesFile("../assets/vertices") };
        auto indices{ std::vector<std::uint32_t>{ 0, 1, 3, 1, 2, 3 } };

        m_Vbo.load(v_pos);
        m_Vib.load(indices);
        m_DefaultShaders.load(
                "../assets/shaders/defaultVertexShader.glsl",
                "../assets/shaders/defaultPixelShader.glsl"
        );
        m_Texture.load("../assets/textures/container.jpg");

        // Vertex position attribute
        m_Vbo.bind();
        m_Vao.layout(0, 3, 0, 8);

        // Vertex color attribute
        m_Vbo.bind();
        m_Vao.layout(1, 3, 3, 8);

        // Vertex Texture attribute
        m_Vbo.bind();
        m_Vao.layout(2, 2, 6, 8);
    }

    inline Renderer::~Renderer() {
        glfwTerminate();
    }

}


#endif // RENDERER_HH
