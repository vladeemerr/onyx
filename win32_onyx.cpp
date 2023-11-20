#include "win32_window.hpp"
#include "d3d11_renderer.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int) {
	onyx::Win32Window window{instance, 1280, 720, "Onyx Engine"};
	
	onyx::D3D11Renderer renderer{window.getHandle()};

	while (!window.isClosed()) {
		window.update();
		renderer.render();
	}

	return 0;
}
