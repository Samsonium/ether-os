#include "paging.h"

void init_paging()
{
    unsigned long address = 0;
    unsigned int i;

    for (i = 0; i < 1024; i++)
    {
        page_table[i] = address | 3;
        address = address + 4096;
    }

    page_directory[0] = page_table;
    page_directory[0] = page_directory[0] | 3;

    for (i = 1; i < 1024; i++)
        page_directory[i] = 0 | 2;

    write_cr3(page_directory);
    write_cr0(read_cr0() | 0x80000000);
}
