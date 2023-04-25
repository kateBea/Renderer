//
// Created by zanet on 4/25/2023.
//

#include <Camera.hh>

namespace Kate {
    // CAMERA IMPLEMENTATION
    auto Camera::update(const Kate::Window& window) -> glm::mat4 {
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