#include <stddef.h>
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
    mask = get_page_mask(page);

    global_PAT[index] &= mask;

    //TODO get process PAT an unset that
}

unsigned int alloc_page() {
    return alloc_pages_aligned(1, 1);
}

unsigned int alloc_page_aligned(unsigned int alignment) {
    return alloc_pages_aligned(1, alignment);
}

unsigned int alloc_pages(unsigned int amount) {
    return alloc_pages_aligned(amount, 1);
}

unsigned int alloc_pages_aligned(unsigned int amount, unsigned int alignment) {
    unsigned int ret;
    unsigned int i;
    unsigned int done;

    for(ret = 0; ret < number_pages; ret += alignment) {
        done = 0;
        for(i = 0; i < amount; ++i) {
            if(!is_page_free(ret + i))
                break;
            if(i == (amount - 1))
                done = 1;
        }
        if(done)
            break;
    }

    if(ret >= number_pages)
        return SYSERR;

    for(i = 0; i < amount; ++i)
        set_page_alloc(ret + i);

    return ret;
}

void dealloc_page(unsigned int page) {
}

void dealloc_pages(unsigned int page, unsigned int amount) {
}

void dealloc_page_table(unsigned int * PAT) {
    unsigned int i;
    unsigned int indices;

    indices = number_pages >> 5;

    for(i = 0; i < indices; ++i)
        global_PAT[i] &= ~(PAT[i]);
}
