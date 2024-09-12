CFLAGS := -std=c11 -O0 -g -Wall -Wextra -fno-diagnostics-show-caret
LDFLAGS := -lm -lSDL2

all: onyx

.c.o:
	cc -o $@ $(CFLAGS) -c $<

onyx: onyx.o
	cc -o $@ $(LDFLAGS) $^

