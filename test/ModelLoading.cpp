#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Core/Window.hh"
#include "OpenGL/Camera.hh"
#include "OpenGL/Model.hh"
#include "OpenGL/Shader.hh"
#include "Core/Logger.hh"
#include "Core/Application.hh"

class ModelLoader : public kT::Application {
public:
    auto startUp() -> void override {
        kT::Logger::Init();
        KATE_LOGGER_DEBUG("Logger init successful");
        KATE_LOGGER_DEBUG("Initializing application");

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        KATE_LOGGER_INFO("Depth testing and blending enabled");

        // blending function
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        initGUI();
        KATE_LOGGER_INFO("ImGui init successful");
        KATE_LOGGER_INFO("Finished Application start up");
    }

    auto loop() -> void override {
        m_DefaultShader.setUniformVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        m_DefaultShader.setUniformVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        m_DefaultShader.setUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        m_DefaultShader.setUniformFloat("material.shininess", 64.0f);

        while (!m_Window.shouldClose()) {
            // ImGui Frame Setup
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            pushInspection();
            updateMatrices();

            if (m_Lines)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            // Rendering ------------
            glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            m_Model.draw(m_DefaultShader);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            // Update and Render additional Platform Windows
            // (Platform functions may change the current OpenGL context, so
            // we save/restore it to make it easier to paste this code elsewhere.
            // For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                auto backupCurrentContext = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backupCurrentContext);
            }

            m_Window.draw();

        }
    }

    auto shutDown() -> void override {}

