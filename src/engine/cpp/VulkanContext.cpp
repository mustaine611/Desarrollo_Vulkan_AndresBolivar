#include "VulkanContext.h"

#include <stdexcept>
#include <vector>
#include <set>
#include <cstring>

void VulkanContext::initialize(GLFWwindow* window)
{
    createInstance();
    createSurface(window);
    pickPhysicalDevice();
    createLogicalDevice();

    swapChain.initialize(
        physicalDevice,
        device,
        surface,
        window
    );

    renderPass.initialize(
        device,
        swapChain.getImageFormat()
    );

    graphicsPipeline.initialize(
        device,
        swapChain.getExtent(),
        renderPass.getRenderPass()

        //todavia no hemos llamado al .readfile
    );
}

void VulkanContext::cleanup()
{


    if (device != VK_NULL_HANDLE)
    {
        graphicsPipeline.cleanup();
        swapChain.cleanup();
        vkDestroyDevice(device, nullptr);
    }

    if (surface != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR(instance, surface, nullptr);
    }

    if (instance != VK_NULL_HANDLE)
    {
        vkDestroyInstance(instance, nullptr);
    }
}

void VulkanContext::createInstance()
{
    VkApplicationInfo appInfo{};

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Mi Motor Vulkan";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Mi Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions =
        glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    createInfo.enabledLayerCount = 0;

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("No se pudo crear la instancia de Vulkan.");
    }
}

void VulkanContext::createSurface(GLFWwindow* window)
{
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
    {
        throw std::runtime_error("No se pudo crear la superficie de Vulkan.");
    }
}

void VulkanContext::pickPhysicalDevice()
{
    uint32_t deviceCount = 0;

    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        throw std::runtime_error("No se encontraron GPUs compatibles con Vulkan.");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);

    vkEnumeratePhysicalDevices(
        instance,
        &deviceCount,
        devices.data()
    );

    for (const auto& device : devices)
    {
        if (isDeviceSuitable(device))
        {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("No se encontró una GPU adecuada.");
    }
}

bool VulkanContext::isDeviceSuitable(VkPhysicalDevice device)
{
    QueueFamilyIndices indices = findQueueFamilies(device);

    return indices.isComplete() &&
       checkDeviceExtensionSupport(device);
}

VulkanContext::QueueFamilyIndices

VulkanContext::findQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(
        device,
        &queueFamilyCount,
        nullptr
    );

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);

    vkGetPhysicalDeviceQueueFamilyProperties(
        device,
        &queueFamilyCount,
        queueFamilies.data()
    );

    uint32_t i = 0;

    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = VK_FALSE;

        vkGetPhysicalDeviceSurfaceSupportKHR(
            device,
            i,
            surface,
            &presentSupport
        );

        if (presentSupport)
        {
            indices.presentFamily = i;
        }

        if (indices.isComplete())
        {
            break;
        }

        i++;
    }

    return indices;
}

bool VulkanContext::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount = 0;

    vkEnumerateDeviceExtensionProperties(
        device,
        nullptr,
        &extensionCount,
        nullptr
    );

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);

    vkEnumerateDeviceExtensionProperties(
        device,
        nullptr,
        &extensionCount,
        availableExtensions.data()
    );

    for (const auto& extension : availableExtensions)
    {
        if (strcmp(
                extension.extensionName,
                VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
        {
            return true;
        }
    }

    return false;
}

void VulkanContext::createLogicalDevice()
{
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    float queuePriority = 1.0f;

    std::set<uint32_t> uniqueQueueFamilies =
    {
        indices.graphicsFamily.value(),
        indices.presentFamily.value()
    };

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};

        queueCreateInfo.sType =
            VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        queueCreateInfos.push_back(queueCreateInfo);
    }
    VkPhysicalDeviceFeatures deviceFeatures{};

    std::vector<const char*> deviceExtensions =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkDeviceCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    
    createInfo.pQueueCreateInfos =
    queueCreateInfos.data();

    createInfo.queueCreateInfoCount =
    static_cast<uint32_t>(queueCreateInfos.size());

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount =
    static_cast<uint32_t>(deviceExtensions.size());

    createInfo.ppEnabledExtensionNames =
    deviceExtensions.data();

    createInfo.enabledLayerCount = 0;

    if (vkCreateDevice(
            physicalDevice,
            &createInfo,
            nullptr,
            &device) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "No se pudo crear el Logical Device.");
    }

    vkGetDeviceQueue(
        device,
        indices.graphicsFamily.value(),
        0,
        &graphicsQueue
    );

    vkGetDeviceQueue(
        device,
        indices.presentFamily.value(),
        0,
        &presentQueue
    );
}