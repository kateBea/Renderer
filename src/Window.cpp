#include "../include/Window.hh"

namespace Kate {
    Window::Window(std::string_view window_name, std::int32_t width, std::int32_t height)
            :   m_window{}, m_name{ window_name }, m_width{ width }, m_height{ height }, m_Input{}, m_DeltaTime{}, m_LastFrame{}
    {
        startUp();
    }

    auto Window::startUp() -> void {
        setupGlfw();
        setupGlew();
        m_Input.startUp(m_window);

        // ------------------------------- Set necessary callbacks ------------------------------------

        // This callback is set to automatically adjust the window width and height when we resize the
        // window and adjust the viewport to the current frame buffer dimensions
        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)-> void {
            auto ptr{ static_cast<Window*>(glfwGetWindowUserPointer(window)) };
            ptr->m_width = width;
            ptr->m_height = height;
            glViewport(0, 0, width, height);
        });
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
        glfwSwapBuffers(this->m_window);
    }

    Window::~Window() {
        shutdown();
    }

    auto Window::shouldClose() -> bool {
        return glfwWindowShouldClose(this->m_window);
    }

    auto Window::showCursorPos() -> void {
        auto temp{ m_Input.getMousePos() };
        std::printf("mouse cursor at position: [%f, %f]\n", temp.first, temp.second);
    }

    auto Window::isKeyPressed(std::int32_t key) const -> bool {
        return m_Input.isKeyDown(key);
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
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Kate::GLMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Kate::GLMinor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    }

    auto Window::getWindowPointer() const -> GLFWwindow * {
        return m_window;
    }

    auto Window::isMouseButtonDown(std::int32_t button) -> bool {
        return m_Input.isMouseButtonDown(button);
    }

    auto Window::getCursorPosition() const -> std::pair<double, double> {
        return m_Input.getMousePos();
    }

    auto Window::getMouseScrollOffset() const -> const std::pair<double, double> & {
        return m_Input.getScrollOffset();
    }

}