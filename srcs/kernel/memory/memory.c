/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 15:47:20 by lubenard          #+#    #+#             */
/*   Updated: 2022/08/18 02:12:34 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory.h"
#include "../../lib/iolib.h"
#include "../../lib/memlib.h"
#include "../../lib/bitwiselib.h"
#include "grub/grub.h"
#include "pmm/pmm.h"
#include "vmm/vmm.h"

# define MB 1048576
# define GB 1073741824

extern uint32_t endKernel;
extern uint32_t startKernel;

uint32_t static end_kernel_addr = (uint32_t) &endKernel;
uint32_t static start_kernel_addr = (uint32_t) &startKernel;

uint32_t get_kernel_size() {
	return end_kernel_addr - start_kernel_addr;
}

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

void init_memory(multiboot_info_t *mb_mmap) {
	uint32_t nframes;

	multiboot_memory_map_t *map_entry = get_memory_map_from_grub(mb_mmap);

	// Getting the size of the memory via grub.
	// nframes is the number of frames (aka blocks of 4096)
	nframes = (map_entry->len_low / 0x1000); // Ox1000 is 4096 in hexa (Size of one page)

    if (CHECK_FLAG (mb_mmap->flags, 4) && CHECK_FLAG (mb_mmap->flags, 5)) {
        printk(KERN_INFO, "Both bits 4 and 5 are set.");
    }

    if (CHECK_FLAG (mb_mmap->flags, 4)) {
        multiboot_aout_symbol_table_t *multiboot_aout_sym = &(mb_mmap->u.aout_sym);

        printk(KERN_INFO, "multiboot_aout_symbol_table: tabsize = 0x%x, "
                "strsize = 0x%x, addr = 0x%x",
                (unsigned) multiboot_aout_sym->tabsize,
                (unsigned) multiboot_aout_sym->strsize,
                (unsigned) multiboot_aout_sym->addr);
    }

    if (CHECK_FLAG (mb_mmap->flags, 5)) {
        multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mb_mmap->u.elf_sec);

        printk(KERN_INFO, "multiboot_elf_sec: num = %u, size = 0x%x,"
                " addr = 0x%x, shndx = 0x%x",
                (unsigned) multiboot_elf_sec->num, (unsigned) multiboot_elf_sec->size,
                (unsigned) multiboot_elf_sec->addr, (unsigned) multiboot_elf_sec->shndx);


    }

    printk(KERN_INFO, "Memory is %d Gb and %d Mb", map_entry->len_low / GB, (map_entry->len_low % GB) / MB);

	init_memory_infos();
	update_memory_infos(map_entry->len_low, 0);

	printk(KERN_INFO, "Kernel is %d bytes, aka %d pages, aka %d page table", get_kernel_size(), get_kernel_size() / 4096, (get_kernel_size() / 4096) / 1024);
	printk(KERN_INFO, "Kernel start at %p, and end at %p", start_kernel_addr, end_kernel_addr);

	//printd(KERN_INFO, "nframes is %d pages, aka %d page table", nframes, nframes / 1024);
	printd(KERN_INFO, "nframes is %d pages, aka %d page table", nframes, nframes / 1024);

	void *start_pd = (void *)roundUp((char *)end_kernel_addr + 1, 4096);

	void *end_pd = (char *)start_pd + (nframes * sizeof(uint32_t));

	printk(KERN_INFO, "Page tables should begin at %p and end at %p", start_pd, end_pd);

	init_pd_and_map_kernel(start_pd, nframes);

	/* Init physical memory manager */
	void *start_real_memory = create_pmm_array(end_pd + 1, nframes);

	(void)start_real_memory;

	update_memory_infos(map_entry->len_low, (size_t)start_real_memory - (size_t)map_entry->addr_low);

	printk(KERN_INFO, "Memory usage is %d MB %d KB", (get_memory_infos())->used_size / MB, (get_memory_infos()->used_size % MB) / 1024);
	// USED FOR TEST
	/*map_page((void*)0x400000);
	// Do not cause page fault
	uint32_t *ptr1 = (uint32_t*)0x400000;
	uint32_t not_page_fault = *ptr1;
	(void)not_page_fault;*/

	//unmap_page((void*)0x400000);
}
