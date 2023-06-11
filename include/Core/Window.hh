/**
 * @file parser.hh
 * @author kaTe
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
#include <utility>

// Third-Party Libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Project Libraries
#include <Core/InputManager.hh>
#include <Core/Common.hh>

namespace kT {
    class Window {
    public:
        Window() = default;

        auto StartUp(std::string_view title = "Renderer", std::int32_t width = 1280, std::int32_t height = 720) -> void;

        auto ShouldClose() -> bool;
        auto GetWidth() const -> std::int32_t { return m_Width; }
        auto GetHeight() const -> std::int32_t { return m_Height; }
        auto GetWindowPointer() const -> GLFWwindow* { return m_Window; }

        auto SwapBuffers() -> void;

        ~Window();
    private:
        // Forbidden operations
        Window(const Window&) = delete;
        Window(Window&&) = delete;

        auto operator=(const Window&) -> Window& = delete;
        auto operator=(Window&&) -> Window& = delete;
    private:
        auto Init() -> void;
        auto InitGLFW() -> void;
        auto InitGLEW() -> void;

        auto ShutDown() -> void;
    private:
        GLFWwindow* m_Window{};

        std::string m_Title{};
        std::int32_t m_Width{};
        std::int32_t m_Height{};
    };
}
#endif // END WINDOW_HH
