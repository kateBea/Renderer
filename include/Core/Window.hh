/**
 * @file parser.hh
 * @author kT
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
        Window(std::string_view window_name = "Renderer", std::int32_t width = 1280, std::int32_t height = 720);

        Window(const Window&)               = delete;
        Window& operator=(const Window&)    = delete;

        Window(Window&&)                    = delete;
        Window& operator=(Window&&)         = delete;

        /**
         * Returns true if the user attempts to close this Window,
         * returns false otherwise
         * @return true if window should close, false otherwise
         * */
        [[nodiscard]]
        auto shouldClose() -> bool;

        [[nodiscard]]
        auto getDeltaTime() const -> float;

        /**
         * Returns the width of this Window
         * @return Value of width
         * */
        [[nodiscard]]
        auto getWidth() const -> std::int32_t;

        /**
         * Returns the height of this Window
         * @return Value of height
         * */
        [[nodiscard]]
        auto getHeight() const -> std::int32_t;

        [[nodiscard]] auto getWindowPointer() const -> GLFWwindow*;

        auto draw() -> void;

        ~Window();
    private:
        auto startUp() -> void;
        auto shutdown() -> void;

        /**
         * Updates the time passed between frames
         * */
        auto updateDeltaTime() -> void;

        static auto setupGlfwHints() -> void;
        auto setupGlfw() -> void;
        auto setupGlew() -> void;

        float m_DeltaTime{};	// Time between current frame and last frame
        float m_LastFrame{}; // Time of last frame

        GLFWwindow*         m_window{};
        std::string         m_name{};
        std::int32_t        m_width{};
        std::int32_t        m_height{};
    };
}
#endif // END WINDOW_HH
