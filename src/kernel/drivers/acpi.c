#include <acpi.h>
#include <memory.h>
#include <debug.h>

#define RSDP_SIGNATURE "RSD PTR "

#define MADT_SIGNATURE "APIC"
#define MADT_CPU 0
#define MADT_IOAPIC 1
#define MADT_INT 2

struct rsdp
{
    uint8_t signature[8];
    uint8_t checksum;
    uint8_t OEMID[6];
    uint8_t revision;
    uint32_t rsdt;
    uint32_t length;
    uint64_t xsdt;
    uint8_t checksum2;
    uint8_t _[3];
} __attribute__((packed));

struct sdt
{
    uint8_t signature[8];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    uint8_t OEMID[6];
    uint8_t tableID[8];
    uint32_t OEM_revision;
    uint32_t creator;
    uint32_t creator_rev;
    uint8_t data[];
} __attribute__((packed));

struct madt
{
    uint32_t lic_address;
    uint32_t falgs;
    uint8_t data[];
} __attribute__((packed));

struct madt_entry
{
    uint8_t type;
    uint8_t length;

    union
    {
        struct
        {
            uint8_t id;
            uint8_t apic;
            uint32_t flags;
        } __attribute__((packed)) lapic;

        struct
        {
            uint8_t id;
            uint8_t _;
            uint32_t addr;
            uint32_t base;
        } __attribute__((packed)) ioapic;

        struct
        {
            uint8_t bus;
            uint8_t source;
            uint32_t target;
            uint16_t flags;
        } __attribute__((packed)) interrupt;
    };
} __attribute__((packed));

struct acpi_info acpi_info = {0};

static void *scan_rsdp(uint64_t start, uint64_t end)
{
    void *p = P2V(start);
    while (p < P2V(end))
    {
        if (!memcmp(p, RSDP_SIGNATURE, 8))
            return p;
        p = incptr(p, 16);
    }

    return 0;
}

static struct rsdp *find_rsdp()
{
    // Extended BIOS Data Area
    uint16_t *ebda_ptr = P2V(0x40E);
    uint64_t ebda = *ebda_ptr << 4;
    void *p = scan_rsdp(ebda, ebda + 1024);
    if (p)
        return p;

    // Scan from E0000 to FFFFF
    p = scan_rsdp(0xE0000, 0xFFFFF);
    if (p)
        return p;

    return 0;
}

static void parse_madt(struct madt *madt, uint32_t length)
{
    uintptr_t end = (uintptr_t)madt + length;
    struct madt_entry *e = (void *)madt->data;

    debug_info("Local interrupt controller: %x\n", madt->lic_address);

    while ((uintptr_t)e < end)
    {
        int i;
        switch (e->type)
        {
        case MADT_CPU:
            if (!(e->lapic.id & 1))
                break;
            i = acpi_info.num_cpus;
            acpi_info.cpu[i].id = e->lapic.id;
            acpi_info.cpu[i].apic = e->lapic.apic;
            acpi_info.num_cpus++;
            break;

        case MADT_IOAPIC:
            i = acpi_info.num_ioapic;
            acpi_info.ioapic[i].id = e->ioapic.id;
            acpi_info.ioapic[i].addr = e->ioapic.addr;
            acpi_info.ioapic[i].base = e->ioapic.base;
            acpi_info.num_ioapic++;
            break;

        case MADT_INT:
            acpi_info.int_map[e->interrupt.source] = e->interrupt.target;
            break;
        }

        debug_info(" MADT(type=%d, length=%d)\n", e->type, e->length);
        e = incptr(e, e->length);
    }
}

static void parse_sdt(struct sdt *sdt, uint8_t revision)
{
    uint32_t *p32 = (void *)sdt->data;
    uint64_t *p64 = (void *)sdt->data;

    int entries = (sdt->length - sizeof(struct sdt)) / (revision ? 8 : 4);
    for (int i = 0; i < entries; i++)
    {
        struct sdt *table = P2V(revision ? p64[i] : p32[i]);

        debug_info("Found table:");
        debug_putsn((char *)table->signature, 4);
        debug_printf("\n");

        if (!memcmp(table->signature, MADT_SIGNATURE, 4))
            parse_madt((void *)table->data, table->length);
    }
}

void acpi_init()
{
    struct rsdp *rsdp = find_rsdp();
    struct sdt *sdt = P2V(rsdp->revision ? rsdp->xsdt : rsdp->rsdt);
    parse_sdt(sdt, rsdp->revision);
}
