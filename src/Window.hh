/**
 * @file parser.hh
 * @author Kate
 * @brief Defines the Window manager
 * @version 1.0
 * @date 2023-03-22
 */


#ifndef WINDOW_HH
#define WINDOW_HH

// C++ Standard Library
#include <iostream>
#include <cstdio>
#include <string_view>
#include <cstddef>

// Third-Party Libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Project Libraries
#include <InputManager.hh>

namespace Kate {
    class Window {
    public:
        explicit Window(std::string_view window_name = "Renderer", std::int32_t width = 1280, std::int32_t height = 720);

        Window(const Window&)               = delete;
        Window& operator=(const Window&)    = delete;

        Window(Window&&)                    = delete;
        Window& operator=(Window&&)         = delete;

        auto showCursorPos() -> void;

        [[nodiscard]]
        auto shouldClose() -> bool;

        [[nodiscard]]
        auto isKeyPressed(std::int32_t key) const -> bool;

        [[nodiscard]]
        auto getDeltaTime() const -> float;

        auto updateDeltaTime() -> void;

        auto resize() -> void;
        auto draw() -> void;
        ~Window();
    private:
        static constexpr std::int32_t glMajor{ 4 };
        static constexpr std::int32_t glMinor{ 1 };

        auto startUp() -> bool;
        auto shutdown() -> void;

        float m_DeltaTime{};	// Time between current frame and last frame
        float m_LastFrame{}; // Time of last frame

        GLFWwindow*     m_window{};
        std::string     m_name{};
        std::int32_t    m_width{};
        std::int32_t    m_height{};
        Kate::InputManager m_Input{};
    };


    // WINDOW CLASS IMPLEMENTATION
    inline Window::Window(std::string_view window_name, std::int32_t width, std::int32_t height)
        :   m_window{}, m_name{ window_name }, m_width{ width }, m_height{ height }, m_Input{}, m_DeltaTime{}, m_LastFrame{}
    {
        startUp();
    }

    inline auto Window::startUp() -> bool {
        // Init GLFW Library
        if (glfwInit() == GL_FALSE) {
            std::cerr << "Failed to initialize the GLFW library...\n";
            return false;
        }

        // Set OpenGL major and minor version numbers for OpenGL 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMinor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

        // Create valid context
        m_window = glfwCreateWindow(m_width, m_height, m_name.data(), nullptr, nullptr);

        if (m_window == nullptr) {
            std::cerr << "There was an error creating the Window\n";
            return false;
        }

        // Create a valid context for initializing GLEW
        glfwMakeContextCurrent(this->m_window);

        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            std::cerr << "Failed to initialize GLEW!\n";
            return false;
        }

        // initialize the inputManager once everything is ok
        m_Input.startUp(m_window);
        return true;
    }

    inline auto Window::shutdown() -> void {
        glfwDestroyWindow(this->m_window);
        glfwTerminate();
    }

    inline auto Window::draw() -> void {
        glfwPollEvents();
        glfwSwapBuffers(this->m_window);
    }

    inline Window::~Window() {
        shutdown();
    }

    inline auto Window::shouldClose() -> bool {
        return glfwWindowShouldClose(this->m_window);
    }

    inline auto Window::showCursorPos() -> void {
        auto temp{ m_Input.getMousePos() };

        double x_pos{ temp.first };
        double y_pos{ temp.second };

        // glfwGetCursorPos(this->m_Window, &x_pos, &y_pos);
        std::printf("mouse cursor at position: [%f, %f]\n", x_pos, y_pos);
    }

    inline auto Window::resize() -> void {
        std::int32_t width{};
        std::int32_t height{};
        glfwGetFramebufferSize(m_window, &width, &height);
        glViewport(0, 0, width, height);
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
}
#endif // END WINDOW_HH
