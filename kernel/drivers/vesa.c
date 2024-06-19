#include "../include/vesa.h"

#define VESA_MEM_START 0x100000

int vesa_bios_call(u16 ax, u16 dx)
{
    u16 result;
    __asm__(
        "mov %0, %%ax\n"
        "mov %1, %%dx\n"
        "int $0x10\n"
        "mov %%ax, %0\n"
        : "=r"(result)
        : "r"(ax),
          "r"(dx)
        : "memory");

    return result;
}

static void *vesa_mem_ptr = (void *)VESA_MEM_START;
void *vesa_malloc(unsigned long size)
{
    void *ptr = vesa_mem_ptr;
    vesa_mem_ptr += size;

    if (vesa_mem_ptr > (void *)0xffffffff)
        return 0;

    return ptr;
}

int init_vesa(int desired_width, int desired_height)
{
    u16 ax = 0xF000;
    u16 dx = 0;
    int result = vesa_bios_call(ax, dx);
    if (result != 0x004F)
        return VESA_ERR_NOT_SUPPORTED;

    ax = 0x0F00;
    dx = (desired_width << 4) | desired_height;
    result = vesa_bios_call(ax, dx);
    if (result != 0x0000)
        return VESA_ERR_INVALID_MODE;

    ax = 0x0F00;
    dx = 0xE00;
    struct vesa_info *info = vesa_malloc(sizeof(struct vesa_info));
    result = vesa_bios_call(ax, dx);
    if (result != 0x0000)
        return VESA_ERR_INVALID_MODE;
}
