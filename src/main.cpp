#include "computer.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
	// Scene scene;
	std::string path = argc == 2 ? argv[1] : "rt_test/basic_sphere.rt";
	Scene		scene(path);
	Renderer	renderer(400, 400);
	renderer.render(scene);
	renderer.saveToBMP();
	return 0;
}
