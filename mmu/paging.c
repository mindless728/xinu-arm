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

inline void * page_to_address(unsigned int page) {
    return (void *)(page << PAGE_SHIFT);
}

inline unsigned int address_to_page(void * address) {
    return ((unsigned int)address) >> PAGE_SHIFT;
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
    set_page_alloc_PAT(page, NULL);
}

void set_page_alloc_PAT(unsigned int page, unsigned int * PAT) {
    unsigned int index;
    unsigned int mask;

    index = get_page_index(page);
    mask = get_page_mask(page);

    global_PAT[index] |= mask;
    if(PAT != NULL)
        PAT[index] |= mask;
}

void unset_page_alloc(unsigned int page) {
    unset_page_alloc_PAT(page, NULL);
}

void unset_page_alloc_PAT(unsigned int page, unsigned int * PAT) {
    unsigned int index;
    unsigned int mask;

    index = get_page_index(page);
    mask = ~get_page_mask(page);

    global_PAT[index] &= mask;
    if(PAT != NULL)
        PAT[index] &= mask;
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

    //loop through the page numbers increasing by the alignment size in pages
    for(ret = 0; ret < number_pages; ret += alignment) {
        done = 0;
 
        //loop through the amount of pages needed for the allocation
        for(i = 0; i < amount; ++i) {
            if(!is_page_free(ret + i))
                break;

            //this deontes that enough pages have been found
            if(i == (amount - 1))
                done = 1;
        }
        if(done)
            break;
    }

    //there were not enoughp pages after the specified alignment
    if(ret >= number_pages)
        return SYSERR;

    //set the pages as allocated
    for(i = 0; i < amount; ++i)
        set_page_alloc(ret + i);

    return ret;
}

void dealloc_page(unsigned int page) {
    dealloc_page_PAT(page, NULL);
}

void dealloc_page_PAT(unsigned int page, unsigned int * PAT) {
    if(page > number_pages)
        return;

    unset_page_alloc_PAT(page, PAT);
}

void dealloc_pages(unsigned int page, unsigned int amount) {
    dealloc_pages_PAT(page, amount, NULL);
}

void dealloc_pages_PAT(unsigned int page, unsigned int amount, unsigned int * PAT) {
    int i;

    for(i = 0; i < amount; ++i)
        dealloc_page_PAT(page + i, PAT);
}

void dealloc_page_table(unsigned int * PAT) {
    unsigned int i;
    unsigned int indices;

    indices = number_pages >> 5;

    for(i = 0; i < indices; ++i)
        global_PAT[i] &= ~(PAT[i]);
}
