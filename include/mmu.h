#ifndef MMU_H
#define MMU_H

#include <stddef.h>

struct mmu_course_desc {
    uint type           : 2 ;
    uint SBZ            : 3 ;
    uint domain         : 4 ;
    uint IMP            : 1 ;
    uint addr           : 22;
};

struct mmu_small_desc {
    uint type           : 2 ;
    uint bufferable     : 1 ;
    uint cacheable      : 1 ;
    uint ap             : 8 ;
    uint addr           : 20;
};

unsigned int mmu_init(void);
void mmu_enable(void);
void mmu_disable(void);
unsigned int mmu_is_enabled(void);

void set_table_addr(void * addr);
void * get_table_addr_virt(void);
void * get_table_addr_phys(void);

unsigned int create_mapping(void * virt, void * phys);
unsigned int remove_mapping(void * virt);
void * get_phys_addr(void * virt);

#endif
