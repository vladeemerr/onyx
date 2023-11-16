#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace {

bool running = true;

LRESULT CALLBACK windowCallback(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
	LRESULT result = 0;

	switch (message) {
	case WM_DESTROY:
	case WM_CLOSE: {
		running = false;
	} break;

	default: {
		result = DefWindowProcA(window, message, wparam, lparam);
	} break;
	}

	return result;
}

} // namespace

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int) {
	WNDCLASS window_class{};

	window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window_class.lpfnWndProc = windowCallback;
	window_class.hInstance = instance;
	window_class.hCursor = LoadCursor(0, IDC_ARROW);
	window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	window_class.lpszClassName = "onyx";

	RegisterClassA(&window_class);
	HWND window = CreateWindowA(window_class.lpszClassName, "Onyx Engine", WS_OVERLAPPEDWINDOW,
								CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
								0, 0, instance, 0);

	ShowWindow(window, SW_SHOW);

	while (running) {
		MSG message;
		while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	return 0;
}
