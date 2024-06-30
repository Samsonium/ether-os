#include <multiboot.h>
#include <memory.h>
#include <debug.h>

#define MBOOT_REPLY 0x36D76289

struct taglist
{
    uint32_t total_size;
    uint32_t reserved;
} __attribute__((packed));

struct tag
{
    uint32_t type;
    uint32_t size;
    uint8_t data[];
} __attribute__((packed));

struct mmap_entry
{
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t reserved;
} __attribute__((packed));

struct mmap
{
    uint32_t entry_size;
    uint32_t entry_version;
    struct mmap_entry entries[];
} __attribute__((packed));

#define MBOOT2_COMMANDLINE 1
#define MBOOT2_BOOTLOADER 2
#define MBOOT2_MMAP 6

struct kboot_data_st kboot_data;

int parse_multiboot2(struct taglist *tags)
{
    struct tag *tag = incptr(tags, sizeof(struct taglist));
    struct mmap *mmap;
    while (tag->type)
    {
        switch (tag->type)
        {
        case MBOOT2_BOOTLOADER:
            kboot_data.bootloader = (char *)tag->data;
            break;

        case MBOOT2_COMMANDLINE:
            kboot_data.commandline = (char *)tag->data;
            break;

        case MBOOT2_MMAP:
            mmap = kboot_data.mmap = (void *)tag->data;
            kboot_data.mmap_length = (tag->size - 8) / mmap->entry_size;
            kboot_data.mmap_size = (tag->size - 8);
            break;

        default:
            debug_warning("Unknown multiboot2 tag type: %d \n", tag->type);
        }

        int padded_size = tag->size + ((tag->size % 8) ? (8 - (tag->size % 8)) : 0);
        tag = incptr(tag, padded_size);
    }

    return 0;
}

int multiboot_init(uint64_t magic, void *mboot_info)
{
    if (magic == MBOOT_REPLY)
    {
        kboot_data.mb_version = 2;
        parse_multiboot2(mboot_info);
    }
    else
        return 1;

    return 0;
}

int multiboot_get_mem_area(size_t count, uintptr_t *start, uintptr_t *end, uint32_t *type)
{
    if (count >= kboot_data.mmap_length)
        return 1;

    struct mmap *mmap = kboot_data.mmap;
    struct mmap_entry *entry = mmap->entries;
    entry = incptr(entry, count * mmap->entry_size);

    *start = entry->base;
    *end = entry->base + entry->length;
    *type = entry->type;

    return 0;
}

int multiboot_page_used(uintptr_t start)
{
#define overlap(st, len) ((uintptr_t)st < (start + PAGE_SIZE) && start <= (uintptr_t)st + len)
    if (
        overlap(kboot_data.bootloader, strlen(kboot_data.bootloader)) ||
        overlap(kboot_data.commandline, strlen(kboot_data.commandline)) ||
        overlap(kboot_data.mmap, kboot_data.mmap_size) ||
        0)
        return 1;
    return 0;
}
