//
// Created by zanet on 4/25/2023.
//

#include <iostream>

#include <InputManager.hh>

namespace Kate {
    // IMPLEMENTATION
    InputManager::InputManager(GLFWwindow* window) {
        startUp(window);

    }

    auto InputManager::getMousePos() const -> InputManager::pos_t {
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
            glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
                auto input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
                input->m_Keys[key] = action == GLFW_PRESS;
            });

            glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
                auto input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
                input->m_MousePos.first = x;
                input->m_MousePos.second = y;
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