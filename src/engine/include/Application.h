#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VulkanContext.h"

class Application
{
public:
    void run();
    GLFWwindow* getWindow() const;

private:
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

private:
    GLFWwindow* window = nullptr;

    VulkanContext vulkanContext;

    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
};