//
// Created by zanet on 6/10/2023.
//

#ifndef RENDERER_APPLICATION_HH
#define RENDERER_APPLICATION_HH

#include <memory>

#include <Core/Window.hh>
#include "Core/LayerStack.hh"

namespace kT {
    class Application {
    public:
        auto Init() -> void;
        auto UpdateState() -> void;
        auto PushLayer(std::shared_ptr<Layer> layer) -> void;
        auto IsRunning() -> bool { return m_State == State::RUNNING; }

    private:
        enum class State {
            RUNNING,
            STOPPED,
        };

        State m_State;
        std::shared_ptr<Window> m_Window{};
        std::shared_ptr<LayerStack> m_LayerStack{};
    };
}


#endif //RENDERER_APPLICATION_HH
