/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strarraylib.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 13:23:16 by lubenard          #+#    #+#             */
/*   Updated: 2021/02/08 16:12:23 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "charlib.h"

/*
** Check if the character c is:
** - numerical (between 0 and 9)
** - lowercase alpha (between 'a' and 'z')
** - uppercase alpha (between 'A' and 'Z')
** @param int c: the character we want to check
** @return return 1 if the character is matching, 0 if not
*/

int	    isalnum(int c)
{
    if ((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122))
        return (1);
    return (0);
}

/*
** Check if the character c is ascii (contained into the ascii table,
**  between 0 and 127)
** @param int c: the character we want to check
** @return return 1 if the character is matching, 0 if not
*/

int		isascii(int c)
{
    if (c >= 0 && c <= 127)
        return (1);
    return (0);
}

/*
** Check if the character c is a number (between 0 and 9)
** @param int c: the character we want to check
** @return return 1 if the character is matching, 0 if not
*/

int		isdigit(int c)
{
    if (c >= 48 && c <= 57)
        return (1);
    return (0);
}

/*
** Convert a character to lowercase, only uppercase letters
** Example: 'c' -> 'C'
** @param int c: The char we want to convert
** @return the character converted into lowercase
*/

int		ft_tolower(int c)
{
    if (c >= 65 && c <= 90)
        c += 32;
    return (c);
}

/*
** Convert a charcter to upperrcase, only lowercase letters
** Example: 'u' -> 'U'
** @param int c: The character we want to convert
** @return the charcter converted into uppercase
*/

int		ft_toupper(int c)
{
    if (c >= 97 && c <= 122)
        return (c - 32);
    return (c);
}

/*
** Check if the character c is:
** - lowercase alpha (between 'a' and 'z')
** - uppercase alpha (between 'A' and 'Z')
** @param int c: the character we want to check
** @return return 1 if the character is matching, 0 if not
*/
int		isalpha(int c)
{
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
        return (1);
    return (0);
}