// C++ Standard Library
#include <cstdio>
#include <memory>

// Renderer library
#include <application.hh>

int main() {
    std::printf("application entry point...\n");

    auto app{ std::make_unique<kate::application>() };

    app->start_up();
    app->run();
    return 0;
}
