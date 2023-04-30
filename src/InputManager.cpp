#include "../include/InputManager.hh"

namespace Kate {
    InputManager::InputManager(GLFWwindow* window) {
        startUp(window);

    }

    auto InputManager::getMousePos() const -> std::pair<double, double> {
        return m_MousePos;
    }

    auto InputManager::isMouseButtonDown(std::int32_t button) const -> bool {
        return m_MouseKeys[button];
    }

    auto InputManager::isKeyDown(std::int32_t key) const -> bool {
        return m_Keys[key];
    }

    auto InputManager::startUp(GLFWwindow *window) -> void {
        if (window != nullptr) {
            // Keys callback
            glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
                auto input{ static_cast<InputManager*>(glfwGetWindowUserPointer(window)) };
                input->m_Keys[key] = action == GLFW_PRESS;
            });

            // Cursor movement callback
            glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
                auto input{ static_cast<InputManager*>(glfwGetWindowUserPointer(window)) };
                input->m_MousePos.first = x;
                input->m_MousePos.second = y;
            });

            // Mouse buttons callback
            glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
                auto input{ static_cast<InputManager*>(glfwGetWindowUserPointer(window)) };
                input->m_MouseKeys[button] = action == GLFW_PRESS;
            });

            // Mouse scroll callback
            glfwSetScrollCallback(window, [](GLFWwindow* window, double xScroll, double yScroll) {
                auto input{ static_cast<InputManager*>(glfwGetWindowUserPointer(window)) };
                input->m_MouseScroll.first = xScroll;
                input->m_MouseScroll.second = yScroll;
            });

            glfwSetWindowUserPointer(window, this);
        }
        else
            std::printf("Could initialize InputManager. window == nullptr ...\n");
    }

    auto InputManager::getScrollOffset() const -> const std::pair<double, double>& {
        return m_MouseScroll;
    }
}