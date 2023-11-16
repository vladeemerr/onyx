#pragma once

#include "window.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace onyx {

class Win32Window final : public Window {
private:
	HWND handle_;

public:
	Win32Window(HINSTANCE instance, int32_t width, int32_t height, const std::string &title);

	void update() const;
};

} // namespace onyx
