cmake_minimum_required(VERSION 3.22)
project(Renderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    library/glew/include
    library/glfw/include
    library/glm
    library/stb
    src/
)

add_compile_options(-Wall -Wpedantic -Wextra -fsanitize=address)
add_link_options(-fsanitize=address)

find_package(OpenGL REQUIRED)

add_subdirectory(library/glfw)
add_subdirectory(library/glew/build/cmake)

set(LIBRARIES glfw glew_s)

# Renderer main app build
add_executable(renderer src/main.cpp)
target_link_libraries(renderer ${LIBRARIES})
target_compile_definitions(renderer PUBLIC GLEW_STATIC)

# Tests builds

# Test window
add_executable(test_window test/test_window.cpp)
target_link_libraries(test_window ${LIBRARIES})
target_compile_definitions(test_window PUBLIC GLEW_STATIC)