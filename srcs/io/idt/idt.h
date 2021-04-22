/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   idt.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 18:15:57 by lubenard          #+#    #+#             */
/*   Updated: 2021/04/21 18:28:42 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

struct idt32 {
	uint16_t offset_low;
	uint16_t selector;
	uint8_t zero;
	uint8_t types_attr;
	uint16_t offset_mid;
};

extern struct idt32[256];
