#include <Core/Window.hh>

namespace kT {
    auto Window::StartUp(std::string_view window_name, std::int32_t width, std::int32_t height) -> void {
        m_name = window_name;
        m_width = width;
        m_height = height;

        setupGlfw();
        setupGlew();

        // This callback is set to automatically adjust the window width and height when we resize the
        // window and adjust the viewport to the current frame buffer dimensions
        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)-> void {
            auto ptr{ static_cast<Window*>(glfwGetWindowUserPointer(window)) };
            ptr->m_width = width;
            ptr->m_height = height;
            glViewport(0, 0, width, height);
        });
    }

    auto Window::startUp() -> void {

    }

    auto Window::shutdown() -> void {
        glfwDestroyWindow(this->m_window);
        glfwTerminate();
    }

    auto Window::draw() -> void {
        // Process queued events
        glfwPollEvents();

        // Update delta time
        updateDeltaTime();

        // Swap back and front buffers
        glfwSwapBuffers(m_window);
    }

    Window::~Window() {
        shutdown();
    }

    auto Window::shouldClose() -> bool {
        return glfwWindowShouldClose(this->m_window);
    }

    auto Window::getDeltaTime() const -> float {
        return m_DeltaTime;
    }

    auto Window::updateDeltaTime() -> void {
        float currentFrame{ static_cast<float>(glfwGetTime()) };
        m_DeltaTime = currentFrame - m_LastFrame;
        m_LastFrame = currentFrame;
    }

    auto Window::getWidth() const -> std::int32_t {
        return m_width;
    }

    auto Window::getHeight() const -> std::int32_t {
        return m_height;
    }

    auto Window::setupGlfw() -> void {
        // Init GLFW Library
        if (glfwInit() == GL_FALSE)
            throw std::runtime_error("Failed to initialize the GLFW library");

        setupGlfwHints();

        // Create valid context
        m_window = glfwCreateWindow(m_width, m_height, m_name.data(), nullptr, nullptr);

        if (m_window == nullptr)
            throw std::runtime_error("There was an error creating the Window");
    }

    auto Window::setupGlew() -> void {
        glfwMakeContextCurrent(this->m_window);
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
            throw std::runtime_error("Failed to initialize GLEW");

    }

    auto Window::setupGlfwHints() -> void {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, kT::GLMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, kT::GLMinor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    }

    auto Window::getWindowPointer() const -> GLFWwindow* {
        return m_window;
    }

}