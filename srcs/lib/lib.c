/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 00:11:28 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/02 14:48:38 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

size_t strlen(const char *str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

int		isalpha(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (1);
	return (0);
}

size_t	strchri(const char *s, int c)
{
	size_t	n;

	n = 0;
	if (!s || !c)
		return (0);
	while (s[n])
	{
		if (s[n] == c)
			return (n);
		n++;
	}
	return (0);
}

void	bzero(void *s, size_t n)
{
	int		i;
	char	*t;

	i = 0;
	t = (char *)s;
	while (i != (int)n)
		t[i++] = '\0';
}

int		isprint(int c)
{
	if (c >= 32 && c <= 126)
		return (1);
	return (0);
}

char	*strupper(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= 97 && str[i] <= 122)
			str[i] -= 32;
		i++;
	}
	return (str);
}

void	*memset(void *s, int c, size_t n)
{
	char	*t;
	int		i;

	t = (char *)s;
	i = 0;
	while (i != (int)n)
		t[i++] = c;
	return (t);
}
