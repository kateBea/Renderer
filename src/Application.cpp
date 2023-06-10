//
// Created by zanet on 6/10/2023.
//

#include "Application.hh"
#include <OpenGL/Renderer.hh>
#include "Core/Logger.hh"
#include <Core/InputManager.hh>
#include <Core/ImGuiLayer.hh>

namespace kT {
    auto Application::Init() -> void {
        Logger::Init();

        m_State = State::RUNNING;
        m_Window = std::make_shared<Window>();
        m_Window->StartUp("ModelLoading", 1280, 720);
        m_LayerStack = std::make_shared<LayerStack>();

        Renderer::Init();
        InputManager::Init(m_Window->GetWindowPointer());
        ImGuiLayer::ImGuiInit(m_Window);
    }

    auto Application::UpdateState() -> void {
        if (m_Window->ShouldClose()) {
            m_State = State::STOPPED;
            return;
        }

        for (auto& layer : *m_LayerStack)
            layer->OnUpdate(m_Window);

        ImGuiLayer::ImGuiBeginFrame();
        for (auto& layer : *m_LayerStack)
            layer->OnImGuiRender();
        ImGuiLayer::ImGuiEndFrame();

        m_Window->SwapBuffers();
    }

    auto Application::PushLayer(std::shared_ptr<Layer> layer) -> void {
        layer->OnAttach(m_Window);
        m_LayerStack->addLayer(layer);
    }
}
