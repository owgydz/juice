#include "paging.h"
#include "pmm.h"
#include "../lib/memory.h"

#define PAGE_PRESENT  1
#define PAGE_RW       2

static uint32_t* page_directory;
static uint32_t* first_page_table;

void paging_init(void)
{
    /* Allocate page directory */
    page_directory = (uint32_t*)pmm_alloc_page();
    memset(page_directory, 0, PAGE_SIZE);

    /* Allocate first page table */
    first_page_table = (uint32_t*)pmm_alloc_page();
    memset(first_page_table, 0, PAGE_SIZE);

    /* Identity map first 4MB */
    for (uint32_t i = 0; i < 1024; i++) {
        first_page_table[i] =
            (i * PAGE_SIZE) | PAGE_PRESENT | PAGE_RW;
    }

    /* Set first PDE */
    page_directory[0] =
        ((uint32_t)first_page_table) | PAGE_PRESENT | PAGE_RW;

    /* Load page directory into CR3 (32-bit safe) */
    uint32_t pd_phys = (uint32_t)page_directory;

    __asm__ volatile (
        "movl %0, %%cr3"
        :
        : "r"(pd_phys)
        : "memory"
    );

    /* Enable paging */
    uint32_t cr0;

    __asm__ volatile (
        "movl %%cr0, %0"
        : "=r"(cr0)
    );

    cr0 |= 0x80000000;   /* Set PG bit */

    __asm__ volatile (
        "movl %0, %%cr0"
        :
        : "r"(cr0)
        : "memory"
    );
}