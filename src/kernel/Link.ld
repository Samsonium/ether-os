ENTRY(_start)

KERNEL_OFFSET = 0xFFFFFF8000000000;
KERNEL_START = 0x10000;

SECTIONS
{
    . = KERNEL_START + KERNEL_OFFSET;

    .text : AT(ADDR(.text) - KERNEL_OFFSET)
    {
        *(.multiboot)
        *(.text)
    }
}
