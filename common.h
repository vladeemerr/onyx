#ifndef __ONYX_COMMON_H__
#define __ONYX_COMMON_H__

#include <stdint.h>

#define NX_ARRAYLENGTH(STATIC_ARRAY) (sizeof(STATIC_ARRAY) / sizeof(*(STATIC_ARRAY)))

typedef struct nx_int32x2 nx_int32x2;
struct nx_int32x2 {
	int32_t x;
	int32_t y;
};

typedef struct nx_int32x3 nx_int32x3;
struct nx_int32x3 {
	int32_t x;
	int32_t y;
	int32_t z;
};

typedef struct nx_int32x4 nx_int32x4;
struct nx_int32x4 {
	int32_t x;
	int32_t y;
	int32_t z;
	int32_t w;
};

typedef struct nx_float32x2 nx_float32x2;
struct nx_float32x2 {
	float x;
	float y;
};

typedef struct nx_float32x3 nx_float32x3;
struct nx_float32x3 {
	float x;
	float y;
	float z;
};

typedef struct nx_float32x4 nx_float32x4;
struct nx_float32x4 {
	float x;
	float y;
	float z;
	float w;
};

#ifdef NX_USE_SHORT_NAMES
typedef nx_int32x2 int32x2;
typedef nx_int32x3 int32x3;
typedef nx_int32x4 int32x4;
typedef nx_float32x2 float32x2;
typedef nx_float32x3 float32x3;
typedef nx_float32x4 float32x4;
#endif // NX_USE_SHORT_NAMES

#endif // __ONYX_COMMON_H__
