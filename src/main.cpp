#include "computer.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
	// Scene scene;
	size_t					 xSize = 500;
	size_t					 ySize = 500;

	std::string				 path = argc >= 2 ? argv[1] : "rt_test/basic_sphere.rt";
	Scene					 scene(path);
	FrameBuffer				 fb(xSize, ySize);
	Renderer				 renderer(xSize, ySize);
	std::vector<std::thread> threads(7);

	for (size_t i = 0; i < threads.capacity(); i++)
		threads[i] = std::thread(&Renderer::thread, &renderer, scene, &fb);
	for (auto& th : threads)
		th.join();

	fb.saveToBMP();
	return 0;
}
