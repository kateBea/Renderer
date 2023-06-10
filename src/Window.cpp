#include <Core/Window.hh>
#include <OpenGL/Renderer.hh>

namespace kT {
    auto Window::StartUp(std::string_view title, std::int32_t width, std::int32_t height) -> void {
        m_Title = title;
        m_Width = width;
        m_Height = height;

        InitGLFW();
        InitGLEW();

        // This callback is set to automatically adjust the window width and height when we resize the
        // window and adjust the viewport to the current frame buffer dimensions
        glfwSetFramebufferSizeCallback(m_Window,
             [](GLFWwindow* window, int width, int height)-> void {
                 auto ptr{ static_cast<Window*>(glfwGetWindowUserPointer(window)) };
                 ptr->m_Width = width;
                 ptr->m_Height = height;
                 Renderer::ResetViewport(width, height);
            }
        );
    }

    auto Window::Init() -> void {

    }

    auto Window::ShutDown() -> void {
        glfwDestroyWindow(this->m_Window);
        glfwTerminate();
    }

    auto Window::SwapBuffers() -> void {
        // Process queued events
        glfwPollEvents();

        // Update delta time
        updateDeltaTime();

        // Swap back and front buffers
        glfwSwapBuffers(m_Window);
    }

    Window::~Window() {
        ShutDown();
    }

    auto Window::ShouldClose() -> bool {
        return glfwWindowShouldClose(this->m_Window);
    }

    auto Window::updateDeltaTime() -> void {
        float currentFrame{ static_cast<float>(glfwGetTime()) };
        m_DeltaTime = currentFrame - m_LastFrame;
        m_LastFrame = currentFrame;
    }

    auto Window::InitGLFW() -> void {
        if (glfwInit() == GL_FALSE)
            throw std::runtime_error("Failed to initialize the GLFW library");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, kT::GLMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, kT::GLMinor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.data(), nullptr, nullptr);

        if (m_Window == nullptr)
            throw std::runtime_error("There was an error creating the Window");
    }

    auto Window::InitGLEW() -> void {
        glfwMakeContextCurrent(this->m_Window);
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
            throw std::runtime_error("Failed to initialize GLEW");

    }

}