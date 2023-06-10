#ifndef LAYER_HH
#define LAYER_HH

// Third-Party Libraries
#include <GLFW/glfw3.h>

// Project Headers
#include <Core/Window.hh>

namespace kT {
    class Layer {
    public:
        Layer(std::string_view name = "BaseLayer") : m_Name{ name } {}
        virtual auto OnAttach(std::shared_ptr<Window> handle = nullptr) -> void = 0;
        virtual auto OnUpdate(std::shared_ptr<Window> handle) -> void = 0;
        virtual auto OnImGuiRender() -> void = 0;

    private:
        std::string m_Name{};
    };
}


#endif // LAYER_HH
