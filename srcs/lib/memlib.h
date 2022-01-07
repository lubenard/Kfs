/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memlib.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 00:16:46 by lubenard          #+#    #+#             */
/*   Updated: 2022/01/07 17:03:33 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMLIB_H
# define MEMLIB_H

# include <stddef.h>
# include <stdint.h>

size_t		roundUp(void *a, size_t b);
size_t		roundUpDiff(void *a, size_t b);
void		*memset(void *s, int c, size_t n);
void		*memcpy(void *s1, void const *s2, size_t n);

#endif
