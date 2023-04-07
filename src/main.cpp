// C++ Standard Library
#include <cstdio>
#include <memory>

// Renderer library
#include <renderer.hh>

int main() {
    auto app{ std::make_unique<kate::renderer>() };
    app->start_up();
    app->run();
    return 0;
}
