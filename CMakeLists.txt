cmake_minimum_required(VERSION 3.22)
project(Renderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
        libs/third_party/glew/include
        libs/third_party/glfw/include
        libs/third_party/soil/inc
        libs/third_party/glm
        libs/kate
)

add_subdirectory(libs/third_party/glew/build/cmake)
add_subdirectory(libs/third_party/glfw)

add_executable(renderer main.cpp)

target_link_libraries(renderer glfw glew_s)
