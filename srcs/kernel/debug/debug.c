/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 15:47:20 by lubenard          #+#    #+#             */
/*   Updated: 2023/02/05 15:00:50 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include "../../lib/iolib.h"
#include "../../lib/strlib.h"
#include "../grub/grub.h"
#include "debug.h"

struct multiboot_tag_elf_sections *multiboot_infos = 0;

void init_debugging_infos(multiboot_tag_t *tag) {
    printd(KERN_INFO, "Found Elf Tag Type section at addr %p", tag);
    printd(KERN_INFO, "Elf info type %d | size %d | num %d | entsize %d | shndx %d",
           ((struct multiboot_tag_elf_sections *) tag)->type,
           ((struct multiboot_tag_elf_sections *) tag)->size,
           ((struct multiboot_tag_elf_sections *) tag)->num,
           ((struct multiboot_tag_elf_sections *) tag)->entsize,
           ((struct multiboot_tag_elf_sections *) tag)->shndx);

    struct multiboot_tag_elf_sections *debug = ((struct multiboot_tag_elf_sections *) tag);

    multiboot_infos = debug;

    Elf32_Shdr_t *name_sections = (Elf32_Shdr_t *) (debug->sections + (debug->entsize * debug->shndx));
    Elf32_Shdr_t *strtab = 0;
    printd(KERN_INFO, "Name section is supposed to be at %p at index[%d]", name_sections, debug->shndx);

    for (unsigned int i = 0; i < debug->num; i++) {
        Elf32_Shdr_t *section = (Elf32_Shdr_t *) (debug->sections + (debug->entsize * i));
        printd(KERN_INFO, "Section header [%d]: Addr: 0x%x, name: %s", i, section, (char *)(name_sections->sh_addr + section->sh_name));
        if (strcmp(".strtab", (char *)(name_sections->sh_addr + section->sh_name)) == 0) {
            strtab = section;
        }
    }

    if (strtab != 0) {
        printd(KERN_INFO, "Strtab found !!");

        for (unsigned int j = 0; j < multiboot_infos->num; j++) {
            Elf32_Shdr_t *section = (Elf32_Shdr_t *) (multiboot_infos->sections + (multiboot_infos->entsize * j));

            //printd(KERN_INFO, "Section header [%d]: Addr: 0x%x, name: %s sh_size %d", j, section, (char *)(name_sections->sh_addr + section->sh_name), section->sh_size);

            if (section->sh_type != SHT_SYMTAB)
                continue;

            void *ptr = (void *)section->sh_addr;
            //printd(KERN_INFO, "Pointer is %x", ptr);
            //printd(KERN_INFO, "Addr is %x", addr);

            unsigned int h = 0;
            while (h < section->sh_size) {
                Elf32_Sym_t *symbol = (Elf32_Sym_t *)(ptr + h);

                printd(KERN_INFO, "Located symbol names: %s (0x%x + 0x%x) symbol is at 0x%x, addr = 0x%x, size = %d equal = %x",
                       (char *) (strtab->sh_addr + symbol->st_name), strtab->sh_addr, symbol->st_name, (ptr + h), symbol->st_value, symbol->st_size, (void *)(symbol->st_value + symbol->st_size));

                h += section->sh_entsize;
            }
        }
    }
    printd(KERN_INFO, "Debug info parsed !");
}

char *get_symbol_from_address(void *addr) {
    //printd(KERN_INFO, "Trying to get symbol for addr %x....", addr);

    if (multiboot_infos != 0) {
        Elf32_Shdr_t *name_sections = (Elf32_Shdr_t *) (multiboot_infos->sections + (multiboot_infos->entsize * multiboot_infos->shndx));

        Elf32_Shdr_t *strtab = 0;

        for (unsigned int i = 0; i < multiboot_infos->num; i++) {
            Elf32_Shdr_t *section = (Elf32_Shdr_t *) (multiboot_infos->sections + (multiboot_infos->entsize * i));
            //printd(KERN_INFO, "Section header [%d]: Addr: 0x%x, name: %s, sh_addr is %x, hdr.sh_type = %x", i, section, (char *)(name_sections->sh_addr + section->sh_name), section->sh_addr, section->sh_type);
            if (strcmp(".strtab", (char *)(name_sections->sh_addr + section->sh_name)) == 0) {
                strtab = section;
            }
        }

        if (strtab != 0) {
            //printd(KERN_INFO, "Strtab found !!");

            for (unsigned int j = 0; j < multiboot_infos->num; j++) {
                Elf32_Shdr_t *section = (Elf32_Shdr_t *) (multiboot_infos->sections + (multiboot_infos->entsize * j));

                //printd(KERN_INFO, "Section header [%d]: Addr: 0x%x, name: %s sh_size %d", j, section, (char *)(name_sections->sh_addr + section->sh_name), section->sh_size);

                if (section->sh_type != SHT_SYMTAB)
                    continue;

                void *ptr = (void *)section->sh_addr;
                //printd(KERN_INFO, "Pointer is %x", ptr);
                //printd(KERN_INFO, "Addr is %x", addr);

                unsigned int h = 0;
                while (h < section->sh_size) {
                    Elf32_Sym_t *symbol = (Elf32_Sym_t *)(ptr + h);

                    if (addr < (void *)(symbol->st_value + symbol->st_size) && addr > (void *)symbol->st_value) {
                        //printd(KERN_INFO, "[Was looking for %x] Function name could be : %s (0x%x + 0x%x) symbol is at 0x%x, addr = 0x%x, size = %d equal = %x",
                        //      addr, (char *) (strtab->sh_addr + symbol->st_name), strtab->sh_addr, symbol->st_name, (ptr + h), symbol->st_value, symbol->st_size, (void *)(symbol->st_value + symbol->st_size));
                        return (char *) (strtab->sh_addr + symbol->st_name);
                        break;
                    } else {
                        //printd(KERN_INFO, "[Was looking for %x] Symbol should be at: %x, name is %s addr = 0x%x, size = %d, equal = %x",
                        //       addr, (ptr + h), (char *)(strtab->sh_addr + symbol->st_name),symbol->st_value, symbol->st_size, (void *)(symbol->st_value + symbol->st_size));
                        //    printd(KERN_INFO, "Function name might be : %s (0x%x + 0x%x)", (char *)(strtab->sh_addr + symbol->st_name), strtab->sh_addr, symbol->st_name);
                    }

                    h += section->sh_entsize;
                }
            }
        }
        //printd(KERN_INFO, "End of research of function name");
    }
    return 0;
}