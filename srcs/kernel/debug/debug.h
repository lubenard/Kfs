/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 15:47:20 by lubenard          #+#    #+#             */
/*   Updated: 2023/02/05 15:00:50 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H

#include "../../lib/iolib.h"

typedef struct Elf32_Shdr_s {
    uint32_t      sh_name;
    uint32_t      sh_type;
    uint32_t      sh_flags;
    uint32_t      sh_addr;
    uint32_t      sh_offset;
    uint32_t      sh_size;
    uint32_t      sh_link;
    uint32_t      sh_info;
    uint32_t      sh_addralign;
    uint32_t      sh_entsize;
} Elf32_Shdr_t;

typedef struct Elf32_Sym_s {
    uint32_t		st_name;
    uint32_t		st_value;
    uint32_t		st_size;
    uint8_t			st_info;
    uint8_t			st_other;
    uint16_t		st_shndx;
} Elf32_Sym_t;

void init_debugging_infos(multiboot_tag_t *tag);
void get_symbol_from_address(void *addr);

#endif //KFS_DEBUG_H
