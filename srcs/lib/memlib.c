/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memlib.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 00:24:32 by lubenard          #+#    #+#             */
/*   Updated: 2022/01/07 17:09:13 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memlib.h"
#include "strlib.h"
#include "../kernel/memory/vmm/malloc/malloc.h"

/**
 * Return the next 'aligned' address on b.
 * Param void *a is the base address.
 * Param size_t b is the alignement wanted.
 * Exemple: a= 0x123456, b= 4096
 * Return:  124000
 * Cast the return as a pointer
 */
size_t roundUp(void *a, size_t b) {
	return (1 + ((size_t)a - 1) / b) * b;
}

/**
 * Return the diff between given address (a), and next aligned address based on b
 * Parameter are the same as roundUp function
 */
size_t roundUpDiff(void *a, size_t b) {
	return (size_t)roundUp(a, b) - (size_t)a;
}

/*
** Set n bytes of s datas with the c character
** @param void *s: the datas we fill
** @param int c: the character we want to fill with
** @param size_t n: The number of bytes we want to set
** @return the datas s with the copied bytes
*/
void	*memset(void *s, int c, size_t n) {
	char	*t;
	int		i;

	t = (char *)s;
	i = 0;
	if (!s)
		return 0;
	while (i != (int)n)
		t[i++] = c;
	return (t);
}

/*
** Copy n bytes of datas from s2 to s1
** If datas exist in s1, new datas will overwrite them
** @param void *s1: The datas to fill
** @param void *s2: The datas to fill with
** @param size_t n: The number of bytes to copy
** @return s1 with the new datas copied from s2
*/
void	*memcpy(void *s1, void const *s2, size_t n) {
	int		i;
	char	*t1;
	char	*t2;

	i = 0;
	t1 = (char *)s1;
	t2 = (char *)s2;
	while (i != (int)n)
	{
		t1[i] = t2[i];
		i++;
	}
	return (t1);
}

/*
** Similar to memcpy
** Copy n bytes of datas from s2 to s1
** If datas exist in s1, new datas will overwrite them
** If during the copy the character c is found in s2, the copy stops
** and a pointer to the byte after the copy of c in s2 is returned
** @param void *s1: The datas to fill
** @param void *s2: The datas to fill with
** @param int c: The character that can stop the copy
** @param size_t n: The number of bytes to copy
** @return NULL, or a pointer of c + 1 in s2 if found in s2
*/

void	*ft_memccpy(void *s1, const void *s2, int c, size_t n)
{
    int i;

    i = 0;
    while (i < (int)n)
    {
        ((unsigned char *)s1)[i] = ((unsigned char *)s2)[i];
        if (((unsigned char *)s1)[i] == (unsigned char)c)
            return ((unsigned char *)s1 + i + 1);
        i++;
    }
    return (NULL);
}

/*
** Compare s2 and s2 on the n first characters
** @param void const *s1: the first datas
** @param void const *s2: the second datas
** @param size_t n: the number of bytes we want to check
** @return 0 if n = 0. return the difference of size between s1 and s2
*/

int		ft_memcmp(void const *s1, void const *s2, size_t n)
{
    int					i;
    unsigned char const	*t1;
    unsigned char const	*t2;

    t1 = s1;
    t2 = s2;
    i = 0;
    if (n == 0)
        return (0);
    while (i != (int)n - 1 && t1[i] == t2[i])
        i++;
    return ((unsigned char)t1[i] - (unsigned char)t2[i]);
}

/*
** Copies n bytes from string s2 to string s1.
** The two strings may overlap;
** @param void *s1: The datas we will copy into
** @param void const s2: The datas we take from to copy
** @param size_t n: The number of byte we want to copy
** @return the s1 string with new datas
*/

void	*ft_memmove(void *s1, void const *s2, size_t n)
{
    if ((unsigned char *)s2 < (unsigned char *)s1)
    {
        n--;
        while (n + 1 > 0)
        {
            ((unsigned char *)s1)[n] = ((unsigned char *)s2)[n];
            n--;
        }
    }
    else
        memcpy(s1, s2, n);
    return (s1);
}

/*
** Allocate the size n of memory using malloc
** Use FREE after use
** @param size_t n: number of byte we want to allocate
** @return the fresh allocated data
*/

void	*ft_memalloc(size_t n)
{
    void *dest;

    if (!(dest = (void *)malloc(n)))
        return (NULL);
    bzero(dest, n);
    return (dest);
}

/*
** Search for the c character in the n bytes of s
** @param void const *s: datas we are looking int
** @param int c: The character we are looking for
** @param size_t n: The N first character to look into
** @return a pointer to the byte if located in s, or NULL if c does not exist
** in the first n character of s
*/

void	*ft_memchr(void const *s, int c, size_t n)
{
    int		i;
    char	*t;

    t = (char *)s;
    i = 0;
    while (i < (int)n)
    {
        if ((char)c == *t)
            return (t);
        t++;
        i++;
    }
    return (NULL);
}

/*
** Take an adress of datas and free it
** @param void **ap: The datas we want to erase
*/

void	ft_memdel(void **ap)
{
    if (ap == NULL)
        return ;
    free(*ap);
    *ap = NULL;
}