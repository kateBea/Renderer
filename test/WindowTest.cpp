// C++ Standard Library
#include <cstdio>
#include <memory>

// Renderer library
#include <Window.hh>

int main(int, char**) {
    std::printf("test_window entry point...\n");
    auto win{ std::make_unique<Kate::Window>() };
    while (!win->shouldClose()) {

        if (win->isKeyPressed(GLFW_KEY_A))
            std::cout << "Button A pressed\n";

        if (win->isKeyPressed(GLFW_KEY_B))
            std::cout << "Button B pressed\n";

        if (win->isKeyPressed(GLFW_KEY_C))
            std::cout << "Button C pressed\n";

        if (win->isKeyPressed(GLFW_KEY_D))
            std::cout << "Button D pressed\n";

        win->draw();
    }

    return 0;
}