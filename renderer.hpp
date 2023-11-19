#pragma once

namespace onyx {

class Renderer {
public:
	virtual ~Renderer() {}

	virtual void render() {}
};

} // namespace onyx
