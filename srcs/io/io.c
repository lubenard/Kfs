/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 22:31:29 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/16 23:24:27 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

uint8_t	inb(uint16_t port)
{
	uint8_t ret;
	asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return (ret);
}

void	outb(uint16_t port, uint8_t byte)
{
	asm volatile ("outb %1, %0" : : "dN"(port), "a"(byte));
}

void	outw(uint16_t port, uint16_t byte)
{
	asm volatile ("outw %1, %0" : : "dN"(port), "a"(byte));
}
