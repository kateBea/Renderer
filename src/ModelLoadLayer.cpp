//
// Created by zanet on 6/10/2023.
//
#include <imgui.h>

#include "Core/Window.hh"
#include "OpenGL/Camera.hh"
#include "OpenGL/Model.hh"
#include "OpenGL/Shader.hh"
#include "Core/Logger.hh"
#include "Core/ModelLoadLayer.hh"
#include <OpenGL/Renderer.hh>


namespace kT {
    auto ModelLoader::OnAttach(std::shared_ptr<Window> handle) -> void {
        m_Camera = std::make_shared<Camera>();
        m_Model = std::make_shared<Model>();
        m_DefaultShader = std::make_shared<Shader>();

        m_Camera->Init(*handle);
        m_DefaultShader->LoadFromFile("../assets/shaders/defaultVertex.glsl", "../assets/shaders/defaultFragment.glsl");
        m_Model->LoadFromFile("../assets/models/Pod42/source/POD/POD.obj");
        m_ClearColor = { 1.0, 1.0, 1.0, 1.0 };
    }

    auto ModelLoader::OnUpdate(std::shared_ptr<Window> handle) -> void {
        m_DefaultShader->setUniformVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        m_DefaultShader->setUniformVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        m_DefaultShader->setUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        m_DefaultShader->setUniformFloat("material.shininess", 64.0f);

        glm::mat4 model{ glm::mat4(1.0f) };
        glm::vec3 tr{ glm::vec3(m_ModelPosition[0], m_ModelPosition[1], m_ModelPosition[2]) };
        glm::vec3 resizing{glm::vec3(m_ModelSize[0], m_ModelSize[1], m_ModelSize[2]) };

        model = glm::translate(model, tr);
        model = glm::scale(model, resizing);
        model = glm::rotate(model, m_Rotation[0], glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, m_Rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, m_Rotation[2], glm::vec3(1.0f, 0.0f, 0.0f));

        m_Camera->updateProjection(handle->GetWidth(), handle->GetHeight());

        if (InputManager::isMouseButtonDown(GLFW_MOUSE_BUTTON_2)) {
            m_Camera->move(InputManager::getMousePos());
        }

        m_Camera->lookAround(*handle, glm::vec3(0.0f, 0.0f, 0.0f));

        m_DefaultShader->setUniformVec3("light.position", glm::vec3(m_LightPosition[0], m_LightPosition[1], m_LightPosition[2]));
        m_DefaultShader->setUniformVec3("viewPos", m_Camera->getPosition());
        m_DefaultShader->setUniformMat4("model", model);
        m_DefaultShader->setUniformMat4("projection", m_Camera->getProjection());
        m_DefaultShader->setUniformMat4("view", m_Camera->getView());

        if (m_Lines)
            Renderer::EnableWireframeMode();
        else
            Renderer::DisableWireframeMode();

        // DRAWING
        Renderer::ClearColor(m_ClearColor);
        Renderer::DrawModel(*m_DefaultShader, *m_Model);
    }

    auto ModelLoader::OnImGuiRender() -> void {
        // Control Panel
        ImGui::Begin("Control Panel");
        ImGui::ColorEdit3("clear color", glm::value_ptr(m_ClearColor));

        ImGui::Text("Container Block Settings");
        ImGui::DragFloat3("Rotation", glm::value_ptr(m_Rotation), 0.1f, 0.0f, 360.0f);
        ImGui::DragFloat3("Coordinates", glm::value_ptr(m_ModelPosition), 0.01f, -100.0f, 100.0f);
        ImGui::DragFloat3("Size", glm::value_ptr(m_ModelSize), 0.01f, 0.0f, 20.0f);

        ImGui::Text("Light block Settings");
        ImGui::DragFloat3("Position", glm::value_ptr(m_LightPosition), 0.1f, -100.0f, 100.0f);
        ImGui::Checkbox("Enable wireframe", &m_Lines);
        ImGui::End();

        static constexpr int HOURS_TO_SECS{ 3600 };
        static constexpr int MIN_TO_SECS{ 60 };
        static auto formatTime{
            [](int hour, int min, int sec) -> std::string {
                std::string result{"Time elapsed: "};

                result.append(hour > 9 ? std::to_string(hour) : "0" + std::to_string(hour));
                result.append(":");
                result.append(min > 9 ? std::to_string(min) : "0" + std::to_string(min));
                result.append(":");
                result.append(sec > 9 ? std::to_string(sec) : "0" + std::to_string(sec));

                return result;
            }
        };

        ImGui::Begin("Inspection");
        ImGui::Text("Model: 1"); // Just one model loaded at a time currently
        ImGui::Text("Vertices: %lx", m_Model->getVertexCount());
        ImGui::Text("Indices: %lx", m_Model->getIndexCount());
        ImGui::Text("Textures: %lx", m_Model->getTextureCount());
        ImGui::Text("Frame Rate: %.1f FPS)", ImGui::GetIO().Framerate);

        auto sTime = static_cast<int>(glfwGetTime());
        ImGui::Text("%s", formatTime(sTime / HOURS_TO_SECS, (sTime % HOURS_TO_SECS) / MIN_TO_SECS,
                                     (sTime % HOURS_TO_SECS) % MIN_TO_SECS).c_str());

        ImGui::End();
    }
}