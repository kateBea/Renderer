/**
 * @file parser.hh
 * @author kate
 * @brief Defines the window manager
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
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace kate {
    class window {
    public:
        window(std::string_view window_name = "renderer", std::int32_t width = 1280, std::int32_t height = 720);

        window(const window&)               = delete;
        window& operator=(const window&)    = delete;

        window(window&&)                    = delete;
        window& operator=(window&&)         = delete;

        auto show_cursor_pos() -> void;

        [[nodiscard]]
        auto should_close() -> bool;

        auto resize() -> void;
        auto draw() -> void;
        ~window();
    private:
        auto start_up() -> bool;
        auto shutdown() -> void;
        GLFWwindow*     m_window{};
        std::string     m_name{};
        std::int32_t    m_width{};
        std::int32_t    m_height{};
    };


    // WINDOW CLASS IMPLEMENTATION
    inline window::window(std::string_view window_name, std::int32_t width, std::int32_t height)
        :   m_window{}, m_name{ window_name }, m_width{ width }, m_height{ height }
    {
        start_up();
    }

    inline auto window::start_up() -> bool {
        // Init GLFW Library
        if (glfwInit() == GL_FALSE) {
            std::cerr << "Failed to initialize the GLFW library...\n";
            return false;
        }

        // Set OpenGL major and minor version numbers for OpenGL 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

        return true;
    }

    inline auto window::shutdown() -> void {
        //glfwDestroyWindow(this->m_window);
        glfwTerminate();
    }

    inline auto window::draw() -> void {
        glfwPollEvents();
        glfwSwapBuffers(this->m_window);
    }

    inline window::~window() {
        shutdown();
    }

    inline auto window::should_close() -> bool {
        return glfwWindowShouldClose(this->m_window);
    }

    inline auto window::show_cursor_pos() -> void {
        double x_pos{};
        double y_pos{};

        glfwGetCursorPos(this->m_window, &x_pos, &y_pos);
        std::printf("mouse cursor at position: [%f, %f]\n", x_pos, y_pos);
    }

    inline auto window::resize() -> void {
        std::int32_t width{};
        std::int32_t height{};
        glfwGetFramebufferSize(m_window, &width, &height);
        glViewport(0, 0, width, height);
    }
}
#endif // END WINDOW_HH
