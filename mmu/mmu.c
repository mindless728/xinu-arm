#include <mmu.h>
#include <paging.h>

unsigned int mmu_init(void) {
    paging_init();

    return 0;
}

void mmu_enable(void) {
    asm(
        //read control register in coprocessor 15
        "mrc p15, 0, r0, c1, c0, 0\n"

        //turn on the mmu enable bit
        "orr r0, r0, #0x1\n"

        //write back to control resgister in coprocessor 15
        "mcr p15, 0, r0, c1, c0, 0"
    );
}

void mmu_disable(void) {
    asm(
        //read control register in coprocessor 15
        "mrc p15, 0, r0, c1, c0, 0\n"

        //turn off the mmu enable bit
        "and r0, r0, #0xFFFFFFFE\n"

        //write back to control register in coprocessor 15
        "mcr p15, 0, r0, c1, c0, 0"
    );
}

unsigned int mmu_is_enabled(void) {
    register unsigned int r0 asm("r0");

    asm(
        //read control register in coprocessor 15
        "mrc p15, 0, r0, c1, c0, 0"
    );

    //only send back the mmu enabled bit
    return (r0 & 0x1);
}

void set_table_addr(void * addr) {
    register unsigned int r0 asm("r0");

    r0 = (unsigned int)addr;
    r0 &= 0xFFFFC000;

    asm(
        //write to Translation Table Base Register 0 (TTRB0)
        "mcr p15, 0, r0, c2, c0, 0"
    );
}

void * get_table_addr_virt(void) {
    //TODO get this from the current process that is running
    return NULL;
}

void * get_table_addr_phys(void) {
    register unsigned int r0 asm("r0");

    asm(
        //read from TTRB0 into r0
        "mrc p15, 0, r0, c2, c0, 0"
    );

    //get back the physical address of the translation table
    return (void *)(r0 & 0xFFFFC000);
}

unsigned int create_mapping(void * virt, void * phys) {
    //TODO create the damn mapping
    return 0;
}

unsigned int remove_mapping(void * virt) {
    //TODO strip out the damn mapping (L2 only though)
    return 0;
}

void * get_phys_addr(void * virt) {
    //TODO do a table walk in software
    return NULL;
}
