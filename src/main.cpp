// C++ Standard Library
#include <memory>

// Renderer library
#include "../include/Renderer.hh"

int main() {
    auto app{ std::make_unique<Kate::Renderer>() };

    app->startUp();
    app->run();

    return 0;
}
