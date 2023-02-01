/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 23:45:42 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/17 00:13:27 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IO_H
# define IO_H

# include <stdint.h>
# include "../lib/stddef.h"

uint8_t	inb(uint16_t port);
void	outb(uint16_t port, uint8_t byte);
void	outw(uint16_t port, uint16_t byte);

#endif
