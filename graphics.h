#ifndef __ONYX_GRAPHICS_H__
#define __ONYX_GRAPHICS_H__

#include "common.h"

typedef struct nx_framebuffer nx_framebuffer;
struct nx_framebuffer {
	uint16_t width;
	uint16_t height;
	uint32_t* pixels;
};

void nx_fill_rect(const nx_framebuffer framebuffer,
                  const nx_int32x2 origin, const int32x2 size,
                  const uint32_t color);

void nx_draw_line(const nx_framebuffer framebuffer,
                  const nx_int32x2 points[2], const uint32_t color);

#endif // __ONYX_GRAPHICS_H__
