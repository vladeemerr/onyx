#pragma once

namespace onyx {

class Renderer {
public:
	virtual ~Renderer() {}

	virtual void render() noexcept = 0;
};

} // namespace onyx
