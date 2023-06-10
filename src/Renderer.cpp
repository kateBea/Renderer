#include "OpenGL/Renderer.hh"

namespace kT {
    auto Renderer::Init() -> void {
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    auto Renderer::ShutDown() -> void {

    }

    auto Renderer::EnableWireframeMode() -> void {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    auto Renderer::DisableWireframeMode() -> void {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    auto Renderer::DrawMesh(Shader& shader, Mesh& mesh) -> void {
        mesh.draw(shader);
    }

    auto Renderer::DrawModel(Shader& shader, Model& model) -> void {
        model.draw(shader);
    }

    auto Renderer::ClearColor(const glm::vec4 &color) -> void {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    auto Renderer::ClearColor(float red, float green, float blue, float alpha) -> void {
        glClearColor(red, green, blue, alpha);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

}