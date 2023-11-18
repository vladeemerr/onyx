#pragma once

#include <cstdint>
#include <string>

namespace onyx {

class Window {
protected:
	int32_t width_, height_;
	std::string title_;
	bool closed_ = true;

public:
	Window(int32_t width, int32_t height, const std::string &title)
		: width_{width}, height_{height}, title_{title} {}

	Window(const Window &) = delete;
	Window(Window &&) noexcept = delete;
	Window &operator=(const Window &) = delete;
	Window &operator=(Window &&) noexcept = delete;

	inline void close() {
		closed_ = true;
	}

	inline bool isClosed() const {
		return closed_;
	}
};

} // namespace onyx
