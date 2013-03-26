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

unsigned int * global_PAT; //global page allocation table
