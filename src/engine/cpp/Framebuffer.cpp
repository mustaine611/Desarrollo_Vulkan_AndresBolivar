#include "Framebuffer.h"
#include <stdexcept>

void Framebuffer::initialize(
    VkDevice device,
    VkRenderPass renderPass,
    VkExtent2D extent,
    const std::vector<VkImageView>& imageViews
)
{
    this->device = device;
    this->renderPass = renderPass;
    this->extent = extent;

    framebuffers.resize(imageViews.size());

    for (size_t i = 0; i < imageViews.size(); i++)
    {
        VkImageView attachments[] =
        {
            imageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};

        framebufferInfo.sType =
            VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;

        framebufferInfo.renderPass =
            renderPass;

        framebufferInfo.attachmentCount = 1;

        framebufferInfo.pAttachments =
            attachments;

        framebufferInfo.width =
            extent.width;

        framebufferInfo.height =
            extent.height;

        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(
                device,
                &framebufferInfo,
                nullptr,
                &framebuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error(
                "No se pudo crear el Framebuffer."
            );
        }
    }
}

void Framebuffer::cleanup()
{
    for (auto framebuffer : framebuffers)
    {
        vkDestroyFramebuffer(
            device,
            framebuffer,
            nullptr
        );
    }

    framebuffers.clear();
}