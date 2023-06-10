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
#include <OpenGL/Mesh.hh>
#include <OpenGL/Model.hh>
#include <OpenGL/Shader.hh>
#include <OpenGL/ElementBuffer.hh>

namespace kT {
    class Renderer {
    public:
        static auto Init() -> void;
        static auto ShutDown() -> void;

        static auto EnableWireframeMode() -> void;
        static auto DisableWireframeMode() -> void;

        static auto DrawMesh(Shader& shader, const Mesh& mesh) -> void;
        static auto DrawModel(Shader& shader, Model& model) -> void;
        static auto DrawGeometry(Shader& shader, const VertexBuffer& vertexBuffer) -> void;
        static auto DrawGeometry(Shader& shader, const VertexBuffer& vertexBuffer, const ElementBuffer& indexBuffer) -> void;
        static auto DrawGeometry(Shader &shader, std::initializer_list<float> &&vertexBuffer) -> void;
        static auto DrawGeometry(Shader &shader, std::initializer_list<float> &&vertexBuffer, std::initializer_list<std::uint32_t> &&indexBuffer) -> void;

        static auto ClearColor(float red, float green, float blue, float alpha) -> void;
        static auto ClearColor(const glm::vec4& color) -> void;

        static auto ResetViewport(std::int32_t width, std::int32_t height) -> void;

    private:
        inline static std::shared_ptr<VertexArray> s_VertexArray{};

    };

}

#endif // RENDERER_HH
