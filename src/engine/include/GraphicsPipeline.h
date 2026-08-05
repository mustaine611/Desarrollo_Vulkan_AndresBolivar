#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

class GraphicsPipeline{
    public:

        //se debe especificar el tipo de retorno que en este caso es "void"
        void initialize( 

            //estos son los que llaman "parametros" de la firma, en esta parte estamos en la etapa de declaracion
            //en el archivo cpp estariamos en la etapa de definicion
            VkDevice device,
            VkExtent2D extent,
            VkRenderPass renderPass
        );
        void cleanup();//siempre deben terminar en " ; "
    
    private:
        //se debe especificar el tipo de retorno que en este caso es "void"
        void createPipelineLayout();
        void createGraphicsPipeline();
        static std::vector<char> readFile(const std::string& filename);
        VkShaderModule createShaderModule(const std::vector<char>& code);

        //conviene inicializar los miembros privados con VK_NULL_HANDLE
        VkDevice device = VK_NULL_HANDLE;
        VkRenderPass renderPass = VK_NULL_HANDLE;
        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
        VkPipeline graphicsPipeline = VK_NULL_HANDLE;
        VkExtent2D extent{};

};