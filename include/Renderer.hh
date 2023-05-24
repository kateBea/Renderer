/**
 * @file Renderer.hh
 * @author kT
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
#include "BasicLighting.hh"
#include "Vao.hh"
#include "Vbo.hh"
#include "Common.hh"

#include "Model.hh"

namespace kT {
    class Renderer {
    public:

        /**
         * Initializes this renderer. The shaders for this renderer are specified in the path
         * parameter. The default fragment shader and vertex shader are expected to be named
         * "defaultVertex.glsl" and "defaultFragment.glsl" respectively
         * @param path directory containing the shader files
         * */
        explicit Renderer(const std::filesystem::path& path);

        auto startUp() -> void;
        auto shutDown() -> void;

        static auto enableWireframeMode() -> void;
        static auto disableWireframeMode() -> void;

        auto enableBlending() -> void;

        auto setupBlendingProperties() -> void;

        ~Renderer() = default;

    private:
        static auto setOpenGLHints() -> void;
        kT::Shader    m_DefaultShaders{};
        kT::Shader    m_LightShader{};
        std::vector<Mesh> m_Meshes{};

        std::filesystem::path m_ShadersPath{};
    };

}

#endif // RENDERER_HH
