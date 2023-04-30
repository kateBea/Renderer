#include "../include/Renderer.hh"

namespace Kate {
    // IMPLEMENTATION
    Renderer::Renderer(std::string_view name, std::int32_t width, std::int32_t height)
            :   m_Window{name, width, height }
            ,   m_DefaultShaders{}
            ,   m_Light{}
            ,   m_Vao{}
            ,   m_Vbo{}
            ,   m_Vib{}
            ,   m_Camera{ m_Window }
            ,   m_Texture{}
            ,   m_Meshes{}
    {

    }

    auto Renderer::run() -> void {
        ImVec4 bgColor = ImVec4(0.254f, 0.083f, 0.144f, 1.00f);
        glm::vec3 rotationAngles{};
        glm::vec3 modelPosition{};
        glm::vec3 modelSize{ 1.0, 1.0, 1.0 };

        glm::vec3 lightPosition{ 3.0f, 0.0f, -5.0f };
        glm::vec3 lightColor{ 255.0f, 255.0f, 255.0f };

        float alphaValue = 1.0f;

        m_DefaultShaders.setUniformInt("material.diffuse", 0);

        while (!m_Window.shouldClose()) {
            // ImGui Frame Setup
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            {
                // Control Panel
                ImGui::Begin("Control Panel");
                ImGui::SliderFloat("Background Alpha", &alphaValue, 0.0f, 1.0f);
                ImGui::ColorEdit3("clear color", reinterpret_cast<float*>(&bgColor));
                ImGui::Text("Container Block Settings");
                ImGui::DragFloat3("B_Rotation", glm::value_ptr(rotationAngles), 0.1f, 0.0f, 360.0f);
                ImGui::DragFloat3("B_Coordinates", glm::value_ptr(modelPosition), 0.01f, -100.0f, 100.0f);
                ImGui::DragFloat3("B_Size", glm::value_ptr(modelSize), 0.01f, 0.0f, 20.0f);
                ImGui::Text("Light block Settings");
                ImGui::DragFloat3("L_Position", glm::value_ptr(lightPosition), 0.01f, -100.0f, 100.0f);
                ImGui::DragFloat3("L_Color", glm::value_ptr(lightColor), 1.0f, 0.0f, 255.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            // Clear background
            glClearColor(bgColor.x, bgColor.y, bgColor.z, alphaValue);

            // use GL_DEPTH_BUFFER_BIT  because depth testing is enabled by default
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // RENDER CUBE
            {
                // Update model matrix. translate, scale, rotate
                glm::mat4 model{ glm::mat4(1.0f) };
                glm::vec3 transtalion{ glm::vec3(modelPosition[0], modelPosition[1], modelPosition[2]) };
                glm::vec3 resizing{ glm::vec3(modelSize[0], modelSize[1], modelSize[2]) };
                model = glm::translate(model, transtalion);
                model = glm::scale(model, resizing);
                model = glm::rotate(model, rotationAngles[0], glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::rotate(model, rotationAngles[1], glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::rotate(model, rotationAngles[2], glm::vec3(1.0f, 0.0f, 0.0f));

                // Update view and projection
                m_Camera.updateProjection(m_Window.getWidth(), m_Window.getHeight());
                if (m_Window.isMouseButtonDown(GLFW_MOUSE_BUTTON_2))
                    m_Camera.move(m_Window.getCursorPosition());
                m_Camera.lookAround(m_Window, glm::vec3(0.0f, 0.0f, 0.0f));

                m_DefaultShaders.use();
                m_DefaultShaders.setUniformVec3("light.position", glm::vec3(lightPosition[0], lightPosition[1], lightPosition[2]));
                m_DefaultShaders.setUniformVec3("viewPos", m_Camera.getPosition());

                // light properties
                m_DefaultShaders.setUniformVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
                m_DefaultShaders.setUniformVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
                m_DefaultShaders.setUniformVec3("light.specular", glm::vec4(lightColor[0] / 255, lightColor[1] / 255, lightColor[2] / 255, 1.0f));

                // material properties
                m_DefaultShaders.setUniformVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
                m_DefaultShaders.setUniformFloat("material.shininess", 64.0f);

                // Pass mvp matrices to the default shader
                m_DefaultShaders.setUniformMat4("model", model);
                m_DefaultShaders.setUniformMat4("view", m_Camera.getView());
                m_DefaultShaders.setUniformMat4("projection", m_Camera.getProjection());

                m_Texture.bindUnit(0);

                // render objects (this should be done by the model class)
                m_DefaultShaders.use();
                m_Vbo.bind();
                m_Vao.bind();

                // NOTE: 36 because we have 6 faces to render with 2 triangles
                // each and 3 vertices per triangles, same goes for the light cube
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            // RENDER CUBE LIGHT
            {
                m_LightShader.use();

                glm::mat4 lightBlock{ glm::mat4(1.0f) };
                glm::vec3 translation{ glm::vec3(lightPosition[0], lightPosition[1], lightPosition[2]) };
                lightBlock = glm::translate(lightBlock, translation);

                m_LightShader.setUniformMat4("model", lightBlock);
                m_LightShader.setUniformMat4("view", m_Camera.getView());
                m_LightShader.setUniformMat4("projection", m_Camera.getProjection());

                m_LightShader.setUniformVec4("lightColor", glm::vec4(lightColor[0] / 255, lightColor[1] / 255, lightColor[2] / 255, 1.0f));
                m_Light.bind();
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            // window render and gui render
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            m_Window.draw();
        }
    }

    auto Renderer::startUp() -> void {
        // Setup OpenGL Rendering
        setOpenGLHints();


        // Initialize the GUI library
        initImGui();

        // setup vertices
        auto v_pos{ Kate::parseVerticesFile("../assets/vertices") };

        // this part is not necessary as we can use the same vertex positions from the model
        // and simply specify a different layout for the cube light
        auto lightPos{ Kate::parseVerticesFile("../assets/lightingBoxVertices") };
        auto indices{ std::vector<std::uint32_t>{ 0, 1, 3, 1, 2, 3 } };

        m_Light.load(lightPos);
        m_Vbo.load(v_pos);
        m_Vib.load(indices);

        m_DefaultShaders.load("../assets/shaders/defaultVertex.glsl", "../assets/shaders/defaultFragment.glsl");
        m_LightShader.load("../assets/shaders/lightVertex.glsl", "../assets/shaders/lightFragment.glsl");
        m_Texture.load("../assets/textures/container2.png");

        // Vertex position attribute
        m_Vbo.bind();
        m_Vao.layout(0, 3, 0, 8);

        // Vertex Normals attribute
        m_Vbo.bind();
        m_Vao.layout(1, 3, 3, 8);

        // Vertex Texture attribute
        m_Vbo.bind();
        m_Vao.layout(2, 2, 6, 8);
    }

    auto Renderer::shutDown() -> void {

    }

    auto Renderer::initImGui() -> void {
        // Initialize ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(m_Window.getWindowPointer(), true);

        // NOTE: this has to be the same as glMajor and glMinor variables in Window class
        // needs rework to be done automatically
        ImGui_ImplOpenGL3_Init(fmt::format("#version {}{}0", Kate::GLMajor, Kate::GLMinor).c_str());
        ImGui::StyleColorsClassic();

        // Window softness (rounding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);

        ImGuiIO& io = ImGui::GetIO();

        // can C-Style void cast "io" to, i.e. (void)io in case of compiler warning about unused variable
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    }

    auto Renderer::enableWireframeMode() -> void {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    auto Renderer::disableWireframeMode() -> void {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    auto Renderer::setOpenGLHints() -> void {
        glEnable(GL_DEPTH_TEST);
    }
}