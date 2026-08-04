#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

class SwapChain
{

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
    public:
        void initialize(
            VkPhysicalDevice physicalDevice,
            VkDevice device,
            VkSurfaceKHR surface,
            GLFWwindow* window
        );
        void cleanup();
        VkFormat getImageFormat() const;
        VkExtent2D getExtent() const;
        
    
    private:
        SwapChainSupportDetails querySwapChainSupport();

        VkSurfaceFormatKHR chooseSurfaceFormat(
            const std::vector<VkSurfaceFormatKHR>& availableFormats
        );

        
        VkPresentModeKHR choosePresentMode(
            const std::vector<VkPresentModeKHR>& availablePresentModes
        );

        VkExtent2D chooseExtent(
            const VkSurfaceCapabilitiesKHR& capabilities
        );

        void createSwapChain();
        void getSwapChainImages();
        void createImageViews();

        VkSwapchainKHR swapChain = VK_NULL_HANDLE;
        VkFormat swapChainImageFormat;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;

        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device = VK_NULL_HANDLE;
        VkSurfaceKHR surface = VK_NULL_HANDLE;
        GLFWwindow* window = nullptr;
        VkExtent2D swapChainExtent{};
};