/**
 * @file Renderer.hh
 * @author Kate
 * @brief Defines the Renderer interface
 * @version 1.0
 * @date 2023-03-16
 */

#ifndef RENDERER_HH
#define RENDERER_HH

// C++ Standard Library
#include <array>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <string_view>
#include <string>
#include <vector>
#include <memory>

// Third-Party Libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

// Project Libraries
#include "Mesh.hh"
#include "Camera.hh"
#include "Shader.hh"
#include "Window.hh"
#include "Vao.hh"
#include "Vbo.hh"
#include "utils.hh"

#include "Model.hh"

namespace Kate {
    class Renderer {
    public:

        /**
         * Initializes both the GLEW and the GLFW library and provides with a valid
         * OpenGL context. On success it sets the Init bit to true, otherwise it sets it to false
         * @param appName Title for the Renderer Window
         * */
        explicit Renderer(std::string_view name = "Renderer", std::int32_t width = 1280, std::int32_t height = 1000);

        auto run() -> void;
        auto startUp() -> void;
        auto shutDown() -> void;

        ~Renderer();

    private:
        auto initImGui() -> void;
        Kate::Window    m_Window{};
        Kate::Shader    m_DefaultShaders{};
        Kate::Vao       m_Vao{};
        Kate::Vbo       m_Vbo{};
        Kate::Vib       m_Vib{};
        Kate::Camera    m_Camera{};
        std::vector<Kate::Texture> m_Texture{};
        std::vector<Mesh> m_Meshes{};
    };

}


#endif // RENDERER_HH
