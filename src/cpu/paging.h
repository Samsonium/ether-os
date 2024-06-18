#ifndef PAGING_H
#define PAGING_H

#include "types.h"

#define NUM_PAGES 1024
#define PAGE_FRAME_SIZE 4096

#define PRESENT 1
#define PAGE_READONLY 0
#define PAGE_RW 1
#define PAGE_USER 1
#define PAGE_KERNEL 0
#define PAGE_SIZE_4KB 0
#define PAGE_SIZE_4MB 1

typedef struct page
{
    u32 present : 1;
    u32 rw : 1;
    u32 user : 1;
    u32 accessed : 1;
    u32 dirty : 1;
    u32 unused : 7;
    u32 frame : 20;

} page_t;

typedef struct page_table
{
    page_t pages[NUM_PAGES] __attribute__((aligned(PAGE_FRAME_SIZE)));
} page_table_t;

typedef struct page_directory
{
    page_table_t *tables[4096];
    u32 tablesPhysical[1024];
    u32 physicalAddr;
} page_directory_t;

void init_paging();
void page_fault();

#endif
