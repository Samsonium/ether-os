#ifndef GDT_H
#define GDT_H

#include <stdbool.h>
#include "gdt_entry.h"
#include "gdt_desc.h"

void gdt_init();
void gdt_entry_add(int i, bool exec, uint8_t privelege);

#endif
