#include "computer.hpp"
#include "env.hpp"
#include <iostream>
#include <unistd.h>

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;

	std::string				 path = argc >= 2 ? argv[1] : "rt_test/basic_sphere.rt";
	Scene					 scene(path);
	Frame_buffer			 fb(scene.resolution.width(), scene.resolution.height());
	Renderer				 renderer(scene.resolution.width(), scene.resolution.height());
	std::vector<std::thread> threads(env::threads);

	std::cout << scene << std::endl;
	env::print();

	for (size_t i = 0; i < threads.capacity(); i++)
		threads[i] = std::thread(&Renderer::thread, &renderer, scene, &fb);

	Time time;
	for (auto& th : threads)
		th.join();
	time.print();

	fb.save_to_BMP();
	return 0;
}
