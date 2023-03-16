// C++ Standard Library
#include <cstdio>
#include <memory>

// Renderer library
#include <Application.hh>

int main() {
    std::printf("Application entry point...\n");

    auto app{ std::make_unique<Kate::Application>() };

    app->startUp();
    app->run();
    return 0;
}
