#pragma once

#include <stdint.h>

struct kboot_data_st
{
    int mb_version;
    char *bootloader;
    char *commandline;
};

extern struct kboot_data_st kboot_data;

int multiboot_init(uint64_t magic, void *mboot_info);
