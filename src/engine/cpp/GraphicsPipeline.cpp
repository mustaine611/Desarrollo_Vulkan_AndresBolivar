#include "GraphicsPipeline.h"

#include <stdexcept>
#include <vector>
#include <fstream>

//esto es a lo que llaman "firma" , la firma abarca los nombres y los parametros.
//si cambias el nombre o los parametros ya sera una firma distinta
void GraphicsPipeline::initialize(
     VkDevice device,
    VkExtent2D extent,
    VkRenderPass renderPass
)
{
    //guardar device
    this->device = device;
    this->extent = extent;
    this->renderPass = renderPass;
    
    createPipelineLayout();

    createGraphicsPipeline();
}

void GraphicsPipeline::cleanup()
{
    if (graphicsPipeline != VK_NULL_HANDLE)
    {
        vkDestroyPipeline(
            device,
            graphicsPipeline,
            nullptr
        );
    }

    if (pipelineLayout != VK_NULL_HANDLE)
    {
        vkDestroyPipelineLayout(
            device,
            pipelineLayout,
            nullptr
        );
    }
}

void GraphicsPipeline::createPipelineLayout()
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

    pipelineLayoutInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;

    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(
            device,
            &pipelineLayoutInfo,
            nullptr,
            &pipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "No se pudo crear el Pipeline Layout."
        );
    }
}

void GraphicsPipeline::createGraphicsPipeline()
{
}

