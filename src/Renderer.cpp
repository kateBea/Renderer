
#include "Renderer.hh"

namespace Kate {
    // IMPLEMENTATION
    Renderer::Renderer(std::string_view name, std::int32_t width, std::int32_t height)
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

    auto Renderer::run() -> void {
        float red{};
        float blue{};
        float green{};

        bool wireframe{ false };

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
                wireframe = !wireframe;

                if (wireframe)
                    Kate::Window::enableWireframeMode();
                else
                    Kate::Window::disableWireframeMode();
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
            glm::mat4 model{ glm::rotate(glm::mat4(1.0f), static_cast<float>(glfwGetTime()) *
                                                          glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f)) };

            // view
            glm::mat4 view{ m_Camera.update(m_Window) };

            // projection
            glm::mat4 projection{ glm::perspective(glm::radians(45.0f),
                                                   static_cast<float>(m_Window.getWidth()) / static_cast<float>(m_Window.getHeight()),
                                                   0.1f, 100.0f) };

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
            glDrawArrays(GL_TRIANGLES, 0, 36);
            m_Window.draw();
        }
    }

    auto Renderer::startUp() -> void {
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
        m_Texture[1].load("../assets/textures/lava512x512.png");

        // Vertex position attribute
        m_Vbo.bind();
        m_Vao.layout(0, 3, 0, 5);

        // Vertex Texture attribute
        m_Vbo.bind();
        m_Vao.layout(1, 2, 3, 5);
    }

    Renderer::~Renderer() {
        glfwTerminate();
    }
}