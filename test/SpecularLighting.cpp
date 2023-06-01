// C++ Standard Library
#include <vector>

// Third-Party Libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

// Project Libraries
#include "OpenGL/Mesh.hh"
#include "OpenGL/Camera.hh"
#include "OpenGL/Shader.hh"
#include "Core/Window.hh"
#include "OpenGL/BasicLighting.hh"
#include "OpenGL/Vao.hh"
#include "OpenGL/Vbo.hh"
#include "Core/Common.hh"
#include "Core/Logger.hh"


// GLOBALS
// Warning: C++ does not guarantee globals to be
// initialized in the order they are declared
static kT::Window        g_Window{"SpecularLighting", 1280, 720 };
static kT::Camera        g_Camera{g_Window };

static kT::Vao           g_Vao{};
static kT::Vbo           g_Vbo{};

static kT::BasicLighting g_Light{};
static kT::Texture g_Texture{Texture::TextureType::SPECULAR, 0, 0};
static kT::Texture g_TextureSpec{Texture::TextureType::SPECULAR, 0, 0};
static kT::Shader        g_LightShader{};
static kT::Shader        g_DefaultShaders{};

auto run() -> void;
auto startUp() -> void;
auto initImGui() -> void;
auto setupLayout() -> void;
auto setOpenGLHints() -> void;


int main(int, char**) {
    kT::Logger::Init();

    KATE_LOGGER_ERROR("Error message");
    KATE_LOGGER_WARN("Warn message");
    KATE_LOGGER_CRITICAL("Critical message");
    KATE_LOGGER_TRACE("Trace message");
    KATE_LOGGER_INFO("Info message");
    KATE_LOGGER_DEBUG("Debug message");

    startUp();
    run();
    return 0;
}

auto run() -> void {
    ImVec4 bgColor = ImVec4(0.254f, 0.083f, 0.144f, 1.00f);
    glm::vec3 rotationAngles{};
    glm::vec3 modelPosition{};
    glm::vec3 modelSize{ 1.0, 1.0, 1.0 };

    glm::vec3 lightPosition{ 3.0f, 0.0f, -5.0f };
    glm::vec3 lightColor{ 255.0f, 255.0f, 255.0f };

    float alphaValue = 1.0f;

    g_DefaultShaders.setUniformInt("material.diffuse", 0);

    while (!g_Window.shouldClose()) {
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
            ImGui::DragFloat3("Position", glm::value_ptr(lightPosition), 0.01f, -100.0f, 100.0f);
            ImGui::DragFloat3("Color", glm::value_ptr(lightColor), 1.0f, 0.0f, 255.0f);
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
            g_Camera.updateProjection(g_Window.getWidth(), g_Window.getHeight());
            if (g_Window.isMouseButtonDown(GLFW_MOUSE_BUTTON_2))
                g_Camera.move(g_Window.getCursorPosition());
            g_Camera.lookAround(g_Window, glm::vec3(0.0f, 0.0f, 0.0f));

            g_DefaultShaders.use();
            g_DefaultShaders.setUniformVec3("light.position", glm::vec3(lightPosition[0], lightPosition[1], lightPosition[2]));
            g_DefaultShaders.setUniformVec3("viewPos", g_Camera.getPosition());

            // light properties
            g_DefaultShaders.setUniformVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
            g_DefaultShaders.setUniformVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
            g_DefaultShaders.setUniformVec3("light.specular", glm::vec4(lightColor[0] / 255, lightColor[1] / 255, lightColor[2] / 255, 1.0f));

            // material properties
            g_DefaultShaders.setUniformVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
            g_DefaultShaders.setUniformFloat("material.shininess", 64.0f);

            // Pass mvp matrices to the default shader
            g_DefaultShaders.setUniformMat4("model", model);
            g_DefaultShaders.setUniformMat4("view", g_Camera.getView());
            g_DefaultShaders.setUniformMat4("projection", g_Camera.getProjection());

            g_DefaultShaders.setUniformInt("material.specular", 1);

            g_Texture.bind();
            kT::Texture::bindUnit(0);

            g_TextureSpec.bind();
            kT::Texture::bindUnit(1);

            // render objects (this should be done by the model class)
            g_DefaultShaders.use();
            g_Vbo.bind();
            g_Vao.bind();

            // NOTE: 36 because we have 6 faces to render with 2 triangles
            // each and 3 vertices per triangles, same goes for the light cube
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // RENDER CUBE LIGHT
        {
            g_LightShader.use();

            glm::mat4 lightBlock{ glm::mat4(1.0f) };
            glm::vec3 translation{ glm::vec3(lightPosition[0], lightPosition[1], lightPosition[2]) };
            lightBlock = glm::translate(lightBlock, translation);

            g_LightShader.setUniformMat4("model", lightBlock);
            g_LightShader.setUniformMat4("view", g_Camera.getView());
            g_LightShader.setUniformMat4("projection", g_Camera.getProjection());

            g_LightShader.setUniformVec4("lightColor", glm::vec4(lightColor[0] / 255, lightColor[1] / 255, lightColor[2] / 255, 1.0f));
            g_Light.bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // window render and gui render
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        g_Window.draw();
    }
}

auto startUp() -> void {
    // Setup OpenGL Rendering
    setOpenGLHints();


    // Initialize the GUI library
    initImGui();

    // setup vertices
    auto v_pos{ kT::parseVerticesFile("../assets/vertices") };

    // this part is not necessary as we can use the same vertex positions from the model
    // and simply specify a different layout for the cube light
    auto lightPos{ kT::parseVerticesFile("../assets/vertices") };

    g_Light.load(lightPos);
    g_Vbo.load(v_pos);

    g_DefaultShaders.load("../assets/shaders/defaultVertex.glsl", "../assets/shaders/defaultFragment.glsl");
    g_LightShader.load("../assets/shaders/lightVertex.glsl", "../assets/shaders/lightFragment.glsl");
    g_Texture.load("../assets/textures/container2.png");
    g_TextureSpec.load("../assets/textures/container2_specular.png");

    setupLayout();
}

auto setupLayout() -> void {
    // Vertex position attribute
    g_Vbo.bind();
    g_Vao.layout(0, 3, kT::Vao::Attribute::POSITION);

    // Vertex Normals attribute
    g_Vbo.bind();
    g_Vao.layout(1, 3, kT::Vao::Attribute::NORMAL);

    // Vertex Texture attribute
    g_Vbo.bind();
    g_Vao.layout(2, 2, kT::Vao::Attribute::TEXTURE);
}

auto initImGui() -> void {
    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(g_Window.getWindowPointer(), true);

    // NOTE: this has to be the same as glMajor and glMinor variables in Window class
    // needs rework to be done automatically
    ImGui_ImplOpenGL3_Init(fmt::format("#version {}{}0", kT::GLMajor, kT::GLMinor).c_str());
    ImGui::StyleColorsClassic();

    // Window softness (rounding)
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);

    ImGuiIO& io = ImGui::GetIO();

    // can C-Style void cast "io" to, i.e. (void)io in case of compiler warning about unused variable
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
}

auto setOpenGLHints() -> void {
    glEnable(GL_DEPTH_TEST);
}