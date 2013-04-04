#include <mmu.h>
#include <paging.h>

unsigned int mmu_init(void) {
    paging_init();

    //mmu_enable();

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
    mmu_course_desc * table = (mmu_course_desc *)get_table_addr_virt();
    return create_mapping_ttb(virt, phys, table);
}

unsigned int create_mapping_ttb(void * virt, void * phys, mmu_course_desc * table) {
    unsigned int i;
    void * page;
    unsigned int course_index;
    mmu_course_desc * course_desc;
    mmu_small_desc * small_desc;
    mmu_course_desc * table_phys;
    int reenable_mmu = 0;

    //get the course description on the 4MB boundary
    course_index = get_course_index((void*)(((unsigned int)virt) & 0xFFC00000));
    course_desc = get_course_desc(course_index, table);

    //if there is an invalid entry, we need to allocate for the small tables for the next 4MB
    if(course_desc->type == MMU_NOMAP_TYPE) {
        //alloc a new page and get the address of it
        page = page_to_address(alloc_page());

        //go through each course entry (1MB of mapping) and give it an address to the page
        for(i = 0; i < 4; ++i) {
            course_desc = get_course_desc(course_index + i, table);
            course_desc->type = MMU_COURSE_TYPE;
            course_desc->SBZ = 0;
            course_desc->domain = 0;
            course_desc->IMP = 0;
            course_desc->addr = ((unsigned int)page + i * 0x400) >> 10; //shift by 10 to fit into bit field
        }
    }

    //get the physcal address to the course table
    table_phys = (mmu_course_desc *)get_phys_addr(table);

    if(mmu_is_enabled()) {
        reenable_mmu = 1;

        //turn off interuptis
        disable();

        //turn off the mmu
        mmu_disable();
    }

    //get the small descriptor from the index
    small_desc = get_small_desc(get_small_index(virt), table_phys);

    //set the fields of the descriptor
    small_desc->type = MMU_SMALL_TYPE;
    small_desc->bufferable = 0;
    small_desc->cacheable = 0;
    small_desc->ap = MMU_AP_NONE;
    small_desc->addr = ((unsigned int)phys) >> 12; //shift by 12 to fit into bit field

    if(reenable_mmu) {
        //turn on mmu
        mmu_enable();

        //turn on interupts
        enable();
    }
}

unsigned int remove_mapping(void * virt) {
    mmu_course_desc * table = (mmu_course_desc *)get_table_addr_virt();
    return remove_mapping_ttb(virt, table);
}

unsigned int remove_mapping_ttb(void * virt, mmu_course_desc * table) {
}

void * get_phys_addr(void * virt) {
    //TODO do a table walk in software
    return NULL;
}

inline unsigned int get_course_index(void * addr) {
    return ((unsigned int)addr) >> 20;
}

inline mmu_course_desc * get_course_desc(unsigned int index, mmu_course_desc * table) {
    return &(table[index]);
}

inline unsigned int get_small_index(void * addr) {
    return (((unsigned int)addr) >> 12) & 0xFF;
}

inline mmu_small_desc * get_small_desc(unsigned int index, mmu_small_desc * table) {
    return &(table[index]);
}
