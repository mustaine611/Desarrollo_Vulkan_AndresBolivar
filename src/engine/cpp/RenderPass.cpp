#include "RenderPass.h"
// error: 'VkRenderPass' does not name a type 1 | VkRenderPass RenderPass::getRenderPass() const
//eso me salio por no incluir el .h en el cpp
VkRenderPass RenderPass::getRenderPass() const
{
    return renderPass;
}

// undefined reference to `RenderPass::initialize(VkDevice_T*, VkFormat)'
//ese error me salio por no implementear el initialize de una clase en Vulkan
void RenderPass::initialize(
    VkDevice device,
    VkFormat swapChainImageFormat)
{
    this->device = device;
    this->swapChainImageFormat = swapChainImageFormat;

    createRenderPass();
}

void RenderPass::createRenderPass()
{
    // Se implementara despues.
}