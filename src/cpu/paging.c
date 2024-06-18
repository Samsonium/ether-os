#include "paging.h"
#include "../drivers/screen.h"

extern void enable_paging(u32 *);

u32 page_directory[NUM_PAGES] __attribute__((aligned(PAGE_FRAME_SIZE)));
u32 page_table[NUM_PAGES] __attribute__((aligned(PAGE_FRAME_SIZE)));

void init_paging()
{
    s32 i;

    // Create page directory (supervisor mode) in RW not present mode (0x010 == 2)
    for (i = 0; i < NUM_PAGES; i++)
        page_directory[i] = 0x00000002;

    // Create page table (supervisor mode) in RW present mode (0x011 == 3)
    for (i = 0; i < NUM_PAGES; i++)
        page_table[i] = (i * 0x1000) | 3;

    // Put page table into page directory supervisor level, RW present
    page_directory[0] = ((s32)page_table) | 3;

    enable_paging(page_directory);
}

void page_fault()
{
    char msg[] = "Page error";
    kprint(msg, get_color(RED, BLACK));
}
