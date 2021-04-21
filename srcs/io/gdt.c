/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gdt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 17:35:12 by lubenard          #+#    #+#             */
/*   Updated: 2021/04/21 17:59:46 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gdt.h"

__attribute__((aligned(0x1000)))
struct GDT DefaultGDT = {
	.null = {0, 0, 0, 0x00, 0x00, 0}, // null
	.kernelCode = {0, 0, 0, 0x9a, 0xa0, 0}, // kernel code
	.kernelData = {0, 0, 0, 0x92, 0xa0, 0}, // kernel code
	.userNull = {0, 0, 0, 0x00, 0x00, 0},
	.userCode = {0, 0, 0, 0x9a, 0xa0, 0},
	.userData = {0, 0, 0, 0x92, 0xa0, 0},
};
