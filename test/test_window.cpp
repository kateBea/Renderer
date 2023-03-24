// C++ Standard Library
#include <cstdio>
#include <memory>

// Renderer library
#include <window.hh>

int main(int, char**) {
    std::printf("test_window entry point...\n");
    auto win{ std::make_unique<kate::window>() };
    while (!win->should_close()) {
        win->draw();
    }

    return 0;
}