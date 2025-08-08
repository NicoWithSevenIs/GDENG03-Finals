#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "Window/AppWindow.h"

#include "Math/Matrix4x4.h"


int main() {
	
	
	AppWindow app;

	if (app.init()) {
		while (app.isRunning()) {
			app.broadcast();

		}
	}
	

	return 0;
}