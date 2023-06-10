//
// Created by zanet on 6/10/2023.
//

#ifndef RENDERER_IMGUI_LAYER_HH
#define RENDERER_IMGUI_LAYER_HH

#include "Core/Layer.hh"
#include "Window.hh"

namespace kT {
    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer(std::string_view name = "ImGuiLayer") : Layer{ name } {}
        auto OnAttach(std::shared_ptr<Window> handle = nullptr) -> void override;
        auto OnUpdate(std::shared_ptr<Window> handle) -> void override;
        auto OnImGuiRender() -> void override;

        static auto ImGuiInit(std::shared_ptr<Window> handle) -> void;
        static auto ImGuiBeginFrame() -> void;
        static auto ImGuiEndFrame() -> void;
    private:
        static auto setCustomStyle() -> void;
        static auto loadIniFile(const std::string &path) -> void;
    };
}


#endif //RENDERER_IMGUI_LAYER_HH
