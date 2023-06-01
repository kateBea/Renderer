/**
 * @file InputManager.hh
 * @author kT
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
#include <Core/Common.hh>

namespace kT {
    class InputManager {
    public:
        using Pair_T = std::pair<double, double>;

        template<std::size_t size_>
        using ArrayBool_T = std::array<bool, size_>;

        explicit InputManager(GLFWwindow* window);

        [[nodiscard]]
        static auto getMousePos() -> Pair_T { return s_Instance->_getMousePos(); }

        [[nodiscard]]
        static auto isMouseButtonDown(std::int32_t button) -> bool { return s_Instance->_isMouseButtonDown(button); }

        [[nodiscard]]
        static auto isKeyDown(std::int32_t key) -> bool { return s_Instance->_isKeyDown(key); }

        [[nodiscard]]
        static auto getScrollOffset() -> Pair_T { return s_Instance->_getScrollOffset(); }

    private:
        [[nodiscard]]
        auto _getMousePos() const -> Pair_T;

        [[nodiscard]]
        auto _isMouseButtonDown(std::int32_t button) const -> bool;

        [[nodiscard]]
        auto _isKeyDown(std::int32_t key) const -> bool;

        [[nodiscard]]
        auto _getScrollOffset() const -> Pair_T;
        auto startUp(GLFWwindow* window) -> void;

        struct InputData {
            Pair_T mouseScroll{};
            Pair_T mousePos{};
            ArrayBool_T<GLFW_KEY_LAST> keys{};
            ArrayBool_T<GLFW_MOUSE_BUTTON_LAST> mouseKeys{};
        };

        inline static InputManager* s_Instance{ nullptr };

        InputData m_Data{};
    };
}

#endif // END INPUT_HH
