#ifndef TYPES_H
#define TYPES_H

typedef unsigned int u32;
typedef int s32;

typedef unsigned short u16;
typedef short s16;

typedef unsigned char u8;
typedef char s8;

typedef unsigned int size_t;

#define NULL ((void *)0)
#define true 1
#define false 0

#define low_16(address) (u16)((address) & 0xFFFF)
#define high_16(address) (u16)(((address) >> 16) & 0xFFFF)

#endif
