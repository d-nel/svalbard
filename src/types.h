#pragma once

#include <stdint.h>
#include <stdlib.h>

#define u8  uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define s8  int8_t
#define s16 int16_t
#define s32 int32_t
#define s64 int64_t

#define f32 float
#define f64 double

#define usize size_t

#define array_sizeof(ARRAY) sizeof(ARRAY) / sizeof(ARRAY[0])

/*
#ifndef NULL
#define NULL 0
#endif
*/
