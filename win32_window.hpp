#pragma once

#include "window.hpp"

typedef struct HWND__ *HWND;
typedef struct HINSTANCE__ *HINSTANCE;

namespace onyx {

class Win32Window final : public Window {
private:
	HWND handle_;

public:
	Win32Window(HINSTANCE instance, int32_t width, int32_t height, const std::string &title);

	void update() const;

	inline HWND getHandle() const {
		return handle_;
	}
};

} // namespace onyx
