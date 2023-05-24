#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "../include/Window.hh"
#include "../include/Camera.hh"
#include "../include/Model.hh"
#include "../include/Shader.hh"
#include "../include/Logger.hh"
#include "../include/Application.hh"

class ModelLoader : public kT::Application {
public:
    auto startUp()  -> void override {
        kT::Logger::Init();
        KATE_LOGGER_INFO("Logger init successful");
        KATE_LOGGER_INFO("Initializing application");

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        KATE_LOGGER_WARN("Depth testing and blending enabled");

        // blending function
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        initImGui(window);
        KATE_LOGGER_INFO("ImGui init successful");
        KATE_LOGGER_INFO("Finished Application start up");
    }

    auto run()  -> void override {
        ImVec4 bgColor = ImVec4(0.254f, 0.083f, 0.144f, 1.00f);
        glm::vec3 rotationAngles{};
        glm::vec3 modelPosition{};
        glm::vec3 modelSize{ 1.0, 1.0, 1.0 };
        glm::vec3 lightPosition{ 3.0f, 0.0f, -5.0f };

        float alphaValue = 1.0f;
        bool lines{ false };

        shader.setUniformVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        shader.setUniformVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        shader.setUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setUniformFloat("material.shininess", 64.0f);

        while (!window.shouldClose()) {
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
                ImGui::DragFloat3("Rotation", glm::value_ptr(rotationAngles), 0.1f, 0.0f, 360.0f);
                ImGui::DragFloat3("Coordinates", glm::value_ptr(modelPosition), 0.01f, -100.0f, 100.0f);
                ImGui::DragFloat3("Size", glm::value_ptr(modelSize), 0.01f, 0.0f, 20.0f);
                ImGui::Text("Light block Settings");
                ImGui::DragFloat3("Position", glm::value_ptr(lightPosition), 0.1f, -100.0f, 100.0f);
                ImGui::Text("Framerate: %.1f FPS)", ImGui::GetIO().Framerate);
                ImGui::Checkbox("Enable wireframe", &lines);
                ImGui::End();
            }

            if (lines) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            glClearColor(bgColor.x, bgColor.y, bgColor.z, alphaValue);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Update model matrix. translate, scale, rotate
            glm::mat4 model{ glm::mat4(1.0f) };
            glm::vec3 tr{glm::vec3(modelPosition[0], modelPosition[1], modelPosition[2]) };
            glm::vec3 resizing{ glm::vec3(modelSize[0], modelSize[1], modelSize[2]) };
            model = glm::translate(model, tr);
            model = glm::scale(model, resizing);
            model = glm::rotate(model, rotationAngles[0], glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::rotate(model, rotationAngles[1], glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, rotationAngles[2], glm::vec3(1.0f, 0.0f, 0.0f));

            // view/projection transformations
            camera.updateProjection(window.getWidth(), window.getHeight());

            if (window.isMouseButtonDown(GLFW_MOUSE_BUTTON_2)) {
                camera.move(window.getCursorPosition());
            }
            camera.lookAround(window, glm::vec3(0.0f, 0.0f, 0.0f));

            shader.setUniformVec3("light.position", glm::vec3(lightPosition[0], lightPosition[1], lightPosition[2]));
            shader.setUniformVec3("viewPos", camera.getPosition());
            shader.setUniformMat4("model", model);
            shader.setUniformMat4("projection", camera.getProjection());
            shader.setUniformMat4("view", camera.getView());

            objModel.draw(shader);

            // window render and gui render
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            window.draw();

        }
    }

    auto shutDown()  -> void override {

    }
private:
    static auto initImGui(kT::Window& window) -> void {
        // Initialize ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window.getWindowPointer(), true);

        // NOTE: this has to be the same as glMajor and glMinor variables in Window class
        // needs rework to be done automatically
        ImGui_ImplOpenGL3_Init("#version 410");
        ImGui::StyleColorsClassic();

        // Window softness (rounding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);

        ImGuiIO& io = ImGui::GetIO();

        // can C-Style void cast "io" to, i.e. (void)io in case of compiler warning about unused variable
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    }

    kT::Window window{ "ModelLoading", 1280, 720 };
    kT::Camera camera{ window };
    kT::Shader shader{ "../assets/shaders/defaultVertex.glsl", "../assets/shaders/defaultFragment.glsl" };
    kT::Model objModel{ "../assets/models/backpack/backpack.obj" };

};

int main(int, char**) {
    auto loader{ std::make_unique<ModelLoader>() };

    loader->startUp();
    loader->run();
    loader->shutDown();

    return 0;
}