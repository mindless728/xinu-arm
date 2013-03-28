#include <tlb.h>

void tlb_init(void) {
}

void tlb_flush(void) {
    register unsigned int r0 asm("r0");
    
    r0 = 0;

    asm(
        "mcr p15, 0, r0, c8, c7, 0"
    );
}
