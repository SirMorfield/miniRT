#include "main.hpp"

int main() {
	Scene	 scene;
	Renderer renderer(500, 500);
	renderer.render(scene);
	renderer.saveToBMP();
	return 0;
}
