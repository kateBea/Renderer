
#include "../include/Renderer.hh"

namespace Kate {
    // IMPLEMENTATION
    Renderer::Renderer(std::string_view name, std::int32_t width, std::int32_t height)
            :   m_Window{name, width, height }
            ,   m_DefaultShaders{}
            ,   m_Vao{}
            ,   m_Vbo{}
            ,   m_Vib{}
            ,   m_Camera{ m_Window }
            ,   m_Texture(2)
            ,   m_Meshes{}
    {

    }

    auto Renderer::run() -> void {
        bool wireframe{ false };
        auto someTests{ [&]() -> void {
            if (m_Window.isKeyPressed(GLFW_KEY_BACKSPACE)) {
                wireframe = !wireframe;

                if (wireframe)
                    Kate::Window::enableWireframeMode();
                else
                    Kate::Window::disableWireframeMode();
            }
        }};

        ImVec4 bgColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        std::array<float, 3> rotationAngles{};
        float alphaValue = 0.0f;

        while (!m_Window.shouldClose()) {
            // ImGui Frame Setup
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // some useless tests
            someTests();

            {
                // Control Panel
                ImGui::Begin("Control Panel");
                ImGui::SliderFloat("Background Alpha", &alphaValue, 0.0f, 1.0f);
                ImGui::ColorEdit3("clear color", reinterpret_cast<float*>(&bgColor));
                ImGui::DragFloat3("Rotation", rotationAngles.data(), 0.1f, 0.0f, 360.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            // Clear background
            glClearColor(bgColor.x, bgColor.y, bgColor.z, alphaValue);
            // use GL_DEPTH_BUFFER_BIT  because depth testing is enabled by default
            // when creating a Kate::Window
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // model
            glm::vec3 rotationAxis{ glm::vec3(1.0f, 0.0f, 0.0f) };

            glm::mat4 model{ glm::rotate(glm::mat4(1.0f), rotationAngles[1], rotationAxis) };
            m_Camera.updateProjection(m_Window.getWidth(), m_Window.getHeight());

            m_DefaultShaders.setUniformMat4("model", model);
            if (m_Window.isMouseButtonDown(GLFW_MOUSE_BUTTON_2)) {
                m_Camera.move(m_Window.getCursorPosition());
            }
            m_Camera.lookAround(m_Window, glm::vec3(0.0f, 0.0f, 0.0f));
            m_DefaultShaders.setUniformMat4("view", m_Camera.getView());
            m_DefaultShaders.setUniformMat4("projection", m_Camera.getProjection());

            m_Texture[0].bindUnit(0);
            //m_Texture[1].bindUnit(1);

            m_DefaultShaders.setUniformInt("u_Texture1", 0);
            //m_DefaultShaders.setUniformInt("u_Texture2", 1);

            // render objects (this should be done by the model class)
            m_DefaultShaders.use();
            m_Vao.bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // window render and gui render
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            m_Window.draw();
        }
    }

    auto Renderer::startUp() -> void {
        initImGui();

        // setup vertices
        auto v_pos{ Kate::parseVerticesFile("../assets/vertices") };
        auto indices{ std::vector<std::uint32_t>{ 0, 1, 3, 1, 2, 3 } };

        m_Vbo.load(v_pos);
        m_Vib.load(indices);
        m_DefaultShaders.load(
                "../assets/shaders/defaultVertex.glsl",
                "../assets/shaders/defaultFragment.glsl"
        );
        m_Texture[0].load("../assets/textures/Pack_4_stones_on_grass_PBR_nafgames/StoneOnGrass_4/stone_on_grass_4_diffuseOriginal.png");
        m_Texture[1].load("../assets/textures/lava512x512.png");

        // Vertex position attribute
        m_Vbo.bind();
        m_Vao.layout(0, 3, 0, 5);

        // Vertex Texture attribute
        m_Vbo.bind();
        m_Vao.layout(1, 2, 3, 5);
    }

    Renderer::~Renderer() = default;

    auto Renderer::shutDown() -> void {

    }

    auto Renderer::initImGui() -> void {
        // Initialize ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(m_Window.getWindowPointer(), true);
        // NOTE: this has to be the same as glMajor and glMinor variables in Window class
        // needs rework to be done automatically
        ImGui_ImplOpenGL3_Init("#version 410");
        ImGui::StyleColorsClassic();

        // Window softness (rounding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);

        ImGuiIO& io = ImGui::GetIO();

        // can C-Style cast io to void, i.e. (void)io in case of compiler warning about unused variable
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    }
}