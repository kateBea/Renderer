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
#include <utility>

// Third-Party Libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Project Libraries
#include "InputManager.hh"

namespace Kate {
    class Window {
    public:
        explicit Window(std::string_view window_name = "Renderer", std::int32_t width = 640, std::int32_t height = 480);

        Window(const Window&)               = delete;
        Window& operator=(const Window&)    = delete;

        Window(Window&&)                    = delete;
        Window& operator=(Window&&)         = delete;

        /**
         * Prints to standard output the current position
         * of the cursor in this Window
         * */
        auto showCursorPos() -> void;

        /**
         * Returns true if the user attempts to close this Window,
         * returns false otherwise
         * @return true if window should close, false otherwise
         * */
        [[nodiscard]]
        auto shouldClose() -> bool;

        [[nodiscard]]
        auto isKeyPressed(std::int32_t key) const -> bool;

        [[nodiscard]]
        auto getMouseScrollOffset() const -> const std::pair<double, double>&;

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

        // shouldn't be in the window class
        static auto enableWireframeMode() -> void;
        static auto disableWireframeMode() -> void;

        [[nodiscard]] auto getWindowPointer() const -> GLFWwindow*;

        [[nodiscard]] auto getCursorPosition() const -> std::pair<double, double>;

        auto isMouseButtonDown(std::int32_t button) -> bool;

        auto draw() -> void;

        ~Window();
    private:
        static constexpr std::int32_t glMajor{ 4 };
        static constexpr std::int32_t glMinor{ 1 };

        auto startUp() -> void;
        auto shutdown() -> void;

        /**
         * Updates the time passed between frames
         * */
        auto updateDeltaTime() -> void;

        static auto enableDepthTesting() -> void;
        static auto setupGlfwHints() -> void;
        auto setupGlfw() -> void;
        auto setupGlew() -> void;

        float m_DeltaTime{};	// Time between current frame and last frame
        float m_LastFrame{}; // Time of last frame

        GLFWwindow*         m_window{};
        std::string         m_name{};
        std::int32_t        m_width{};
        std::int32_t        m_height{};
        Kate::InputManager  m_Input{};
    };
}
#endif // END WINDOW_HH
