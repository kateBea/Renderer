/**
 * @file camera.hh
 * @author kate
 * @brief Defines the camera class
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
#include <input.hh>

namespace kate {
    class camera {
    public:
        explicit camera() = default;

        camera(const camera&) = delete;
        camera& operator=(const camera&) = delete;

        camera(camera&&) = delete;
        camera& operator=(camera&&) = delete;

        auto update() -> void;


    private:
        
    };

    // CAMERA IMPLEMENTATION
    inline auto update() -> void {
        glm::mat4 projection{ get_projection() };
        glm::mat4 view{ get_view() };
        glm::mat4 model{ glm::mat4(1.0) };
    }
}
#endif // END CAMERA_HH
