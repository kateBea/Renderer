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
#include <cstddef>
#include <array>

// Third-Party Libraries
#include <GLFW/glfw3.h>

// Project Libraries
#include <utils.hh>

namespace Kate {
    class InputManager {
    public:
        /*
         * Represents a mouse positions, first component is x coordinate, second is y
         * */
        using pos_t = std::pair<double, double>;

        explicit InputManager() = default;

        explicit InputManager(GLFWwindow* window);

        [[nodiscard]]
        auto getMousePos() const -> pos_t;

        [[nodiscard]]
        auto isMouseButtonDown(std::int32_t button) const -> bool;

        [[nodiscard]]
        auto isKeyDown(std::int32_t key) const -> bool;

        auto startUp(GLFWwindow* window) -> void;


    private:
        pos_t m_MousePos{};
        std::array<bool, GLFW_KEY_LAST> m_Keys{};
        std::array<bool, GLFW_MOUSE_BUTTON_LAST> m_MouseKeys{};
    };

    // IMPLEMENTATION
    inline InputManager::InputManager(GLFWwindow* window) {
        startUp(window);

    }

    inline auto InputManager::getMousePos() const -> InputManager::pos_t {
        return m_MousePos;
    }

    inline auto InputManager::isMouseButtonDown(std::int32_t button) const -> bool {
        return m_MouseKeys[button];
    }

    inline auto InputManager::isKeyDown(std::int32_t key) const -> bool {
        return m_Keys[key];
    }

    auto InputManager::startUp(GLFWwindow *window) -> void {
        if (window != nullptr) {
            glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
                auto input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
                input->m_Keys[key] = action == GLFW_PRESS;
            });
            glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
                auto input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
                input->m_MousePos.first = xpos;
                input->m_MousePos.second = ypos;
            });
            glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
                auto input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
                input->m_MouseKeys[button] = action == GLFW_PRESS;
            });
            glfwSetWindowUserPointer(window, this);
        }
        else
            std::cerr << "Could initialize InputManager. window == nullptr ...\n";
    }
}

#endif // END INPUT_HH
