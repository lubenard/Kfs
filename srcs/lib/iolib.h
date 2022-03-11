/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iolib.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 00:11:13 by lubenard          #+#    #+#             */
/*   Updated: 2022/03/11 11:51:55 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IOLIB_H
# define IOLIB_H

#include <stdint.h>

# define KERN_INFO 0
# define KERN_ERROR 1
# define KERN_NORMAL 2
# define KERN_WARNING 3

void	printk(int info_type, const char *str, ...);
void	printd(int info_type, const char *str, ...);

int init_com_port(uint16_t port);
void serial_writestring(uint16_t port, const char *string);

#endif
