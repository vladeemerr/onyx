#include "win32_window.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace {

LRESULT CALLBACK windowCallback(HWND handle, UINT message, WPARAM wparam, LPARAM lparam) {
	LRESULT result = 0;

	using onyx::Window;
	onyx::Window *window = reinterpret_cast<Window *>(GetWindowLongPtr(handle, GWLP_USERDATA));

	switch (message) {
	case WM_DESTROY:
	case WM_CLOSE: {
		window->close();
	} break;

	default: {
		result = DefWindowProcA(handle, message, wparam, lparam);
	} break;
	}

	return result;
}

} // namespace

namespace onyx {

Win32Window::Win32Window(HINSTANCE instance, int32_t width, int32_t height, const std::string &title)
: Window{width, height, title} {
	WNDCLASS window_class{
		.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
		.lpfnWndProc = windowCallback,
		.hInstance = instance,
		.hCursor = LoadCursor(0, IDC_ARROW),
		.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)),
		.lpszClassName = "onyx",
	};

	RegisterClassA(&window_class);

	handle_ = CreateWindowA(window_class.lpszClassName, title.c_str(), WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT, CW_USEDEFAULT, width, height,
							0, 0, instance, 0);

	SetWindowLongPtrA(handle_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	closed_ = ShowWindow(handle_, SW_SHOW);
}

void Win32Window::update() const {
	MSG message;

	while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

} // namespace onyx
