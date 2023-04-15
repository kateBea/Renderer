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
#include <cstddef>

// Third-Party Libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Project Libraries
#include <InputManager.hh>

namespace Kate {
    class Camera {
    public:
        explicit Camera() = default;

        Camera(const Camera&) = delete;
        Camera& operator=(const Camera&) = delete;

        Camera(Camera&&) = delete;
        Camera& operator=(Camera&&) = delete;

        auto update() -> void;


    private:
        auto getProjection() -> glm::mat4 {
            // TODO: Implement
            return {};
        }

        auto getView() -> glm::mat4 {
            // TODO: Implement
            return {};
        }
    };

    // CAMERA IMPLEMENTATION
    inline auto Camera::update() -> void {
        glm::mat4 projection{ getProjection() };
        glm::mat4 view{ getView() };
        glm::mat4 model{ glm::mat4(1.0) };
    }
}
#endif // END CAMERA_HH
