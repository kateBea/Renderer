#include <Core/InputManager.hh>

namespace kT {
    InputManager::InputManager(GLFWwindow* window) {
        s_Instance = this;
        startUp(window);
    }

    auto InputManager::_getMousePos() const -> Pair_T {
        return m_Data.mousePos;
    }

    auto InputManager::_isMouseButtonDown(std::int32_t button) const -> bool {
        return m_Data.mouseKeys[button];
    }

    auto InputManager::_isKeyDown(std::int32_t key) const -> bool {
        return m_Data.keys[key];
    }

    auto InputManager::startUp(GLFWwindow *window) -> void {
        glfwSetWindowUserPointer(window, &m_Data);

        if (window != nullptr) {
            // Keys callback
            glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
                auto input{ static_cast<InputData*>(glfwGetWindowUserPointer(window)) };
                input->keys[key] = action == GLFW_PRESS;
            });

            // Cursor movement callback
            glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
                auto input{ static_cast<InputData*>(glfwGetWindowUserPointer(window)) };
                input->mousePos.first = x;
                input->mousePos.second = y;
            });

            // Mouse buttons callback
            glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
                auto input{ static_cast<InputData*>(glfwGetWindowUserPointer(window)) };
                input->mouseKeys[button] = action == GLFW_PRESS;
            });

            // Mouse scroll callback
            glfwSetScrollCallback(window, [](GLFWwindow* window, double xScroll, double yScroll) {
                auto input{ static_cast<InputData*>(glfwGetWindowUserPointer(window)) };
                input->mouseScroll.first = xScroll;
                input->mouseScroll.second = yScroll;
            });
        }
    }

    auto InputManager::_getScrollOffset() const -> Pair_T {
        return m_Data.mouseScroll;
    }
}