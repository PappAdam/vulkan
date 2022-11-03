#include "vk_init.h"
#include <stdlib.h>

typedef struct QueueFamilyIndices
{
    uint32_t graphicsFamily;
    uint8_t foundGraphicsFam;
} QueueFamilyIndices;

uint8_t createInstance(App *app)
{
    VkApplicationInfo appInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Test Triangle App",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = NULL,
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_0,
    };

    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    if (glfwExtensions == NULL)
    {
        printf("Couldn't get required extensions\n");
        return 0;
    }

    VkInstanceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo,
        .ppEnabledExtensionNames = glfwExtensions,
        .enabledExtensionCount = glfwExtensionCount,
        .enabledLayerCount = 0,
    };

    VkResult result = vkCreateInstance(&createInfo, NULL, &(app->instance));

    if (result != VK_SUCCESS)
    {
        printf("Instance creation was failed due to: %s\n", getErrorNameFromVkResult(result));
        return 0;
    }

    return 1;
}

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indicies;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);

    VkQueueFamilyProperties *queueFamilies = (VkQueueFamilyProperties *)malloc(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

    for (uint32_t i = 0; i < queueFamilyCount; i++)
    {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indicies.graphicsFamily = i;
            indicies.foundGraphicsFam = 1;
            break;
        }
    }

    free(queueFamilies);

    return indicies;
}

uint8_t
isDeviceSuitable(VkPhysicalDevice device)
{
    QueueFamilyIndices indicies = findQueueFamilies(device);

    VkPhysicalDeviceProperties deviceProps;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProps);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    puts("Device found:");
    printf("   Physical device name: \n\t%s\n\t%i\n", deviceProps.deviceName, deviceProps.deviceType);

    return indicies.foundGraphicsFam;
}

uint8_t pickPhysicalDevice(App *app)
{
    uint32_t deviceCount = 0;
    VkPhysicalDevice physicalDevice = NULL;

    vkEnumeratePhysicalDevices(app->instance, &deviceCount, NULL);

    if (deviceCount == 0)
    {
        printf("No devices has been found");
        return 0;
    }

    VkPhysicalDevice *phyDevices = (VkPhysicalDevice *)malloc(sizeof(VkPhysicalDevice) * deviceCount);

    vkEnumeratePhysicalDevices(app->instance, &deviceCount, phyDevices);

    for (uint8_t i = 0; i < deviceCount; i++)
    {
        if (isDeviceSuitable(phyDevices[i]))
        {
            physicalDevice = phyDevices[i];
            free(phyDevices);
            break;
        }
    }

    if (physicalDevice == NULL)
    {
        printf("No suitable physical device has been found (device requirements: discrateGPU, geometry shader support)");
        return 0;
    }

    return 1;
}

uint8_t initVulkan(App *app)
{
    if (!createInstance(app) || !pickPhysicalDevice(app))
        return 0;

    return 1;
}