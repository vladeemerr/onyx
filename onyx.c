#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>

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

int main(void) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		EXIT_WITH_ERROR(SDL_GetError());
	}

	window = SDL_CreateWindow(DEFAULT_WINDOW_TITLE,
	                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                          DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
	                          0u);
	if (!window) {
		EXIT_WITH_ERROR(SDL_GetError());
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		EXIT_WITH_ERROR(SDL_GetError());
	}

	backbuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XRGB8888,
	                               SDL_TEXTUREACCESS_STREAMING,
	                               DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
	if (!backbuffer) {
		EXIT_WITH_ERROR(SDL_GetError());
	}

	while (running) {
		uint64_t time = SDL_GetTicks64();

		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
			switch (ev.type) {
				case SDL_QUIT:
					running = false;
					break;
			}
		}

		uint32_t* pixels = NULL;
		int32_t pitch = 0;
		SDL_LockTexture(backbuffer, NULL, (void**)&pixels, &pitch); {
			SDL_memset4(pixels, 0x101010, DEFAULT_WINDOW_WIDTH * DEFAULT_WINDOW_HEIGHT);
		} SDL_UnlockTexture(backbuffer);

		SDL_RenderCopy(renderer, backbuffer, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
}
