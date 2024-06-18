#ifndef MM_H
#define MM_H

unsigned long *page_directory = (unsigned long *)0x9C000;
unsigned long *page_table = (unsigned long *)0x9D000;

void init_paging();

extern void write_cr0(unsigned long v);
extern void write_cr3(unsigned long v);
extern unsigned long read_cr0();
extern unsigned long read_cr3();

#endif
