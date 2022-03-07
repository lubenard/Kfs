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

# include <stddef.h>
size_t	strlen(const char *str);
int		isalpha(int c);
int		strchri(const char *s, int c);
void	bzero(void *s, size_t n);
int		isprint(int c);
char	*strupper(char *str);
int		strcmp(char const *s1, char const *s2);
int		strncmp(const char *s1, const char *s2, size_t n);
int		atoi(char const *str);
char	*strcpy(char *dest, char const *src);
char	*strlcpy(char *dest, char *src, int size);
char	**ft_strsplit(char const *s, char c);
#endif
