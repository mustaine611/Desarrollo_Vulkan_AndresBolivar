#include "Application.h"

#include <stdexcept>

void Application::run()
{
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void Application::initWindow()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(
        WIDTH,
        HEIGHT,
        "Mi Motor Vulkan",
        nullptr,
        nullptr
    );

    if (!window)
    {
        throw std::runtime_error("No se pudo crear la ventana.");
    }
}

void Application::initVulkan()
{
    vulkanContext.initialize(window);
}

void Application::mainLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
}

void Application::cleanup()
{
    vulkanContext.cleanup();

    glfwDestroyWindow(window);

    glfwTerminate();
}

GLFWwindow* Application::getWindow() const
{
    return window;
}