#pragma once

#include <stdint.h>
#include <stddef.h>

#define MMAP_FREE 1

struct kboot_data_st
{
    int mb_version;
    char *bootloader;
    char *commandline;
    size_t mmap_size;
    unsigned int mmap_length;
    void *mmap;
};

extern struct kboot_data_st kboot_data;

int multiboot_init(uint64_t magic, void *mboot_info);
int multiboot_get_mem_area(size_t count, uintptr_t *start, uintptr_t *end, uint32_t *type);
int multiboot_page_used(uintptr_t start);
