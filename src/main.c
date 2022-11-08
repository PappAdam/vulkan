#include "vk_types.h"

int main(int argc, char const *argv[])
{

    App app = newApp();

    if (!app.isInitilized)
    {
        printf("App is not initilized properly\n");
        return -1;
    }

    runApp(&app);

    return 0;
}
