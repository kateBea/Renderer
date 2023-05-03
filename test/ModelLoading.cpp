

#include "Window.hh"
#include "Camera.hh"
#include "Model.hh"
#include "Shader.hh"

int main(int, char**) {
    Kate::Window window{ "ModelLoading", 1280, 720 };
    Kate::Camera camera{ window };
    Kate::Shader shader{ "../assets/shaders/basicVertex.glsl",
                         "../assets/shaders/basicFragment.glsl" };

    Kate::Model objModel{ "../assets/models/lowPolyG19/source/G19.blend" };

    glEnable(GL_DEPTH_TEST);

    while (!window.shouldClose()) {
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // view/projection transformations
        shader.setUniformMat4("projection", camera.getProjection());
        shader.setUniformMat4("view", camera.getView());

        if (window.isMouseButtonDown(GLFW_MOUSE_BUTTON_2))
            camera.move(window.getCursorPosition());

        camera.lookAround(window, glm::vec3(0.0f, 0.0f, 0.0f));
        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        shader.setUniformMat4("model", model);
        objModel.draw(shader);


        window.draw();

    }

    return 0;
}