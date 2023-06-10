//
// Created by zanet on 6/10/2023.
//

#ifndef RENDERER_LAYERSTACK_HH
#define RENDERER_LAYERSTACK_HH
#include <vector>
#include <memory>
#include <cstdint>

// Project Headers
#include <Core/Layer.hh>

namespace kT {
    class LayerStack {
    public:
        explicit LayerStack() : m_Layers{}, m_LayerIndex{} {}

        auto shutDown() -> void { /*Empty for now */ }

        auto addLayer(std::shared_ptr<Layer> layer) -> void;
        auto addOverlay(std::shared_ptr<Layer> overlay) -> void;

        auto popLayer(std::shared_ptr<Layer> layer) -> void;
        auto popOverlay(std::shared_ptr<Layer> overlay) -> void;

        auto begin() ->  std::vector<std::shared_ptr<Layer>>::iterator { return m_Layers.begin(); }
        auto end() ->  std::vector<std::shared_ptr<Layer>>::iterator { return m_Layers.end(); }

        auto rbegin() -> std::vector<std::shared_ptr<Layer>>::reverse_iterator { return m_Layers.rbegin(); }
        auto rend() -> std::vector<std::shared_ptr<Layer>>::reverse_iterator { return m_Layers.rend(); }

    private:
        std::vector<std::shared_ptr<Layer>> m_Layers{};
        std::uint32_t m_LayerIndex{};
    };
}


#endif //RENDERER_LAYERSTACK_HH
