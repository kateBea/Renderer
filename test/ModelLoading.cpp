#include <memory>
#include <Core/ImGuiLayer.hh>
#include <Core/ModelLoadLayer.hh>
#include <Application.hh>

int main(int, char**) {
    auto app{ std::make_shared<kT::Application>() };

    app->Init();
    app->PushLayer(std::make_shared<kT::ModelLoader>());

    while (app->IsRunning())
        app->UpdateState();

    return 0;
}