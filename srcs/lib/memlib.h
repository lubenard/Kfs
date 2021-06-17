/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memlib.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 00:16:46 by lubenard          #+#    #+#             */
/*   Updated: 2021/06/17 17:44:21 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMLIB_H
# define MEMLIB_H

# include <stddef.h>
# include <stdint.h>

void		*memset(void *s, int c, size_t n);
void		*memcpy(void *s1, void const *s2, size_t n);
uint32_t	e_kmalloc(uint32_t size, int align, uint32_t *phys);

#endif
