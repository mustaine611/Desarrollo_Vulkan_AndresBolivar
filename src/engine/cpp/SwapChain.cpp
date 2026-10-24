#include "SwapChain.h"
#include <vector>
#include <algorithm>
#include <stdexcept>

void SwapChain::initialize(
    VkPhysicalDevice physicalDevice,
    VkDevice device,
    VkSurfaceKHR surface,
    GLFWwindow* window)
{
    this->physicalDevice = physicalDevice;
    this->device = device;
    this->surface = surface;
    this->window = window;

    SwapChainSupportDetails support = querySwapChainSupport();
    createSwapChain();
    getSwapChainImages();
    createImageViews();
}

SwapChain::SwapChainSupportDetails
SwapChain::querySwapChainSupport()
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        physicalDevice,
        surface,
        &details.capabilities
    );

    uint32_t formatCount = 0;

    vkGetPhysicalDeviceSurfaceFormatsKHR(
        physicalDevice,
        surface,
        &formatCount,
        nullptr
    );

    if (formatCount != 0)
    {
        details.formats.resize(formatCount);

        vkGetPhysicalDeviceSurfaceFormatsKHR(
            physicalDevice,
            surface,
            &formatCount,
            details.formats.data()
        );
    }

    uint32_t presentModeCount = 0;

    vkGetPhysicalDeviceSurfacePresentModesKHR(
        physicalDevice,
        surface,
        &presentModeCount,
        nullptr
    );

    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);

        vkGetPhysicalDeviceSurfacePresentModesKHR(
            physicalDevice,
            surface,
            &presentModeCount,
            details.presentModes.data()
        );
    }

    return details;
}

VkSurfaceFormatKHR
SwapChain::chooseSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR
SwapChain::choosePresentMode(
    const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D
SwapChain::chooseExtent(
    const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != UINT32_MAX)
    {
        return capabilities.currentExtent;
    }

    int width;
    int height;

    glfwGetFramebufferSize(
        window,
        &width,
        &height
    );

    VkExtent2D actualExtent =
    {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };

    actualExtent.width =
        std::clamp(
            actualExtent.width,
            capabilities.minImageExtent.width,
            capabilities.maxImageExtent.width);

    actualExtent.height =
        std::clamp(
            actualExtent.height,
            capabilities.minImageExtent.height,
            capabilities.maxImageExtent.height);

    return actualExtent;
}

void SwapChain::createSwapChain()
{
    SwapChainSupportDetails support =
        querySwapChainSupport();

    VkSurfaceFormatKHR surfaceFormat =
        chooseSurfaceFormat(support.formats);

    swapChainImageFormat = surfaceFormat.format;    

    VkPresentModeKHR presentMode =
        choosePresentMode(support.presentModes);

    VkExtent2D extent = chooseExtent(support.capabilities);
    swapChainExtent = extent;
    uint32_t imageCount =
        support.capabilities.minImageCount + 1;

    if (support.capabilities.maxImageCount > 0 &&
        imageCount > support.capabilities.maxImageCount)
    {
        imageCount = support.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};

    createInfo.sType =
        VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

    createInfo.surface = surface;

    createInfo.minImageCount = imageCount;

    createInfo.imageFormat =
        surfaceFormat.format;

    createInfo.imageColorSpace =
        surfaceFormat.colorSpace;

    createInfo.imageExtent =
        extent;

    createInfo.imageArrayLayers = 1;

    createInfo.imageUsage =
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    createInfo.imageSharingMode =
        VK_SHARING_MODE_EXCLUSIVE;

    createInfo.preTransform =
        support.capabilities.currentTransform;

    createInfo.compositeAlpha =
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode =
        presentMode;

    createInfo.clipped =
        VK_TRUE;

    createInfo.oldSwapchain =
        VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(
            device,
            &createInfo,
            nullptr,
            &swapChain) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "No se pudo crear la Swap Chain.");
    }
}

void SwapChain::getSwapChainImages()
{
    uint32_t imageCount = 0;

    vkGetSwapchainImagesKHR(
        device,
        swapChain,
        &imageCount,
        nullptr
    );

    swapChainImages.resize(imageCount);

    vkGetSwapchainImagesKHR(
        device,
        swapChain,
        &imageCount,
        swapChainImages.data()
    );
}

void SwapChain::createImageViews()
{
    swapChainImageViews.resize(
        swapChainImages.size());

    for (size_t i = 0;
         i < swapChainImages.size();
         i++)
    {
        VkImageViewCreateInfo createInfo{};

        createInfo.sType =
            VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

        createInfo.image =
            swapChainImages[i];

        createInfo.viewType =
            VK_IMAGE_VIEW_TYPE_2D;

        createInfo.format = 
            swapChainImageFormat;

        createInfo.components.r =
            VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.components.g =
            VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.components.b =
            VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.components.a =
            VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask =
            VK_IMAGE_ASPECT_COLOR_BIT;

        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;

        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(
                device,
                &createInfo,
                nullptr,
                &swapChainImageViews[i]) != VK_SUCCESS)
        {
            throw std::runtime_error(
                "No se pudo crear un Image View.");
        }
    }
}

VkFormat SwapChain::getImageFormat() const
{
    return swapChainImageFormat;
}


//aqui solo estoy implementando el metodo cleanup, no lo estoy ejecutando de una vez
//asi que no se preocupe por el orden, yo creo que el orden de ejecucuion importa en el Vulkancontext
void SwapChain::cleanup()
{
    for (VkImageView imageView :
         swapChainImageViews)
    {
        vkDestroyImageView(
            device,
            imageView,
            nullptr);
    }

    
    if (swapChain != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(
            device,
            swapChain,
            nullptr);
    }
}

VkExtent2D SwapChain::getExtent() const
{
    return swapChainExtent;
}

const std::vector<VkImageView>& SwapChain::getImageViews() const
{
    return swapChainImageViews;
}