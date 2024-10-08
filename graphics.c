#include "graphics.h"

#include <stdio.h>
#include <math.h>

void nx_fill_rect(const nx_framebuffer framebuffer,
                  const nx_int32x2 origin, const int32x2 size,
                  const uint32_t color) {
	int32_t left = origin.x, top = origin.y, right, bottom;
	int32_t width = size.x, height = size.y;
	int32_t overflow;

	if (left < 0) {
		width += left;
		left = 0;
	}

	if (top < 0) {
		height += top;
		top = 0;
	}

	right = left + width;
	bottom = top + height;

	overflow = right - framebuffer.width;

	if (overflow > 0) {
		width -= overflow;
		right = framebuffer.width;
	}

	overflow = bottom - framebuffer.height;

	if (overflow > 0) {
		height -= overflow;
		bottom = framebuffer.height;
	}

	if (width * height <= 0)
		return;

	overflow = left;

	while (top < bottom) {
		while (left < right) {
			framebuffer.pixels[top * framebuffer.width + left] = color;
			++left;
		}

		left = overflow;
		++top;
	}
}

// TODO: Clipping
void nx_draw_line(const nx_framebuffer framebuffer,
                  const nx_int32x2 points[2], const uint32_t color) {
	int32x2 d = {points[1].x - points[0].x, points[1].y - points[0].y};
	int32x2 p = points[0];
	int32x2 ad, s;
	int32_t e, d2, d2d, g;

	if (d.x >= 0) {
		ad.x = d.x;
		s.x = 1;
	} else {
		ad.x = -d.x;
		s.x = -1;
	}

	if (d.y >= 0) {
		ad.y = d.y;
		s.y = 1;
	} else {
		ad.y = -d.y;
		s.y = -1;
	}

	if (ad.x >= ad.y) {
		e = points[1].x + s.x;
		d2 = 2 * ad.y;
		d2d = 2 * (ad.y - ad.x);
		g = d2 - ad.x;

		do {
			framebuffer.pixels[p.y * framebuffer.width + p.x] = color;

			if (g > 0) {
				p.y += s.y;
				g += d2d;
			} else {
				g += d2;
			}

			p.x += s.x;
		} while (p.x != e);
	} else {
		e = points[1].y + s.y;
		d2 = 2 * ad.x;
		d2d = 2 * (ad.x - ad.y);
		g = d2 - ad.y;

		do {
			framebuffer.pixels[p.y * framebuffer.width + p.x] = color;

			if (g > 0) {
				p.x += s.x;
				g += d2d;
			} else {
				g += d2;
			}

			p.y += s.y;
		} while (p.y != e);
	}
}
