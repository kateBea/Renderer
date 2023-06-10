cmake_minimum_required(VERSION 3.22)
project(Renderer)

# Declarations
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LIBRARIES glfw glew_s assimp)

# Source files
SET(SOURCES
        src/Texture.cpp
        src/Camera.cpp
        src/InputManager.cpp
        src/Mesh.cpp
        src/Renderer.cpp
        src/Shader.cpp
        src/Vao.cpp
        src/Vib.cpp
        src/Vbo.cpp
        src/Window.cpp
        src/std_image.cpp
        src/Model.cpp
        src/Logger.cpp
        src/BasicLighting.cpp
        library/imgui/imgui.cpp
        library/imgui/backends/imgui_impl_glfw.cpp
        library/imgui/imgui_widgets.cpp
        library/imgui/imgui_draw.cpp
        library/imgui/imgui_tables.cpp
        library/imgui/backends/imgui_impl_opengl3.cpp src/ModelLoadLayer.cpp src/ImGuiLayer.cpp src/Application.cpp include/Core/LayerStack.cpp)

# Header files directories
include_directories(
    library/glew/include
    library/glfw/include
    library/glm
    library/stb
    library/imgui
    library/fmt/include
    library/spdlog/include
    library/assimp/include
    include/
)

# Compiler flags
if (MSVC)
    # Compile commands for Windows

else()
    # Compile commands for Linux
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()


# Add libraries from submodules
add_subdirectory(library/glfw)
add_subdirectory(library/glew/build/cmake)
add_subdirectory(library/fmt)
add_subdirectory(library/assimp)


# Test builds

# Test with specular Lighting
add_executable(specularLight test/SpecularLighting.cpp ${SOURCES})
target_link_libraries(specularLight ${LIBRARIES})
target_compile_definitions(specularLight PUBLIC GLEW_STATIC)

# Test with Model Loading
add_executable(modelLoading test/ModelLoading.cpp ${SOURCES})
target_link_libraries(modelLoading ${LIBRARIES})
target_compile_definitions(modelLoading PUBLIC GLFW_INCLUDE_NONE)
target_compile_definitions(modelLoading PUBLIC GLEW_STATIC)

# Test with Importer
add_executable(importer test/TestImporter.cpp ${SOURCES})
target_link_libraries(importer ${LIBRARIES})
target_compile_definitions(importer PUBLIC GLEW_STATIC)