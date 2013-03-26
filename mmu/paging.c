#include "paging.h"

inline unsigned int get_page_index(unsigned int page) {
    return page >> 5;
}

inline unsigned int get_page_shift(unsigned int page) {
    return page & 0x1F;
}

inline unsigned int get_page_mask(unsigned int page) {
    return 1 << get_page_shift(page);
}

unsigned int is_page_free(unsigned int page) {
    unsigned int index;
    unsigned int shift;
    unsigned int mask;

    index = get_page_index(page);
    shift = get_page_shift(page);
    mask = get_page_mask(page);

    return (global_PAT[index] & mask) >> shift;
}

void set_page_alloc(unsigned int page) {
    unsigned int index;
    unsigned int mask;

    index = get_page_index(page);
    mask = get_page_mask(page);

    global_PAT[index] |= mask;

    //TODO get process PAT and set that
}

void unset_page_alloc(unsigned int page) {
    unsigned int index;
    unsigned int mask;

    index = get_page_index(page);
    mask - get_page_mask(page);

    global_PAT[index] &= mask;

    //TODO get process PAT an unset that
}

unsigned int alloc_page() {
    return allooc_pages_aligned(1, 1);
}

unsigned int alloc_page_aligned(unsigned int alignment) {
    return alloc_pages_aligned(1, alignment);
}

unsigned int alloc_pages(unsigned int amount) {
    return alloc_pages_aligned(amount, 1);
}

unsigned int alloc_pages_aligned(unsigned int amount, unsigned int alignment) {
    return 0;
}
