/**
 * @file InputManager.hh
 * @author Kate
 * @brief Defines the InputManager interface
 * @version 1.0
 * @date 2023-03-23
 */

#ifndef INPUT_HH
#define INPUT_HH

// C++ Standard Library
#include <cstdint>
#include <array>
#include <cstdio>

// Third-Party Libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Project Libraries
#include "Utils.hh"

namespace Kate {
    class InputManager {
    public:
        explicit InputManager() = default;

        explicit InputManager(GLFWwindow* window);

        [[nodiscard]]
        auto getMousePos() const -> std::pair<double, double>;

        [[nodiscard]]
        auto isMouseButtonDown(std::int32_t button) const -> bool;

        [[nodiscard]]
        auto isKeyDown(std::int32_t key) const -> bool;

        [[nodiscard]]
        auto getScrollOffset() const -> const std::pair<double, double>&;

        auto startUp(GLFWwindow* window) -> void;


    private:
        std::pair<double, double> m_MouseScroll{};
        std::pair<double, double> m_MousePos{};
        std::array<bool, GLFW_KEY_LAST> m_Keys{};
        std::array<bool, GLFW_MOUSE_BUTTON_LAST> m_MouseKeys{};
    };
}

#endif // END INPUT_HH
