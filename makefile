CFLAGS := -std=c11 -O0 -g -I./vendor/libsdl3/include\
	-Wall -Wextra -fno-diagnostics-show-caret\
	-DNX_USE_SHORT_NAMES
LDFLAGS := -lm
STATICLIBS := ./vendor/libsdl3/build/libSDL3.a

all: onyx

.c.o:
	cc -o $@ $(CFLAGS) -c $<

onyx: onyx.o graphics.o
	cc -o $@ $(LDFLAGS) $^ $(STATICLIBS)

