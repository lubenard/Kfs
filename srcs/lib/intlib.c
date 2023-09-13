/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intlib.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 17:34:55 by lubenard          #+#    #+#             */
/*   Updated: 2022/08/18 23:09:31 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stddef.h"
#include "strlib.h"

/*
** Compute the size of a int
** Example: 123 -> 3
** @param int n: the number we want to Compute
** @return the size of the integer
*/
short intlen(long n) {
	long		nbr;
	int			i;

	nbr = n;
	i = 0;
	if (nbr < 0)
		nbr = nbr * -1;
	while (nbr / 10 > 0) {
		i++;
		nbr = nbr / 10;
	}
	if (n < 0)
		return (i + 2);
	else
		return (i + 1);
}

short uintlen(unsigned int n) {
	int				i;

	i = 0;
	while (n / 10 > 0) {
		i++;
		n = n / 10;
	}
	return (i + 1);
}

/*
** Convert a number into a string
** Example: 123-> "123"
** The string is allocated, we need to FREE it after use
** @param int n: The number we want to Convert
** @return the converted integer into a string
*/
static char		*ft_result(char *result, long int nbr, int n)
{
    if (n < 0)
        result[1] = nbr + '0';
    else
        result[0] = nbr + '0';
    return (result);
}

static int		ft_len(int n)
{
    long int	nbr;
    int			i;

    nbr = n;
    i = 0;
    if (nbr < 0)
        nbr = nbr * -1;
    while (nbr / 10 > 0)
    {
        i++;
        nbr = nbr / 10;
    }
    if (n < 0)
        return (i + 2);
    else
        return (i + 1);
}

char			*ft_itoa(int n)
{
    long int	nbr;
    short		neg;
    int			i;
    char		*result;

    nbr = n;
    i = 0;
    neg = (nbr < 0) ? -1 : 0;
    if (!(result = strnew(ft_len(n) - neg)))
        return (NULL);
    if (nbr < 0)
    {
        nbr = nbr * -1;
        result[i] = '-';
        i++;
    }
    while (n != 0 && nbr > 9 && (ft_len(n) - i) > 0)
    {
        result[ft_len(n) - i - neg - 1] = (nbr % 10) + '0';
        nbr = nbr / 10;
        i++;
    }
    return (ft_result(result, nbr, n));
}

/*
** Compute the size of a int
** Example: 123 -> 3
** @param int n: the number we want to Compute
** @return the size of the integer
*/

inline static int	nbrLen(unsigned long n)
{
    int i;

    i = 0;
    if (n == 0)
        return (1);
    while (n != 0)
    {
        i++;
        n /= 10;
    }
    return (i);
}

char				*nzero(char *str)
{
    str[0] = '0';
    return (str);
}

/** Similar to ft_itoa, but take a unsigned int as paramter
        ** Convert a number into a string
        ** Example: 123-> "123"
** The string is allocated, we need to FREE it after use
** @param unisgned int n: The number we want to Convert
** @return the converted integer into a string
*/
char				*ft_itoa_uint(unsigned int n)
{
    unsigned int	nbr;
    int				i;
    char			*result;

    nbr = n;
    i = 0;
    if (!(result = strnew((size_t)nbrLen(nbr))))
        return (NULL);
    if (n == 0)
        return (nzero(result));
    while (nbr != 0)
    {
        result[ft_len(n) - i - 1] = (nbr % 10) + '0';
        nbr /= 10;
        i++;
    }
    return (result);
}

/*
** Similar to ft_itoa, but take a unsigned long long as paramter
** Convert a number into a string
** Example: 123-> "123"
** The string is allocated, we need to FREE it after use
** @param unisgned long long  n: The number we want to Convert
** @return the converted integer into a string
*/

inline static int	ft_len_ulong(unsigned long n)
{
    int i;

    i = 0;
    if (n == 0)
        return (1);
    while (n != 0)
    {
        ++i;
        n /= 10;
    }
    return (i);
}

char				*ft_itoa_ulong(unsigned long long n)
{
    unsigned long	nbr;
    int				i;
    char			*result;

    nbr = n;
    i = 0;
    if (!(result = strnew((size_t)ft_len_ulong(nbr))))
        return (NULL);
    if (n == 0)
    {
        result[0] = '0';
        return (result);
    }
    while (nbr != 0)
    {
        result[ft_len_ulong(n) - i - 1] = (nbr % 10) + '0';
        nbr /= 10;
        ++i;
    }
    return (result);
}
