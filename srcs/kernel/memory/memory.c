/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 15:47:20 by lubenard          #+#    #+#             */
/*   Updated: 2022/10/13 15:00:50 by lubenard         ###   ########.fr       */
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


typedef uint32_t Elf32_Word;
typedef uint32_t Elf32_Addr;
typedef uint32_t Elf32_Off;

typedef struct {
        Elf32_Word      sh_name;
        Elf32_Word      sh_type;
        Elf32_Word      sh_flags;
        Elf32_Addr      sh_addr;
        Elf32_Off       sh_offset;
        Elf32_Word      sh_size;
        Elf32_Word      sh_link;
        Elf32_Word      sh_info;
        Elf32_Word      sh_addralign;
        Elf32_Word      sh_entsize;
} Elf32_Shdr;

static inline Elf32_Shdr *elf_sheader(multiboot_elf_section_header_table_t *hdr) {
    return (Elf32_Shdr *)((int)hdr->addr + hdr->shndx);
}

/*static inline Elf32_Shdr *elf_section(multiboot_elf_section_header_table_t *hdr, int idx) {
    return &elf_sheader(hdr)[idx];
}*/

/*static inline char *elf_str_table(multiboot_elf_section_header_table_t *hdr) {
	return (char *)hdr + elf_section(hdr, hdr->shndx)->sh_offset;
}

static inline char *elf_lookup_string(multiboot_elf_section_header_table_t *hdr, int offset) {
	char *strtab = elf_str_table(hdr);
	if(strtab == NULL) return NULL;
	return strtab + offset;
}*/

void init_memory(multiboot_info_t *mb_mmap) {
	uint32_t nframes;

	multiboot_memory_map_t *map_entry = get_memory_map_from_grub(mb_mmap);

	// Getting the size of the memory via grub.
	// nframes is the number of frames (aka blocks of 4096)
	nframes = (map_entry->len_low / 0x1000); // Ox1000 is 4096 in hexa (Size of one page)

    if (CHECK_FLAG (mb_mmap->flags, 4) && CHECK_FLAG (mb_mmap->flags, 5)) {
        printk(KERN_WARNING, "Both bits 4 and 5 are set.");
    }

	printk(KERN_INFO, "mb_mmap = 0x%x", mb_mmap);

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

		printk(KERN_INFO, "multiboot_elf_sec: num = %u, size = %d bytes,"
				" addr = 0x%x (%d), shndx = %d",
				(unsigned) multiboot_elf_sec->num, (unsigned) multiboot_elf_sec->size,
				(unsigned) multiboot_elf_sec->addr, (unsigned) multiboot_elf_sec->addr,
				(unsigned) multiboot_elf_sec->shndx);

		Elf32_Shdr *list = elf_sheader(multiboot_elf_sec);
		printk(KERN_INFO, "Section should be at: 0x%x", elf_sheader(multiboot_elf_sec));
		int i = 0;
		while (i < 10) {
			printk(KERN_INFO, "Section[%d] at addr %p : %s, size = %d", i, list, list->sh_name, list->sh_size);
			list = (Elf32_Shdr *) ((int) list + multiboot_elf_sec->size);
			i++;
		}
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
