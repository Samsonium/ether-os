#ifndef VESA_H
#define VESA_H

#include "types.h"

#define VESA_CHECK_MAGIC 0xDEADBEEF

struct vesa_info
{
    u16 vesa_signature;
    u16 version;
    u32 oem_string[4];
    u8 capabilities;
    u8 reserved1;
    u8 video_mode_entries;
    u16 reserved2;
    u32 mode_list;
    u32 memory_model;
    u32 banked_memory;
    u32 vlfb_directory;
    u32 capabilities_extended;
    u32 reserved3[4];
    u8 video_mode_information[20];
};

enum vesa_error_codes
{
    VESA_ERR_NOT_SUPPORTED = 1,
    VESA_ERR_INVALID_MODE = 2,
    VESA_ERR_MALLOC = 3
};

int init_vesa(int desired_width, int desired_height);

#endif
