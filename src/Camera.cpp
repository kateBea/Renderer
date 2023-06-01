#include <OpenGL/Camera.hh>
#include <Core/InputManager.hh>

namespace kT {
    // CAMERA IMPLEMENTATION
    auto Camera::lookAround(const kT::Window& window, const glm::vec3& target) -> void {
        computeMatricesFromInput(window);

        // camera direction
        glm::vec3 cameraDirection = glm::normalize(m_CameraPos - target);

        // positive x-axis of the camera
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

        m_CameraUp = glm::cross(cameraDirection, cameraRight);
        m_View = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
    }

    auto Camera::computeMatricesFromInput(const kT::Window &window) -> void {
        const float cameraSpeed{ 2.5f * window.getDeltaTime() };

        if (InputManager::isKeyDown(GLFW_KEY_W))
            m_CameraPos += cameraSpeed * m_CameraFront;

        if (InputManager::isKeyDown(GLFW_KEY_S))
            m_CameraPos -= cameraSpeed * m_CameraFront;

        if (InputManager::isKeyDown(GLFW_KEY_A))
            m_CameraPos -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;

        if (InputManager::isKeyDown(GLFW_KEY_D))
            m_CameraPos += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
    }

    auto Camera::showCameraPos() const -> void {
        std::printf("Camera at pos: [%.3f %.3f %.3f]\n", m_CameraPos[0], m_CameraPos[1],  m_CameraPos[2]);
    }

    auto Camera::getView() const -> const glm::mat4& {
        return m_View;
    }

    auto Camera::move(const std::pair<double, double>& newCursorPosition) -> void {
        auto twoDimenLenght{
            [](std::pair<double, double> first, std::pair<double, double> second) -> double {
                return std::sqrt(
                        std::pow((first.first - second.first), 2) +
                        std::pow((first.second - second.second), 2)
                );
            }
        };

        // reversed since y-coordinates go from bottom to top
        auto offset{ std::make_pair(newCursorPosition.first - m_LastCursorPosition.first,
                                    m_LastCursorPosition.second - newCursorPosition.second) };

        // this fixes the camera jump by simply ignoring the offset in case the angle we have to rotate is too big
        // the threshold is determined by this magical number the result of twoDimenLenght() is compared to
        if (twoDimenLenght(newCursorPosition, m_LastCursorPosition) > 20.0f) {
            m_LastCursorPosition = newCursorPosition;
            return;
        }

        m_LastCursorPosition = newCursorPosition;

        offset.first *= m_Sensitivity;
        offset.second *= m_Sensitivity;

        m_Yaw += offset.first;
        m_Pitch += offset.second;

        // make sure that when the pitch is out of bounds, the screen doesn't get flipped
        if (m_Pitch > 89.0f) m_Pitch = 89.0f;
        if (m_Pitch < -89.0f) m_Pitch = -89.0f;

        glm::vec3 front{};
        front.x = static_cast<float>(std::cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)));
        front.y = static_cast<float>(std::sin(glm::radians(m_Pitch)));
        front.z = static_cast<float>(std::sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)));
        m_CameraFront = glm::normalize(front);
    }

    Camera::Camera(const kT::Window& window, const glm::vec3& startPos) noexcept {
        // yaw is initialized to -90.0 degrees since a yaw of
        // 0.0 results in a direction vector pointing to the right,
        // so we initially rotate a bit to the left.
        m_Yaw   = -90.0f;

        m_Pitch =  0.0f;
        m_LastCursorPosition.first = window.getWidth() / 2.0;
        m_LastCursorPosition.second = window.getHeight() / 2.0;
        m_Fov = 45.0f;

        m_Sensitivity = 0.3;

        m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        m_CameraPos = startPos;
        m_CameraUp = glm::vec3(0.0f, 1.0f,  0.0f);

        m_Projection = glm::perspective(glm::radians(static_cast<float>(m_Fov)),
                                        static_cast<float>(window.getWidth()) / static_cast<float>(window.getHeight()),nearPlane, farPlane);
    }

    auto Camera::getFieldOfView() const -> double {
        return m_Fov;
    }

    auto Camera::getProjection() const -> const glm::mat4 & {
        return m_Projection;
    }

    auto Camera::updateProjection(double width, double height) -> void {
        m_Projection = glm::perspective(glm::radians(static_cast<float>(m_Fov)),
                                        static_cast<float>(width) / static_cast<float>(height),nearPlane, farPlane);
    }

    auto Camera::setFieldOfView(double offset) -> void {
        m_Fov = m_Fov - offset * m_Sensitivity;

        if (m_Fov < minFov) m_Fov = 1.0f;
        if (m_Fov > maxFov) m_Fov = 45.0f;
    }

    auto Camera::getPosition() const -> const glm::vec3 & {
        return m_CameraPos;
    }
}