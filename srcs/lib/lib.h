/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 00:11:13 by lubenard          #+#    #+#             */
/*   Updated: 2021/04/27 18:24:22 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

#define KERN_INFO 0
#define KERN_ERROR 1

size_t	strlen(const char *str);
int		isalpha(int c);
size_t	strchri(const char *s, int c);
void	bzero(void *s, size_t n);
int		isprint(int c);
char	*strupper(char *str);
void	printk(int info_type, const char *str, ...);
