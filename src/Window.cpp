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
        enableDepthTesting();
        m_Input.startUp(m_window);

        // Set necessary callbacks ----------------

        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)-> void {
            // math viewport to the current window size
            glViewport(0, 0, width, height);
        });
    }

    auto Window::shutdown() -> void {
        glfwDestroyWindow(this->m_window);
        glfwTerminate();
    }

    auto Window::draw() -> void {
        glfwPollEvents();
        updateDeltaTime();
        glfwGetFramebufferSize(m_window, &m_width, &m_width);
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

        double x_pos{ temp.first };
        double y_pos{ temp.second };

        // glfwGetCursorPos(this->m_Window, &x_pos, &y_pos);
        std::printf("mouse cursor at position: [%f, %f]\n", x_pos, y_pos);
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

    auto Window::enableDepthTesting() -> void {
        glEnable(GL_DEPTH_TEST);
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
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMinor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    }

    auto Window::enableWireframeMode() -> void {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    auto Window::disableWireframeMode() -> void {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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