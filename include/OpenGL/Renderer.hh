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
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

// Project Libraries
#include "Mesh.hh"
#include "Core/Common.hh"
#include "Model.hh"

namespace kT {
    class Renderer {
    public:
        static auto Init() -> void;
        static auto ShutDown() -> void;

        static auto EnableWireframeMode() -> void;
        static auto DisableWireframeMode() -> void;

        static auto DrawMesh(Shader& shader, Mesh& mesh) -> void;
        static auto DrawModel(Shader& shader, Model& model) -> void;

        static auto ClearColor(float red, float green, float blue, float alpha) -> void;
        static auto ClearColor(const glm::vec4& color) -> void;

    };

}

#endif // RENDERER_HH
