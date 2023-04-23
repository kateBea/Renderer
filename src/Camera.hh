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

    // CAMERA IMPLEMENTATION
    inline auto Camera::update(const Kate::Window& window) -> glm::mat4 {
        handleInput(window);

        // camera direction
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 cameraDirection = glm::normalize(m_CameraPos - cameraTarget);

        // positive x-axis of the camera
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

        return glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
    }

    auto Camera::handleInput(const Window &window) -> void {
        const float cameraSpeed{ 2.5f * window.getDeltaTime() };

        if (window.isKeyPressed(GLFW_KEY_W))
            m_CameraPos += cameraSpeed * m_CameraFront;

        if (window.isKeyPressed(GLFW_KEY_S))
            m_CameraPos -= cameraSpeed * m_CameraFront;

        if (window.isKeyPressed(GLFW_KEY_A))
            m_CameraPos -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;

        if (window.isKeyPressed(GLFW_KEY_D))
            m_CameraPos += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
    }

    Camera::Camera() noexcept
        :   m_CameraPos{ glm::vec3(0.0f, 0.0f,  3.0f) }
        ,   m_CameraFront{ glm::vec3(0.0f, 0.0f, -1.0f) }
        ,   m_CameraUp{ glm::vec3(0.0f, 1.0f,  0.0f) }
    {

    }

    auto Camera::showCameraPos() const -> void {
        std::cout << "Camera at pos: [" << m_CameraPos[0];
        std::cout << ',' << m_CameraPos[0] << ',' << m_CameraPos[0] << "]\n";
    }

}
#endif // END CAMERA_HH
