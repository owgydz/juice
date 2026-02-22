#include "pmm.h"
#include "../lib/memory.h"

extern uint32_t __kernel_end;

static uint8_t* bitmap;
static uint32_t total_pages;
static uint32_t used_pages;
static uint32_t bitmap_size;

/* Bitmap helpers */

static inline void bitmap_set(uint32_t bit)
{
    bitmap[bit / 8] |= (1 << (bit % 8));
}

static inline void bitmap_clear(uint32_t bit)
{
    bitmap[bit / 8] &= ~(1 << (bit % 8));
}

static inline int bitmap_test(uint32_t bit)
{
    return bitmap[bit / 8] & (1 << (bit % 8));
}

static void mark_region_free(uint32_t base, uint32_t length)
{
    uint32_t start_page = base / PAGE_SIZE;
    uint32_t page_count = length / PAGE_SIZE;

    for (uint32_t i = 0; i < page_count; i++) {
        bitmap_clear(start_page + i);
        used_pages--;
    }
}

static void mark_region_used(uint32_t base, uint32_t length)
{
    uint32_t start_page = base / PAGE_SIZE;
    uint32_t page_count = length / PAGE_SIZE;

    for (uint32_t i = 0; i < page_count; i++) {
        bitmap_set(start_page + i);
        used_pages++;
    }
}

void pmm_init(multiboot_tag_mmap_t* mmap)
{
    /* Find highest memory address */
    uint64_t highest_addr = 0;

    for (multiboot_memory_map_entry_t* entry = mmap->entries;
         (uint8_t*)entry < (uint8_t*)mmap + mmap->size;
         entry = (multiboot_memory_map_entry_t*)
                 ((uint8_t*)entry + mmap->entry_size))
    {
        uint64_t end = entry->addr + entry->len;
        if (end > highest_addr)
            highest_addr = end;
    }

    total_pages = highest_addr / PAGE_SIZE;
    bitmap_size = total_pages / 8;

    /* Place bitmap after kernel */
    bitmap = (uint8_t*)(&__kernel_end);
    memset(bitmap, 0xFF, bitmap_size);

    used_pages = total_pages;

    /* Mark usable memory as free */
    for (multiboot_memory_map_entry_t* entry = mmap->entries;
         (uint8_t*)entry < (uint8_t*)mmap + mmap->size;
         entry = (multiboot_memory_map_entry_t*)
                 ((uint8_t*)entry + mmap->entry_size))
    {
        if (entry->type == 1) {
            mark_region_free((uint32_t)entry->addr,
                             (uint32_t)entry->len);
        }
    }

    /* Protect kernel memory */
    mark_region_used(0, (uint32_t)&__kernel_end);
}

void* pmm_alloc_page(void)
{
    for (uint32_t i = 0; i < total_pages; i++) {
        if (!bitmap_test(i)) {
            bitmap_set(i);
            used_pages++;
            return (void*)(i * PAGE_SIZE);
        }
    }
    return 0;
}

void pmm_free_page(void* page)
{
    uint32_t index = (uint32_t)page / PAGE_SIZE;

    if (bitmap_test(index)) {
        bitmap_clear(index);
        used_pages--;
    }
}

uint32_t pmm_get_total_pages(void)
{
    return total_pages;
}

uint32_t pmm_get_used_pages(void)
{
    return used_pages;
}