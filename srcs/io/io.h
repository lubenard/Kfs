/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 23:45:42 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/12 21:58:08 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>

#define KBD_BUF_SIZE 20

typedef struct kbd_entry {
	char	buf[KBD_BUF_SIZE];
	char	buf_raw[KBD_BUF_SIZE];
	unsigned short index;
}				kbd_entry_t;

uint8_t	inb(uint16_t port);
void	outb(uint16_t port, uint8_t byte);
