/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 22:31:29 by lubenard          #+#    #+#             */
/*   Updated: 2021/04/21 23:42:42 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

/*unsigned char inb (unsigned int port) {
	unsigned char byte;
	__asm__ volatile ("inb %1, %0" : "=a" (byte) : "dN" (port));
	return (byte);
}*/

void		outb(size_t port, unsigned char byte)
{
	__asm__ volatile ("outb %0, %w1;" : : "a" (byte), "Nd" (port));
} 
