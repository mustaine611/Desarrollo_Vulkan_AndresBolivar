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
    auto vertShaderCode =
        readFile("shaders/triangle.vert.spv");

    auto fragShaderCode =
        readFile("shaders/triangle.frag.spv");

    VkShaderModule vertShaderModule =
        createShaderModule(vertShaderCode);

    VkShaderModule fragShaderModule =
        createShaderModule(fragShaderCode);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};

        vertShaderStageInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

        vertShaderStageInfo.stage =
            VK_SHADER_STAGE_VERTEX_BIT;

        vertShaderStageInfo.module =
            vertShaderModule;

        vertShaderStageInfo.pName = "main";


    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};

        fragShaderStageInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

        fragShaderStageInfo.stage =
            VK_SHADER_STAGE_FRAGMENT_BIT;

        fragShaderStageInfo.module =
            fragShaderModule;

        fragShaderStageInfo.pName = "main";


    VkPipelineShaderStageCreateInfo shaderStages[] =
        {
            vertShaderStageInfo,
            fragShaderStageInfo
        };  
    //Los Vk son estructuras (struct) y los nombres que ponemos justo a la derecha son una "instancia"   
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};

        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexBindingDescriptions = nullptr;

        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};

        inputAssembly.sType =
            VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;

        inputAssembly.topology =
            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        inputAssembly.primitiveRestartEnable =
            VK_FALSE;

    
    VkViewport viewport{};


        viewport.x = 0.0f;
        viewport.y = 0.0f;

        viewport.width =
            static_cast<float>(extent.width);

        viewport.height =
            static_cast<float>(extent.height);

        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

    VkRect2D scissor{};

        scissor.offset = {0, 0};

        scissor.extent = extent;

    VkPipelineViewportStateCreateInfo viewportState{};

        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;

        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer{};

        rasterizer.sType =
            VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;

        rasterizer.depthClampEnable = VK_FALSE;

        rasterizer.rasterizerDiscardEnable = VK_FALSE;

        rasterizer.polygonMode =
            VK_POLYGON_MODE_FILL;

        rasterizer.lineWidth = 1.0f;

        rasterizer.cullMode =
            VK_CULL_MODE_BACK_BIT;

        rasterizer.frontFace =
            VK_FRONT_FACE_CLOCKWISE;

        rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};

        multisampling.sType =
            VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;

        multisampling.sampleShadingEnable = VK_FALSE;

        multisampling.rasterizationSamples =
            VK_SAMPLE_COUNT_1_BIT;

        multisampling.minSampleShading = 1.0f;

        multisampling.pSampleMask = nullptr;

        multisampling.alphaToCoverageEnable = VK_FALSE;

        multisampling.alphaToOneEnable = VK_FALSE;    


    VkPipelineColorBlendAttachmentState colorBlendAttachment{};

        colorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT |
            VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT;

        colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending{};

        colorBlending.sType =
            VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;

        colorBlending.logicOpEnable = VK_FALSE;

        colorBlending.logicOp =
            VK_LOGIC_OP_COPY;

        colorBlending.attachmentCount = 1;

        colorBlending.pAttachments =
            &colorBlendAttachment;

        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;   

    VkGraphicsPipelineCreateInfo pipelineInfo{};

        pipelineInfo.sType =
            VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;

        pipelineInfo.pVertexInputState =
            &vertexInputInfo;

        pipelineInfo.pInputAssemblyState =
            &inputAssembly;

        pipelineInfo.pViewportState =
            &viewportState;

        pipelineInfo.pRasterizationState =
            &rasterizer;

        pipelineInfo.pMultisampleState =
            &multisampling;

        pipelineInfo.pDepthStencilState =
            nullptr;

        pipelineInfo.pColorBlendState =
            &colorBlending;

        pipelineInfo.pDynamicState =
            nullptr;

        pipelineInfo.layout =
            pipelineLayout;

        pipelineInfo.renderPass =
            renderPass;

        pipelineInfo.subpass = 0;

        pipelineInfo.basePipelineHandle =
            VK_NULL_HANDLE;

        pipelineInfo.basePipelineIndex =
            -1;

    }


std::vector<char> GraphicsPipeline::readFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("No se pudo abrir el archivo.");
    }

    size_t fileSize = static_cast<size_t>(file.tellg());

    std::vector<char> buffer(fileSize);

    file.seekg(0);

    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

VkShaderModule GraphicsPipeline::createShaderModule(
    const std::vector<char>& code)
{
    VkShaderModuleCreateInfo createInfo{};

    createInfo.sType =
        VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

    createInfo.codeSize = code.size();

    createInfo.pCode =
        reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;

    if (vkCreateShaderModule(
            device,
            &createInfo,
            nullptr,
            &shaderModule) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "No se pudo crear el Shader Module."
        );
    }

    return shaderModule;
}

