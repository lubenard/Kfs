/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 22:31:29 by lubenard          #+#    #+#             */
/*   Updated: 2021/04/22 00:49:49 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdint.h>

uint8_t	inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return (ret);
}

void	outb(size_t port, unsigned char byte)
{
	__asm__ volatile ("outb %0, %w1;" : : "a" (byte), "Nd" (port));
} 
