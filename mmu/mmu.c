#include <mmu.h>
#include <paging.h>

unsigned int mmu_init() {
    struct mmu_course_desc L1;

    paging_init();

    L1.type = 0;
    L1.SBZ = 0;
    L1.domain = 0;
    L1.IMP = 0;
    L1.addr = 0x3FFFFF;

    //test that the address is the top part of the struct
    kprintf("%x\n",*((int *)(&L1)));
}
