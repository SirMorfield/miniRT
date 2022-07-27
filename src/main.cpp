#include "computer.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
	// Scene scene;

	std::string				 path = argc >= 2 ? argv[1] : "rt_test/basic_sphere.rt";
	Scene					 scene(path);
	Frame_buffer			 fb(scene.resolution.width(), scene.resolution.height());
	Renderer				 renderer(scene.resolution.width(), scene.resolution.height());
	std::vector<std::thread> threads(7);

	std::cout << scene << std::endl;
	for (size_t i = 0; i < threads.capacity(); i++)
		threads[i] = std::thread(&Renderer::thread, &renderer, scene, &fb);
	for (auto& th : threads)
		th.join();

	fb.save_to_BMP();
	return 0;
}
