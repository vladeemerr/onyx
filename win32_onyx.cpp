#include "win32_window.hpp"

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int) {
	onyx::Win32Window window{instance, 1280, 720, "Onyx Engine"};

	while (!window.isClosed()) {
		window.update();
	}

	return 0;
}
