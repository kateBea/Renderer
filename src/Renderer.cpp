#include "../include/Renderer.hh"

namespace Kate {
    // IMPLEMENTATION
    Renderer::Renderer(std::string_view name, std::int32_t width, std::int32_t height)
            :   m_Window{name, width, height }
            ,   m_DefaultShaders{}
            ,   m_Meshes{}
    {

    }

    auto Renderer::startUp() -> void {
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