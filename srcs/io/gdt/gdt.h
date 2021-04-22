/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gdt.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 14:24:56 by lubenard          #+#    #+#             */
/*   Updated: 2021/04/22 17:29:07 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

struct GDTDescriptor {
	uint16_t size;
	uint32_t offset;
} __attribute__((packed));

struct GDTEntry {
	uint16_t limite;
	uint16_t base0;
	uint8_t base1;
	uint8_t assessByte;
	uint8_t limit1Flags;
	uint8_t base2;
} __attribute__((packed));

struct GDT {
	struct GDTEntry null; //0x00
	struct GDTEntry kernelCode; //0x08
	struct GDTEntry kernelData; // 0x10
	struct GDTEntry userNull;
	struct GDTEntry userCode;
	struct GDTEntry userData;
} __attribute__((packed))
  __attribute__((aligned(0x1000)));

extern struct GDT defaultGDT;

extern void LoadGDT(struct GDTDescriptor *gdtDescriptor);
