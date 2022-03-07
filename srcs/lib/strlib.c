/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strlib.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 00:24:51 by lubenard          #+#    #+#             */
/*   Updated: 2022/03/07 09:05:45 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "../kernel/memory/vmm/malloc/malloc.h"

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

int		strchri(const char *s, int c)
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
	return (-1);
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

int		strcmp(char const *s1, char const *s2)
{
	int i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	while (s1[i] == s2[i] && s1[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int		strncmp(const char *s1, const char *s2, size_t n)
{
	size_t i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	if (n == 0)
		return (0);
	while (s1[i] == s2[i] && s1[i] && s2[i] && i < n - 1)
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int		atoi(char const *str)
{
	size_t	output;
	int		i;
	int		mult;

	output = 0;
	i = 0;
	mult = 1;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\v' \
			|| str[i] == '\n' || str[i] == '\r' || str[i] == '\f')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		mult = (str[i] == '-') ? -1 : 1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		output = output * 10 + str[i] - 48;
		i++;
	}
	return (output * mult);
}

char	*strcpy(char *dest, char const *src)
{
	int i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

size_t		ft_strlen(char const *s)
{
	size_t i;

	i = 0;
	if (!s)
		return (0);
	while (*s)
	{
		s++;
		i++;
	}
	return (i);
}

char	*strlcpy(char *dest, char *src, int size)
{
	int i;

	i = 0;
	while (src[i] && i != size)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

void	ft_bzero(void *s, size_t n)
{
	int		i;
	char	*t;

	i = 0;
	t = (char *)s;
	while (i != (int)n)
		t[i++] = '\0';
}

char	*ft_strnew(size_t n)
{
	char	*dest;

	if (!(dest = (char *)malloc(sizeof(char) * (n + 1))))
		return (NULL);
	ft_bzero(dest, n + 1);
	return (dest);
}

// StrSplit
static int		countwords(char const *s, char c)
{
	size_t	compteur;
	size_t	i;

	i = 0;
	compteur = 0;
	while (i < ft_strlen(s))
	{
		if (i == 0 && s[i] != c)
			compteur++;
		if (s[i] == c && s[i + 1] != c && s[i + 1] != '\0')
			compteur++;
		i++;
	}
	return (compteur);
}

static int		return_i(char const *str, int i, char c)
{
	while (str[i] == c)
		i++;
	return (i);
}

static int		countchar(char const *str, int i, char c)
{
	int nbrchar;

	nbrchar = 0;
	i = return_i(str, i, c);
	while (str[i])
	{
		if (str[i] && str[i] != c)
			nbrchar++;
		else
			return (nbrchar);
		i++;
	}
	return (nbrchar);
}

char			**ft_strsplit(char const *s, char c)
{
	char	**ret;
	int		j;
	int		i;
	int		k;
	int		nbr_words;

	j = 0;
	i = 0;
	nbr_words = countwords(s, c);
	if (!s || !(ret = (char **)malloc(sizeof(char *) * (nbr_words + 1))))
		return (0);
	while (j < nbr_words)
	{
		i = return_i(s, i, c);
		k = 0;
		if (!(ret[j] = ft_strnew(countchar(s, i, c) + 1)))
			return (0);
		while (s[i] && s[i] != c)
			ret[j][k++] = s[i++];
		j++;
	}
	ret[j] = NULL;
	return (ret);
}
