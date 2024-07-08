#include <acpi.h>
#include <memory.h>
#include <debug.h>

#define RSDP_SIGNATURE "RSD PTR "
#define MADT_SIGNATURE "APIC"

#define MADT_CPU 0
#define MADT_IOAPIC 1
#define MADT_INT 2

typedef struct rsdp
{
    uint8_t signature[8];
    uint8_t checksum;
    uint8_t OEMID[6];
    uint8_t revision;
    uint32_t rsdt;
    uint32_t length;
    uint64_t xsdt;
    uint8_t x_checksum;
    uint8_t reserved[3];
} __attribute__((packed)) rsdp_t;

typedef struct sdt
{
    uint8_t signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    uint8_t OEMID[6];
    uint8_t table_ID[8];
    uint32_t OEM_revision;
    uint32_t creator;
    uint32_t creator_rev;
    uint8_t data[];
} __attribute__((packed)) sdt_t;

typedef struct madt
{
    uint32_t lic_addr;
    uint32_t flags;
    uint8_t data[];
} __attribute__((packed)) madt_t;

typedef struct madt_entry
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
} __attribute__((packed)) madt_entry_t;

acpi_info_t acpi_info = {0};

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

static rsdp_t *find_rsdp()
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

static void parse_madt(madt_t *madt, uint32_t length)
{
    uintptr_t end = (uintptr_t)madt + length;
    madt_entry_t *e = (void *)madt->data;
    debug_info("Local interrupt controller: %x\n", madt->lic_addr);
    while ((uintptr_t)e < end)
    {
        int i;
        switch (e->type)
        {
        case MADT_CPU:
            debug_info("Added CPU\n");
            if (!(e->lapic.id & 1))
                break;
            i = acpi_info.num_cpus;
            acpi_info.cpu[i].id = e->lapic.id;
            acpi_info.cpu[i].apic = e->lapic.apic;
            acpi_info.num_cpus++;
            break;

        case MADT_IOAPIC:
            debug_info("Added IOAPIC\n");
            i = acpi_info.num_ioapic;
            acpi_info.ioapic[i].id = e->ioapic.id;
            acpi_info.ioapic[i].addr = e->ioapic.addr;
            acpi_info.ioapic[i].base = e->ioapic.base;
            acpi_info.num_ioapic++;
            break;

        case MADT_INT:
            debug_info("Added interrupt\n");
            acpi_info.int_map[e->interrupt.source] = e->interrupt.target;
            break;

        default:
            debug_warning("Unknown MADT entry %d\n", e->type);
        }

        //        debug_info(" MADT(type=%d, length=%d)\n", e->type, e->length);
        e = incptr(e, e->length);
    }
}

static void parse_sdt(sdt_t *sdt, uint8_t revision)
{
    uint32_t *p32 = (void *)sdt->data;
    uint64_t *p64 = (void *)sdt->data;

    int entries = (sdt->length - sizeof(sdt_t)) / (revision ? 8 : 4);
    debug_info("Found %d SDT entries\n", entries);

    for (int i = 0; i < entries; i++)
    {
        sdt_t *table = P2V(revision ? p64[i] : p32[i]);

        debug_info("Found table: ");
        debug_putsn((char *)table->signature, 4);
        debug_printf("\n");

        if (!memcmp(table->signature, MADT_SIGNATURE, 4))
            parse_madt((void *)table->data, table->length);
    }
}

void acpi_init()
{
    rsdp_t *rsdp = find_rsdp();
    debug_info("ACPI version: %s\n", rsdp->revision == 2 ? "2+ (XSDP)" : "1.0 (RSDP)");

    sdt_t *s = P2V(rsdp->revision == 2 ? rsdp->xsdt : rsdp->rsdt);
    parse_sdt(s, rsdp->revision);
}
