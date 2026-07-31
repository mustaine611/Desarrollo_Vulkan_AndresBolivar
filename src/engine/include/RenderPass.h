#pragma once

#include <vulkan/vulkan.h>

class RenderPass{
    public:
        void initialize(
            VkDevice device,
            VkFormat swapChainImageFormat
        );
        //esto es un getter y creo que se refiere al miembro publico de la clase renderpass
        VkRenderPass getRenderPass() const;
        void cleanup();
        

    private:
        void createRenderPass();

        VkDevice device = VK_NULL_HANDLE;
        VkFormat swapChainImageFormat;

        //miembro privado
        //creo que ese VK_NULL_HANDLE es lo que realmente devuelve vulkan
        //y accedemos a ese valor a travez del getter
        VkRenderPass renderPass = VK_NULL_HANDLE;
};