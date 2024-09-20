#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <SDL3/SDL.h>

#include "graphics.h"

#define EXIT_WITH_ERROR(MESSAGE) do { \
	fprintf(stderr, "%s@%s:%d: %s\n", __FUNCTION__, __FILE__, __LINE__, MESSAGE); \
	exit(EXIT_FAILURE); \
} while (0)

enum {
	DEFAULT_WINDOW_WIDTH = 1280,
	DEFAULT_WINDOW_HEIGHT = 720,
};

static const char DEFAULT_WINDOW_TITLE[] = "Onyx";

static volatile bool running = true;

static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_Texture* backbuffer;

static nx_framebuffer frontbuffer;

int main(void) {
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
		EXIT_WITH_ERROR(SDL_GetError());
	}

	window = SDL_CreateWindow(DEFAULT_WINDOW_TITLE,
	                          DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
	                          0u);
	if (!window) {
		EXIT_WITH_ERROR(SDL_GetError());
	}

	renderer = SDL_CreateRenderer(window, NULL);
	if (!renderer) {
		EXIT_WITH_ERROR(SDL_GetError());
	}


	backbuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XRGB8888,
	                               SDL_TEXTUREACCESS_STREAMING,
	                               DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
	if (!backbuffer) {
		EXIT_WITH_ERROR(SDL_GetError());
	}

	frontbuffer.width = DEFAULT_WINDOW_WIDTH;
	frontbuffer.height = DEFAULT_WINDOW_HEIGHT;

	while (running) {
		uint64_t time = SDL_GetTicks();

		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
			switch (ev.type) {
				case SDL_EVENT_QUIT:
					running = false;
					break;
			}
		}

		uint32_t* pixels = NULL;
		int32_t pitch = 0;
		SDL_LockTexture(backbuffer, NULL, (void**)&pixels, &pitch); {
			frontbuffer.pixels = pixels;

			SDL_memset4(frontbuffer.pixels, 0x101010,
			            frontbuffer.width * frontbuffer.height);

			int32x2 line[2] = {{frontbuffer.width / 2, frontbuffer.height / 2}};
			line[1].x = line[0].x + (int32_t)(256.0f * sinf(time * 0.0001f));
			line[1].y = line[0].y + (int32_t)(256.0f * cosf(time * 0.0001f));

			nx_fill_rect(frontbuffer, (int32x2){line[0].x - 1, line[0].y - 1},
			             (int32x2){3, 3}, 0xffff0000);
			nx_fill_rect(frontbuffer, (int32x2){line[1].x - 1, line[1].y - 1},
			             (int32x2){3, 3}, 0xff0000ff);
			nx_draw_line(frontbuffer, line, 0xffffffff);
		} SDL_UnlockTexture(backbuffer);

		SDL_RenderTexture(renderer, backbuffer, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(backbuffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
