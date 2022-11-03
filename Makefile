SRC_FILES = src/main.c src/window.c src/app.c src/vk_init.c

LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

VulkanTest: $(SRC_FILES)
	gcc $(SRC_FILES) -o VulkanTest  $(LDFLAGS)

.PHONY: test clean

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest
