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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
        std::vector<Kate::Texture> m_Texture{};
        std::vector<Mesh> m_Meshes{};
    };


    // IMPLEMENTATION
    inline Renderer::Renderer(std::string_view name, std::int32_t width, std::int32_t height)
        :   m_Window{name, width, height }
        ,   m_DefaultShaders{}
        ,   m_Vao{}
        ,   m_Vbo{}
        ,   m_Vib{}
        ,   m_Camera{}
        ,   m_Texture(2)
        ,   m_Meshes{}
    {

    }

    inline auto Renderer::run() -> void {
        float red{};
        float blue{};
        float green{};

        auto someTests{ [&]() -> void {
            if (m_Window.isKeyPressed(GLFW_KEY_1)) {
                if (red >= 1.0f)
                    red = 0.0f;
                else
                    red += 0.01;
            }

            if (m_Window.isKeyPressed(GLFW_KEY_2)) {
                if (green >= 1.0f)
                    green = 0.0f;
                else
                    green += 0.01;
            }

            if (m_Window.isKeyPressed(GLFW_KEY_3)) {
                if (blue >= 1.0f)
                    blue = 0.0f;
                else
                    blue += 0.01;
            }

            if (m_Window.isKeyPressed(GLFW_KEY_BACKSPACE)) {
                m_Window.showCursorPos();
            }
        }};

        std::cout << "Press 1, 2 or 3 to change background colors and Backspace to show cursor position" << std::endl;

        while (!m_Window.shouldClose()) {
            m_Window.resize();

            someTests();

            // Clear background
            glClearColor(red, green, blue, 1.0f);
            // use GL_DEPTH_BUFFER_BIT  because depth testing is enabled by default
            // when creating a Kate::Window
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // model
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

            // view
            glm::mat4 view = glm::mat4(1.0f);
            view = m_Camera.update(m_Window);

            // projection
            glm::mat4 projection;
            projection = glm::perspective(glm::radians(45.0f),
                                          static_cast<float>(m_Window.getWidth()) / static_cast<float>(m_Window.getHeight()),
                                          0.1f, 100.0f);

            m_DefaultShaders.setUniformMat4("model", model);
            m_DefaultShaders.setUniformMat4("view", view);
            m_DefaultShaders.setUniformMat4("projection", projection);

            m_Texture[0].bindUnit(0);
            m_Texture[1].bindUnit(1);

            m_DefaultShaders.setUniformInt("u_Texture1", 0);
            m_DefaultShaders.setUniformInt("u_Texture2", 1);

            // draw commands
            m_DefaultShaders.use();
            m_Vao.bind();
            m_Vib.bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
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
        m_Texture[0].load("../assets/textures/container.jpg");
        m_Texture[1].load("../assets/textures/awesomeface.png");

        // Vertex position attribute
        m_Vbo.bind();
        m_Vao.layout(0, 3, 0, 5);

        // Vertex Texture attribute
        m_Vbo.bind();
        m_Vao.layout(1, 2, 3, 5);
    }

    inline Renderer::~Renderer() {
        glfwTerminate();
    }

}


#endif // RENDERER_HH
