/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strlib.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 00:17:58 by lubenard          #+#    #+#             */
/*   Updated: 2022/03/07 09:17:57 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRLIB_H
# define STRLIB_H

#ifndef LAUNCH_TESTS
# include "stddef.h"
#endif

size_t	strlen(const char *str);
int		isalpha(int c);
int		strchri(const char *s, int c);
void	bzero(void *s, size_t n);
int		isprint(int c);
char	*strupper(char *str);
int		strcmp(char const *s1, char const *s2);
char	*strnew(size_t n);
char	*ft_strstr(char const *str, char const *to_find);
int		strncmp(const char *s1, const char *s2, size_t n);
int		atoi(char const *str);
char	*strcpy(char *dest, char const *src);
char	*strlcpy(char *dest, char *src, int size);
char	**ft_strsplit(char const *s, char c);
#endif
