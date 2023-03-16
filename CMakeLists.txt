cmake_minimum_required(VERSION 3.22)
project(Renderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
        libs/ThirdParty/glew/include
        libs/ThirdParty/glfw/include
        libs/ThirdParty/soil/inc
        libs/ThirdParty/glm
        libs/kate
)

add_subdirectory(libs/ThirdParty/glew/build/cmake)
add_subdirectory(libs/ThirdParty/glfw)

add_executable(renderer main.cpp)

target_link_libraries(renderer glfw glew_s)
