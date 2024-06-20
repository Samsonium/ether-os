ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)


SECTIONS
{
    GF = 0;
    . = 2M;

    .text ALIGN(4K) : AT(ADDR(.text) - GF)
    {
        *(.multiboot)
        *(.text)
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - GF)
    {
        *(.rodata)
    }

    .data ALIGN(4K) : AT(ADDR(.data) - GF)
    {
        *(.data)
    }

    .bss ALIGN(4K) : AT(ADDR(.bss) - GF)
    {
        *(COMMON)
        *(.bss)
    }

    kernel_end = .;
}
