cmake_minimum_required(VERSION 3.22)
project(Renderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    library/glew/include
    library/glfw/include
    library/glm
    src/
)

add_subdirectory(library/glew/build/cmake)
add_subdirectory(library/glfw)


# Renderer main app build
add_executable(renderer main.cpp)
target_link_libraries(renderer glfw glew_s)
target_compile_definitions(renderer PUBLIC GLEW_STATIC)

# Tests build

# Test window
add_executable(test_window test/test_window.cpp)
target_link_libraries(test_window glfw glew_s)
target_compile_definitions(test_window PUBLIC GLEW_STATIC)