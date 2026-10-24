#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <optional>
#include "SwapChain.h"
#include "RenderPass.h"
#include "GraphicsPipeline.h"
#include "Framebuffer.h"

class VulkanContext
{
   struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() const
        {
            return graphicsFamily.has_value() &&
                presentFamily.has_value();
        }
    };
        
    public:
        void initialize(GLFWwindow* window);

        void cleanup();

    private:
        void createInstance();
        void createSurface(GLFWwindow* window);
        void pickPhysicalDevice();
        void createLogicalDevice();
        bool isDeviceSuitable(VkPhysicalDevice device);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    private:
        VkInstance instance = VK_NULL_HANDLE;
        VkSurfaceKHR surface = VK_NULL_HANDLE;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

        VkDevice device = VK_NULL_HANDLE;
        VkQueue graphicsQueue = VK_NULL_HANDLE;
        VkQueue presentQueue = VK_NULL_HANDLE;
        SwapChain swapChain;
        RenderPass renderPass;
        GraphicsPipeline graphicsPipeline;
        Framebuffer framebuffer;
};