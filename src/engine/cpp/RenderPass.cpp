#include "RenderPass.h"

#include <stdexcept>



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
    // 1. Definir el attachment de color.
    VkAttachmentDescription colorAttachment{};

    colorAttachment.format = swapChainImageFormat;

    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

    // Limpiar la imagen al comenzar el renderizado.
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;

    // Conservar el resultado al finalizar.
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    colorAttachment.stencilLoadOp =
        VK_ATTACHMENT_LOAD_OP_DONT_CARE;

    colorAttachment.stencilStoreOp =
        VK_ATTACHMENT_STORE_OP_DONT_CARE;

    // No necesitamos conservar el contenido anterior.
    colorAttachment.initialLayout =
        VK_IMAGE_LAYOUT_UNDEFINED;

    // La imagen quedara lista para presentarse.
    colorAttachment.finalLayout =
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


    // 2. Referencia al attachment dentro del subpass.
    VkAttachmentReference colorAttachmentRef{};

    colorAttachmentRef.attachment = 0;

    colorAttachmentRef.layout =
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


    // 3. Definir el subpass de gráficos.
    VkSubpassDescription subpass{};

    subpass.pipelineBindPoint =
        VK_PIPELINE_BIND_POINT_GRAPHICS;

    subpass.colorAttachmentCount = 1;

    subpass.pColorAttachments =
        &colorAttachmentRef;


    // 4. Sincronizar el acceso al attachment de color.
    VkSubpassDependency dependency{};

    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;

    dependency.dstSubpass = 0;

    dependency.srcStageMask =
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    dependency.dstStageMask =
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    dependency.srcAccessMask = 0;

    dependency.dstAccessMask =
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;


    // 5. Crear la estructura principal del RenderPass.
    VkRenderPassCreateInfo renderPassInfo{};

    renderPassInfo.sType =
        VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

    renderPassInfo.attachmentCount = 1;

    renderPassInfo.pAttachments = &colorAttachment;

    renderPassInfo.subpassCount = 1;

    renderPassInfo.pSubpasses = &subpass;

    renderPassInfo.dependencyCount = 1;

    renderPassInfo.pDependencies = &dependency;


    // 6. Solicitar a Vulkan la creación del recurso.
    if (vkCreateRenderPass(
            device,
            &renderPassInfo,
            nullptr,
            &renderPass) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "No se pudo crear el RenderPass.");
    }
}

// error: 'VkRenderPass' does not name a type 1 | VkRenderPass RenderPass::getRenderPass() const
//eso me salio por no incluir el .h en el cpp
VkRenderPass RenderPass::getRenderPass() const
{
    return renderPass;
}

void RenderPass::cleanup()
{
    if (renderPass != VK_NULL_HANDLE)
    {
        vkDestroyRenderPass(
            device,
            renderPass,
            nullptr);

        renderPass = VK_NULL_HANDLE;
    }
}
