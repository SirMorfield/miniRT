#include "computer.hpp"
#include <iostream>

int main() {
	// Scene scene;
	Scene	 scene("rt_test/basic_sphere.rt");
	Renderer renderer(500, 500);
	renderer.render(scene);
	renderer.saveToBMP();
	return 0;
}
