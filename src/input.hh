/**
 * @file input.hh
 * @author kate
 * @brief Defines the input interface
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

namespace kate {
    class input {
    public:
        using pos_t = std::pair<double, double>;

        explicit input(GLFWwindow* window);
        auto get_mouse_pos() -> pos_t;
        auto is_mouse_button_down(std::int32_t button) -> bool;
        auto is_key_down() -> bool;


    private:
        pos_t m_mouse_pos{};
        std::array<bool, GLFW_KEY_LAST> m_keys{};
        std::array<bool, GLFW_MOUSE_BUTTON_LAST> m_mouse_keys{};
    };

    // IMPLEMENTATION
    inline input::input(GLFWwindow *window) {

    }

    inline auto input::get_mouse_pos() -> input::pos_t {
        return kate::input::pos_t();
    }

    inline auto input::is_mouse_button_down(std::int32_t button) -> bool {
        return false;
    }

    inline auto input::is_key_down() -> bool {
        return false;
    }
}

#endif // END INPUT_HH
