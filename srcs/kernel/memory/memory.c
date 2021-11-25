/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 15:47:20 by lubenard          #+#    #+#             */
/*   Updated: 2021/11/24 20:34:47 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory.h"
#include "../../lib/iolib.h"
#include "../../lib/memlib.h"
#include "../../lib/bitwiselib.h"
#include "grub/grub.h"
#include "pmm/pmm.h"
#include "vmm/vmm.h"

extern uint32_t endKernel;
extern uint32_t startKernel;

uint32_t static placement_address = (uint32_t) &endKernel;
uint32_t static start_kernel_addr = (uint32_t) &startKernel;

uint32_t page_directory[1024] __attribute__((aligned(4096)));

unsigned int k;

void map_page(void *addr) {
	// Check if pointer is aligned on 4096
	if (((uintptr_t)addr % 4096) != 0) {
		printk(KERN_INFO, "Pointer is not aligned !");
		return;
	}
	unsigned long pdindex = (unsigned long)addr >> 22;
	unsigned long ptindex = (unsigned long)addr >> 12 & 0x03FF;
	printk(KERN_INFO, "PD : %d, PT : %d", pdindex, ptindex);
	if (page_directory[pdindex] & (1 << 0))
		printk(KERN_INFO, "pdindex present");
	else {
		printk(KERN_INFO, "pdindex NOT present");
		/*uint32_t page_table[1024] __attribute__((aligned(4096)));
		(void)page_table;
		for (int i = 0; i < 1024; i++) {
			page_table[i] = (k * 0x1000) | 3;
			k++;
		}
		printk(KERN_INFO, "Added pdindex %d to page_directory", pdindex);
		*///page_directory[pdindex] = ((unsigned int)page_table) | 3;
		//flush_tlb();
		//enable_paging(page_directory);
	}
	//unsigned long *pt = (unsigned long *)(0x400 * pdindex);
	//printk(KERN_INFO, "PT %p", pt);
}

/*void map_page(void *physaddr, void *virtualaddr, unsigned int flags) {
	// Make sure that both addresses are page-aligned.
	unsigned long pdindex = (unsigned long)virtualaddr >> 22;
	unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;
	unsigned long *pd = (unsigned long *)0xFFFFF000;
	// Here you need to check whether the PD entry is present.
	// When it is not present, you need to create a new empty PT and
	// adjust the PDE accordingly.
	unsigned long *pt = ((unsigned long *)0xFFC00000) + (0x400 * pdindex);
	// Here you need to check whether the PT entry is present.
	// When it is, then there is already a mapping present. What do you do now?
	pt[ptindex] = ((unsigned long)physaddr) | (flags & 0xFFF) | 0x01; // Present
	// Now you need to flush the entry in the TLB
	// or you might not notice the change.
}*/

void set_placement_addr(uint32_t new_placement_address) {
	placement_address += new_placement_address;
}

size_t roundUp3(void *a, size_t b) {
	return (1 + ((size_t)a - 1) / b) * b;
}

uint32_t get_kernel_size() {
	return placement_address - start_kernel_addr;
}

void init_memory(multiboot_info_t *mb_mmap) {
	uint32_t nframes;
	unsigned int i;
	uint32_t page_table[1024] __attribute__((aligned(4096)));
	//uint32_t sec_page_table[1024] __attribute__((aligned(4096)));

	multiboot_memory_map_t *map_entry = get_memory_map_from_grub(mb_mmap);

	// Getting the size of the memory via grub.
	nframes = ((map_entry->len_low - get_kernel_size()) / 0x1000); // Ox1000 is 4096 in hexa (Size of one page)

	printk(KERN_NORMAL, "Should require no more than %d pages without kernel, origninaly %d\n", nframes, (map_entry->len_low / 0x1000));
	printk(KERN_NORMAL, "%d pages = %d tables\n", (map_entry->len_low / 0x1000), (map_entry->len_low / 0x1000) / 1024);

	printk(KERN_INFO, "Memory should begin at %p, placement_address at %p", roundUp3((char *)placement_address + 1, 4096), placement_address);

	printk(KERN_INFO, "Page directory is at %p", &page_directory);

	for(i = 0; i < 1024; i++) {
		// This sets the following flags to the pages:
		//   Supervisor: Only kernel-mode can access them
		//   Write Enabled: It can be both read from and written to
		//   Not Present: The page table is not present
		//   2 is in binary 10
		page_directory[i] = 0x00000002;
	}

	printk(KERN_INFO, "%d page directory needed + %d pages table entry needed + %d frames", nframes / 1024, nframes, nframes % 4096);
	printk(KERN_INFO, "Kernel is %d bytes, aka %d pages, aka %d page table", get_kernel_size(), get_kernel_size() / 4096, (get_kernel_size() / 4096) / 1024);
	printk(KERN_INFO, "Pmm should take %d pages, aka %d page table", nframes / 4096, (nframes / 4096) / 1024);

	k = 0;

	//int test = ((get_kernel_size() / 4096) / 1024) + ((nframes / 4096) / 1024);

	/* Mapping 4 first mb (including kernel) */
	for (i = 0; i < 1024; i++) {
		page_table[i] = (k * 0x1000) | 3; // attributes: supervisor level, read/write, present.
		//printk(KERN_INFO, "k = %d aka %p", k, k * 0x1000);
		//printk(KERN_INFO, "%d * 0x1000 = %d, aka %p", k, k * 0x1000, k * 0x1000);
		k++;
	}
	//printk(KERN_INFO, "J = %d, k = %d", j, k);
	page_directory[0] = ((unsigned int)page_table) | 3; // attributes: supervisor level, read/write, present

	printk(KERN_INFO, "Mapped until %p", (k - 1) * 0x1000);
	//printk(KERN_INFO, "Address of page_table %p and Address of sec_page_table %p, third %p", page_table, sec_page_table, third_page_table);

	// Mapping for exclusive usage of page_dir
	/*for (i = 0; i < 1024; i++) {
		sec_page_table[i] = (k * 0x1000) | 3; // attributes: supervisor level, read/write, present.
		//printk(KERN_INFO, "%d * 0x1000 = %d, aka %p", k, k * 0x1000, k * 0x1000);
		k++;
	}
	page_directory[1] = ((unsigned int)sec_page_table) | 3; // attributes: supervisor level, read/write, present
	*/

	/*for (i = 0; i < 1024; i++) {
		third_page_table[i] = (k * 0x1000) | 3; // attributes: supervisor level, read/write, present.
		//printk(KERN_INFO, "%d * 0x1000 = %d, aka %p", k, k * 0x1000, k * 0x1000);
		k++;
	}
	page_directory[2] = ((unsigned int)third_page_table) | 3; // attributes: supervisor level, read/write, present
*/

	//printk(KERN_INFO, "Mapped %d pages", nframes / 1024);

	/*for (i = 0; i < nframes % 1024; i++) {
		page_table[i] = (k * 0x1000) | 3;
		k++;
	}*/
	//page_directory[++j] = ((unsigned int)page_table) | 3; // attributes: supervisor level, read/write, present

	//printk(KERN_INFO, "Mapped %d", nframes % 1024);

	enable_paging(page_directory);
	//printk(KERN_INFO, "Paging enabled and working, mapped 1mb + kernel");
	/* Init physical memory manager */
	create_pmm_array((char *)placement_address + 1, nframes);
	map_page((void*)0x400000);
}
