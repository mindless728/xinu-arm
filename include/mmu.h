#ifndef MMU_H
#define MMU_H

#include <stddef.h>

//mapping types for the table entries (both course and small)
#define MMU_NOMAP_TYPE      0x0
#define MMU_COURSE_TYPE     0x1
#define MMU_SMALL_TYPE      0x2

//access permissions for small table entries
#define MMU_AP_NONE         0x55
#define MMU_AP_READ         0xAA
#define MMU_AP_RDWR         0xFF

//definitions of virtual memory locations of certain objects
#define VIRT_HEAP_LOCATION      0x10000000
#define VIRT_STACK_LOCATION     0x30000000

//variables needed to book keep some things
extern void * kheap_next_addr;
extern void * kernel_L1_table;

typedef struct _mmu_course_desc {
    uint type           : 2 ;
    uint SBZ            : 3 ;
    uint domain         : 4 ;
    uint IMP            : 1 ;
    uint addr           : 22;
} mmu_course_desc;

typedef struct mmu_small_desc {
    uint type           : 2 ;
    uint bufferable     : 1 ;
    uint cacheable      : 1 ;
    uint ap             : 8 ;
    uint addr           : 20;
} mmu_small_desc;

struct mmu_info {
    void * mmu_ttb_virt;
    void * mmu_ttb_phys;
};

unsigned int mmu_init(void);
void mmu_enable(void);
void mmu_disable(void);
unsigned int mmu_is_enabled(void);

void set_table_addr(void * addr);
void * get_table_addr_virt(void);
void * get_table_addr_phys(void);

unsigned int create_mapping(void * virt, void * phys);
unsigned int create_mapping_ttb(void * virt, void * phys, mmu_course_desc * table);
unsigned int remove_mapping(void * virt);
unsigned int remove_mapping_ttb(void * virt, mmu_course_desc * table);
void * get_phys_addr(void * virt);

inline unsigned int get_course_index(void * addr);
inline mmu_course_desc * get_course_desc(unsigned int index, mmu_course_desc * table);

inline unsigned int get_small_index(void * addr);
inline mmu_small_desc * get_small_desc(unsigned int index, mmu_small_desc * table);

#endif
