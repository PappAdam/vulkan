#include "window.h"

uint8_t initWindow(App *app)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    app->window = glfwCreateWindow(800, 600, "Vulkan", NULL, NULL);

    if (app->window == NULL)
    {
        printf("Couldn't initilze GLFW window");
        return 0;
    }

    return 1;
}
