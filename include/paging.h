#ifndef PAGING_H
#define PAGING_H

#include <stddef.h>
#include <mmu.h>

inline unsigned int get_page_index(unsigned int page);
inline unsigned int get_page_shift(unsigned int page);
inline unsigned int get_page_mask(unsigned int page);
inline void * page_to_address(unsigned int page);
inline unsigned int address_to_page(void * address);

unsigned int is_page_free(unsigned int piage);
void set_page_alloc(unsigned int page);
void set_page_alloc_PAT(unsigned int page, unsigned int * PAT);
void unset_page_alloc(unsigned int page);
void unset_page_alloc_PAT(unsigned int page, unsigned int * PAT);

unsigned int alloc_page(void);
unsigned int alloc_page_aligned(unsigned int alignment);
unsigned int alloc_pages(unsigned int amount);
unsigned int alloc_pages_aligned(unsigned int amount, unsigned int alignment);

void dealloc_page(unsigned int page);
void dealloc_page_PAT(unsigned int page, unsigned int * PAT);
void dealloc_pages(unsigned int page, unsigned int amount);
void dealloc_pages_PAT(unsigned int page, unsigned int amount, unsigned int * PAT);
void dealloc_page_table(unsigned int * PAT);

#endif
