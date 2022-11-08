#include "vk_init.h"
#include <stdlib.h>

#define GRAPHICS_BIT 0b00000001
#define PRESENT_BIT 0b00000010
#define CLEAR_ 0b0

typedef struct QueueFamilyIndices
{
    uint32_t graphicsFamily;
    uint32_t presentFamily;

    uint8_t foundSupport;
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
        printf("Instance creation was failed due to: %s\n", getErrorNameFromVkResult(&result));
        return 0;
    }

    return 1;
}

uint8_t supportedQueue(QueueFamilyIndices *indicies)
{
    return indicies->foundSupport & GRAPHICS_BIT && indicies->foundSupport & PRESENT_BIT;
}

QueueFamilyIndices findQueueFamilies(App *app, VkPhysicalDevice device)
{
    QueueFamilyIndices indicies;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);

    VkQueueFamilyProperties *queueFamilies = (VkQueueFamilyProperties *)malloc(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

    for (uint32_t i = 0; i < queueFamilyCount; i++)
    {
        indicies.foundSupport &= CLEAR_;

        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indicies.graphicsFamily = i;
            indicies.foundSupport |= GRAPHICS_BIT;
        }

        VkBool32 presentSupport = 0;

        if (presentSupport)
        {
            indicies.presentFamily = i;
            indicies.foundSupport |= PRESENT_BIT;
        }

        if (supportedQueue(&indicies))
        {
            break;
        }
    }

    free(queueFamilies);

    return indicies;
}

uint8_t
isDeviceSuitable(App *app, VkPhysicalDevice device)
{
    QueueFamilyIndices indicies = findQueueFamilies(app, device);

    VkPhysicalDeviceProperties deviceProps;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProps);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    return supportedQueue(&indicies);
}

VkPhysicalDevice pickPhysicalDevice(App *app)
{
    uint32_t deviceCount = 0;
    VkPhysicalDevice physicalDevice = NULL;

    vkEnumeratePhysicalDevices(app->instance, &deviceCount, NULL);

    if (deviceCount == 0)
    {
        printf("No devices has been found");
        return NULL;
    }

    VkPhysicalDevice *phyDevices = (VkPhysicalDevice *)malloc(sizeof(VkPhysicalDevice) * deviceCount);

    vkEnumeratePhysicalDevices(app->instance, &deviceCount, phyDevices);

    for (uint8_t i = 0; i < deviceCount; i++)
    {
        if (isDeviceSuitable(app, phyDevices[i]))
        {
            physicalDevice = phyDevices[i];
            free(phyDevices);
            break;
        }
    }

    if (physicalDevice == NULL)
    {
        printf("No suitable physical device has been found\n");
        return NULL;
    }

    return physicalDevice;
}

uint8_t createLogicalDevice(App *app, VkPhysicalDevice physicalDevice)
{
    QueueFamilyIndices indicies = findQueueFamilies(app, physicalDevice);
    float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo queueCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = indicies.graphicsFamily,
        .queueCount = 1,
        .pQueuePriorities = &queuePriority,
    };

    // TODO specify device features
    VkPhysicalDeviceFeatures deviceFeatures;

    VkDeviceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pQueueCreateInfos = &queueCreateInfo,
        .queueCreateInfoCount = 1,
        .pEnabledFeatures = &deviceFeatures,
        .enabledExtensionCount = 0,
    };

    VkResult result = vkCreateDevice(physicalDevice, &createInfo, NULL, &(app->device));

    if (result != VK_SUCCESS)
    {
        printf("Failed to create logical device due to: %s\n", getErrorNameFromVkResult(&result));
        return 0;
    }

    vkGetDeviceQueue(app->device, indicies.graphicsFamily, 0, &(app->graphicsQueue));

    return 1;
}

uint8_t initVulkan(App *app)
{
    if (!createInstance(app))
        return 0;

    VkPhysicalDevice physicalDevice = pickPhysicalDevice(app);

    if (!physicalDevice || !createLogicalDevice(app, physicalDevice))
        return 0;

    return 1;
}