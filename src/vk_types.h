#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <stdio.h>

typedef struct App
{
    uint8_t isInitilized;

    GLFWwindow *window;
    VkInstance instance;
    VkDevice device;
    VkQueue graphicsQueue;

    VkSurfaceKHR surface;
} App;

App newApp();
uint8_t runApp(App *app);

// debug
char *getErrorNameFromVkResult(VkResult *result);