private:
    auto updateMatrices() -> void {
        // Update model matrix. translate, scale, rotate
        glm::mat4 model{glm::mat4(1.0f)};
        glm::vec3 tr{glm::vec3(m_ModelPosition[0], m_ModelPosition[1], m_ModelPosition[2])};
        glm::vec3 resizing{glm::vec3(m_ModelSize[0], m_ModelSize[1], m_ModelSize[2])};
        model = glm::translate(model, tr);
        model = glm::scale(model, resizing);
        model = glm::rotate(model, m_Rotation[0], glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, m_Rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, m_Rotation[2], glm::vec3(1.0f, 0.0f, 0.0f));

        // view/projection transformations
        m_Camera.updateProjection(m_Window.getWidth(), m_Window.getHeight());

        if (kT::InputManager::isMouseButtonDown(GLFW_MOUSE_BUTTON_2)) {
            m_Camera.move(kT::InputManager::getMousePos());
        }
        m_Camera.lookAround(m_Window, glm::vec3(0.0f, 0.0f, 0.0f));

        updateUniforms(model);
    }

    auto updateUniforms(const glm::mat4& model) -> void {
        m_DefaultShader.setUniformVec3("light.position", glm::vec3(m_LightPosition[0], m_LightPosition[1], m_LightPosition[2]));
        m_DefaultShader.setUniformVec3("viewPos", m_Camera.getPosition());
        m_DefaultShader.setUniformMat4("model", model);
        m_DefaultShader.setUniformMat4("projection", m_Camera.getProjection());
        m_DefaultShader.setUniformMat4("view", m_Camera.getView());
    }

    auto pushInspection() -> void{
        // Control Panel
        ImGui::Begin("Control Panel");
        ImGui::ColorEdit3("clear color", reinterpret_cast<float *>(&m_ClearColor));
        ImGui::Text("Container Block Settings");
        ImGui::DragFloat3("Rotation", glm::value_ptr(m_Rotation), 0.1f, 0.0f, 360.0f);
        ImGui::DragFloat3("Coordinates", glm::value_ptr(m_ModelPosition), 0.01f, -100.0f, 100.0f);
        ImGui::DragFloat3("Size", glm::value_ptr(m_ModelSize), 0.01f, 0.0f, 20.0f);
        ImGui::Text("Light block Settings");
        ImGui::DragFloat3("Position", glm::value_ptr(m_LightPosition), 0.1f, -100.0f, 100.0f);
        ImGui::Checkbox("Enable wireframe", &m_Lines);
        displayModelData();
        ImGui::End();
    }

    auto initGUI() -> void {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

        // When viewports are enabled, we tweak WindowRounding/WindowBg so platform windows can
        // look identical to regular ones.
        ImGuiStyle &style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Load custom font
        io.Fonts->AddFontFromFileTTF("../Fonts/Inter/Inter-VariableFont.ttf", 15);
        setCustomStyle();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(m_Window.getWindowPointer(), true);
        ImGui_ImplOpenGL3_Init("#version 410");
        loadIniFile("../assets/imgui.ini");
    }

    auto displayModelData() -> void {
        static constexpr int HOURS_TO_SECS{ 3600 };
        static constexpr int MIN_TO_SECS{ 60 };
        static auto formatTime{
            [](int hour, int min, int sec) -> std::string {
                std::string result{"Time elapsed: "};

                result.append(hour > 9 ? std::to_string(hour) : "0" + std::to_string(hour));
                result.append(":");
                result.append(min > 9 ? std::to_string(min) : "0" + std::to_string(min));
                result.append(":");
                result.append(sec > 9 ? std::to_string(sec) : "0" + std::to_string(sec));

                return result;
            }
        };

        ImGui::Begin("Inspection");
        ImGui::Text("Model: 1"); // Just one model loaded at a time currently
        ImGui::Text("Vertices: %lx", m_Model.getVertexCount());
        ImGui::Text("Indices: %lx", m_Model.getIndexCount());
        ImGui::Text("Textures: %lx", m_Model.getTextureCount());
        ImGui::Text("Frame Rate: %.1f FPS)", ImGui::GetIO().Framerate);

        auto sTime = static_cast<int>(glfwGetTime());
        ImGui::Text("%s", formatTime(sTime / HOURS_TO_SECS, (sTime % HOURS_TO_SECS) / MIN_TO_SECS,
                                     (sTime % HOURS_TO_SECS) % MIN_TO_SECS).c_str());

        ImGui::End();
    }


    static auto loadIniFile(const std::filesystem::path &path) -> void {
        // Assumes there is already a valid ImGui context
        ImGuiIO &io = ImGui::GetIO();
        io.IniFilename = nullptr;
        ImGui::LoadIniSettingsFromDisk(path.native().c_str());
    }

    static auto setCustomStyle() -> void {
        // Setup Dear ImGui style.
        // This is supposed to be a simple variant
        // on top of the default ImGui Dark style

        ImGui::StyleColorsDark();
        ImGuiStyle &style = ImGui::GetStyle();

        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);

        style.Colors[ImGuiCol_Tab] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);

        style.Colors[ImGuiCol_Button] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.26f, 0.26f, 1.0f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);

        style.Colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.26f, 0.26f, 1.0f);
        style.Colors[ImGuiCol_TabActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
        style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);
        style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);

        style.Colors[ImGuiCol_Header] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.26f, 0.26f, 1.0f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);

        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);

        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);

        style.Colors[ImGuiCol_Border] = ImVec4(0.26f, 0.26f, 0.26f, 1.0f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);

        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);


        // borders
        style.WindowBorderSize = 0.0f;
        style.FrameBorderSize = 0.0f;
        style.PopupBorderSize = 0.0f;

        // Rounding values
        style.FrameRounding = 5.0f;
        style.GrabRounding = 5.0f;
        style.ChildRounding = 5.0f;
        style.WindowRounding = 5.0f;
        style.PopupRounding = 5.0f;
        style.ScrollbarRounding = 5.0f;
        style.TabRounding = 5.0f;
    }

    // ImGui Panels Data
    bool            m_Lines{ false };
    ImVec4          m_ClearColor = ImVec4(0.254f, 0.083f, 0.144f, 1.00f);
    glm::vec3       m_Rotation{};
    glm::vec3       m_ModelPosition{};
    glm::vec3       m_ModelSize{ 1.0, 1.0, 1.0 };
    glm::vec3       m_LightPosition{ 3.0f, 0.0f, -5.0f };

    kT::Window      m_Window{ "ModelLoading", 1280, 720 };
    kT::Camera      m_Camera{ m_Window };
    kT::Shader      m_DefaultShader{ "../assets/shaders/defaultVertex.glsl", "../assets/shaders/defaultFragment.glsl" };
    kT::Model       m_Model{ "../assets/models/Pod42/source/POD/POD.obj" };

    // Singleton InputManager. Check definition
    kT::InputManager s_Input{ m_Window.getWindowPointer() };
};

int main(int, char **) {
    auto loader{ std::make_unique<ModelLoader>() };

    loader->startUp();
    loader->loop();
    loader->shutDown();

    return 0;
}