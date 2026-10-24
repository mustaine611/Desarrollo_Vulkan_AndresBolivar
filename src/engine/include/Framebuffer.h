#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class Framebuffer
{
public:

    void initialize(
        VkDevice device,
        VkRenderPass renderPass,
        VkExtent2D extent,
        const std::vector<VkImageView>& imageViews
    );

    void cleanup();

private:

    VkDevice device = VK_NULL_HANDLE;

    VkRenderPass renderPass = VK_NULL_HANDLE;

    VkExtent2D extent{};

    std::vector<VkFramebuffer> framebuffers;
};