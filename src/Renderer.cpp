#include "../include/Renderer.hh"

namespace kT {
    // IMPLEMENTATION
    Renderer::Renderer(const std::filesystem::path& path)
        :   m_DefaultShaders{}, m_LightShader{}, m_Meshes{}, m_ShadersPath{ path }
    {}

    auto Renderer::startUp() -> void {
        m_DefaultShaders.load("defaultVertex.glsl", "defaultFragment.glsl");
        // Setup OpenGL Rendering
        setOpenGLHints();
    }

    auto Renderer::shutDown() -> void {

    }

    auto Renderer::enableWireframeMode() -> void {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    auto Renderer::disableWireframeMode() -> void {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    auto Renderer::setOpenGLHints() -> void {
        glEnable(GL_DEPTH_TEST);
    }
}