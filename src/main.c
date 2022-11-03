#include "vk_types.h"

int main(int argc, char const *argv[])
{

    App app = newApp();

    if (!runApp(&app))
        return -1;

    return 0;
}
