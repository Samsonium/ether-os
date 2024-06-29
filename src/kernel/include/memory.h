#pragma once

#define KERNEL_OFFSET 0xFFFFFF8000000000

#ifdef __ASSEMBLER__
#define V2P(a) ((a) - KERNEL_OFFSET)
#define P2V(a) ((a) + KERNEL_OFFSET)
#else
#include <stdint.h>
#define V2P(a) ((uintptr_t)(a) & ~KERNEL_OFFSET)
#define P2V(a) ((void *)((uintptr_t)(a) | KERNEL_OFFSET))
#define incptr(p, n) ((void *)(((uintptr_t)(p)) + (n)))
#endif

#define P1_OFFSET(a) (((a) >> 12) & 0x1FF)
#define P2_OFFSET(a) (((a) >> 21) & 0x1FF)
#define P3_OFFSET(a) (((a) >> 30) & 0x1FF)
#define P4_OFFSET(a) (((a) >> 39) & 0x1FF)

#define PAGE_PRESENT 0x001
#define PAGE_WRITE 0x002
#define PAGE_USER 0x004
#define PAGE_HUGE 0x080
#define PAGE_GLOBAL 0x100

#define PAGE_SIZE 0x1000
#define ENTRIES_PER_PT 512

#ifndef __ASSEMBLER__
#include <stddef.h>

// extern uint64_t kernel_p4;

// void *memcpy(void *, const void *, size_t);
// void *memset(void *, int, size_t);
// void *memmove(void *, const void *, size_t);
// int memcmp(const void *, const void *, size_t);
// size_t strlen(const char *);

// void pmm_free(uint64_t);
// uint64_t pmm_alloc();
// uint64_t pmm_callc();

// uint64_t new_P4();
// uint64_t vmm_get_page(uint64_t, uint64_t);

// #define PAGE_EXISTS(p) ((p) != (uint64_t) - 1)

// int vmm_set_page(uint64_t, uint64_t, uint64_t, uint16_t);
// void vmm_clear_page(uint64_t, uint64_t, int);
// size_t memcpy_to_p4(uint64_t, void *, void *, size_t);
// size_t memcpy_from_p4(void *, uint64_t, void *, size_t);

// extern union PTE BootP4;
// extern int kernel_start, kernel_end;

// void memory_init();

#endif
