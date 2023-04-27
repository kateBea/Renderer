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
#include <cstdio>
#include <cstddef>
#include <cmath>

// Third-Party Libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Project Libraries
#include "InputManager.hh"
#include "Window.hh"

namespace Kate {
    class Camera {
    public:
        /**
         *
         * */
        explicit Camera() = default;

        /**
         * Creates a new instance of a camera within the given window.
         * */
        explicit Camera(const Kate::Window& window, const glm::vec3& startPos = glm::vec3(0.0f, -1.0f,  7.0f)) noexcept;

        /**
         * Copy constructor disabled
         * */
        Camera(const Camera&) = delete;

        /**
         * Copy assigment constructor disabled
         * */
        Camera& operator=(const Camera&) = delete;

        /**
         * Move constructor disabled
         * */
        Camera(Camera&&) = delete;

        /**
         * Move assigment constructor disabled
         * */
        Camera& operator=(Camera&&) = delete;

        /**
         *
         * */
        auto lookAround(const Kate::Window& window, const glm::vec3& target) -> void;

        /**
         * Returns the current field of view of this camera.
         * On creation of a Camera instance, the default field of
         * view value is set to 45 degrees
         * @return field of view
         * */
        [[nodiscard]]
        auto getFieldOfView() const -> double;

        /**
         * Returns the projection matrix of this Camera
         * @return projection matrix
         * */
        [[nodiscard]]
        auto getProjection() const -> const glm::mat4&;

        /**
         *
         * */
        auto move(const std::pair<double, double>& newCursorPosition) -> void;

        /**
         *
         * */
        auto updateProjection(double width, double height) -> void;

        /**
         *
         * */
        auto showCameraPos() const -> void;

        /**
         *
         * */
        [[nodiscard]]
        auto getView() const -> const glm::mat4 &;

    private:
        /**
         *
         * */
        static constexpr float nearPlane{ 0.01f };
        /**
         *
         * */
        static constexpr float farPlane{ 100.f };

        /**
         *
         * */
        auto computeMatricesFromInput(const Kate::Window &window) -> void;

        glm::mat4 m_View{};
        glm::mat4 m_Projection{};

        glm::vec3 m_CameraFront{};
        glm::vec3 m_CameraPos{};
        glm::vec3 m_CameraUp{};

        double m_Pitch{};
        double m_Yaw{};
        double m_Roll{};

        double m_Sensitivity{};

        double m_Fov{};

        std::pair<double, double> m_LastCursorPosition{};
    };
}
#endif // END CAMERA_HH