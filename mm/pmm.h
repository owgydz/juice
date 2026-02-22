#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include "../arch/x86_64/boot/multiboot2.h"

#define PAGE_SIZE 4096

void pmm_init(multiboot_tag_mmap_t* mmap);
void* pmm_alloc_page(void);
void  pmm_free_page(void* page);

uint32_t pmm_get_total_pages(void);
uint32_t pmm_get_used_pages(void);

#endif