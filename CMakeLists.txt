cmake_minimum_required(VERSION 3.22)
project(Renderer)

# Declarations
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LIBRARIES glfw glew_s)

# Source files
SET(SOURCES
        src/main.cpp
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
        library/imgui/imgui.cpp
        library/imgui/backends/imgui_impl_glfw.cpp
        library/imgui/imgui_widgets.cpp
        library/imgui/imgui_draw.cpp
        library/imgui/imgui_tables.cpp
        library/imgui/backends/imgui_impl_opengl3.cpp)

# Header files directories
include_directories(
    library/glew/include
    library/glfw/include
    library/glm
    library/stb
    library/imgui
    include/
)

# Compiler flags
if (MSVC)
    # Compile commands for Windows
    add_compile_options(/DWINDOWS)
else()
    # Compile commands for Linux
    add_compile_options(-Wall -Wpedantic -Wextra -fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

# find OpenGL package in system
find_package(OpenGL REQUIRED)

# Add libraries from submodules
add_subdirectory(library/glfw)
add_subdirectory(library/glew/build/cmake)

# Renderer main app build
add_executable(renderer ${SOURCES})
target_link_libraries(renderer ${LIBRARIES})
target_compile_definitions(renderer PUBLIC GLEW_STATIC)