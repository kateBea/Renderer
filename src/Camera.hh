/**
 * @file Camera.hh
 * @author Kate
 * @brief Defines the Camera class
 * @version 1.0
 * @date 2023-03-16
 */

#ifndef CAMERA_HH
#define CAMERA_HH

// C++ Standard Library
#include <iostream>
#include <cstddef>

// Third-Party Libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Project Libraries
#include <InputManager.hh>
#include <Window.hh>

namespace Kate {
    class Camera {
    public:
        explicit Camera() noexcept;

        Camera(const Camera&) = delete;
        Camera& operator=(const Camera&) = delete;

        Camera(Camera&&) = delete;
        Camera& operator=(Camera&&) = delete;

        [[nodiscard]]
        auto update(const Kate::Window& window) -> glm::mat4;

        auto showCameraPos() const -> void;

    private:
        glm::vec3 m_CameraPos{};
        glm::vec3 m_CameraFront{};
        glm::vec3 m_CameraUp{};

        auto handleInput(const Kate::Window& window) -> void;

    };
}
#endif // END CAMERA_HH
