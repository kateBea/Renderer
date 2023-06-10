//
// Created by zanet on 6/10/2023.
//

#ifndef RENDERER_MODEL_LOAD_LAYER_HH
#define RENDERER_MODEL_LOAD_LAYER_HH

#include <memory>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Core/Window.hh"
#include "OpenGL/Camera.hh"
#include "OpenGL/Model.hh"
#include "OpenGL/Shader.hh"
#include "Core/Logger.hh"
#include "Core/Layer.hh"

namespace kT {
    class ModelLoader : public kT::Layer {
    public:
        ModelLoader(std::string_view name = "ModelLoadLayer") : Layer{ name } {}
        auto OnAttach(std::shared_ptr<Window> handle = nullptr) -> void override;
        auto OnUpdate(std::shared_ptr<Window> handle) -> void override;
        auto OnImGuiRender() -> void override;

    private:
        // ImGui Panels Data
        bool            m_Lines{ false };
        glm::vec4         m_ClearColor {};
        glm::vec3       m_Rotation{};
        glm::vec3       m_ModelPosition{};
        glm::vec3       m_ModelSize{ 1.0, 1.0, 1.0 };
        glm::vec3       m_LightPosition{ 3.0f, 0.0f, -5.0f };

        std::shared_ptr<Camera> m_Camera{};
        std::shared_ptr<Shader> m_DefaultShader{};
        std::shared_ptr<Model> m_Model{};

    };
}


#endif //RENDERER_MODELLOADLAYER_HH
