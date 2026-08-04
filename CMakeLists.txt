cmake_minimum_required(VERSION 3.20)

project(VulkanEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)

add_subdirectory(external/glfw-3.4)

add_executable(VulkanEngine
    src/main.cpp
    src/engine/cpp/Application.cpp
    src/engine/cpp/VulkanContext.cpp
    src/engine/cpp/SwapChain.cpp
    src/engine/cpp/RenderPass.cpp
    src/engine/cpp/GraphicsPipeline.cpp
)

target_include_directories(VulkanEngine PRIVATE
    src/engine/include
)

target_link_libraries(VulkanEngine PRIVATE
    glfw
    Vulkan::Vulkan
)