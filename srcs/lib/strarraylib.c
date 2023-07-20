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

/*
** Look for a pattern into a char **
** @param char **tab: the char ** we want to explore
** @param int pattern: the pattern we want to match
** @return 1 if the pattern is found, else 0
*/

int		tabchr(char **tab, int pattern)
{
    int i;

    i = 0;
    if (!tab)
        return (0);
    while (tab[i])
    {
        if (ft_strchri(tab[i++], pattern))
            return (1);
    }
    return (0);
}

/*
** Compute the size of a char **
** @param: the char ** you want to compute
** @return the size of the char **
*/

int		tablen(char **tab)
{
    int		i;

    i = 0;
    while (tab[i])
        i++;
    return (i);
}

/*
** Similar to ft_strstr, but for char **
** Find a substring in a string
** @param char **tab: the array to search into
** @param const string to_find: the string we are looking for
** @return a pointer where the beginning of the located substring is, or 0
**  if not found
*/

char	*tabstr(char **tab, char const *to_find)
{
    int		i;
    char	*ret;

    i = 0;
    while (tab[i])
    {
        if ((ret = ft_strstr(tab[i], to_find)) != 0)
            return (ret);
        i++;
    }
    return (0);
}

/*
** Delete a char ** passed with adress
** Free all elements then free the char **
** @param char ***tab: The wanted array to free
*/
void	deltab(char ***tab)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while ((*tab)[i])
        i++;
    while (j != i)
    {
        free((*tab)[j]);
        (*tab)[j] = NULL;
        j++;
    }
    free(*tab);
    *tab = NULL;
}
