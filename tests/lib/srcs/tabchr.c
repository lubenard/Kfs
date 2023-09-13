/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tabchr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 19:35:26 by lubenard          #+#    #+#             */
/*   Updated: 2021/02/09 21:12:49 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/test_header.h"

int		nullargs(void)
{
	if (tabchr(NULL, 0) != 0)
		return (1);
	return (0);
}

int		nulltab(void)
{
	if (tabchr(NULL, 'a') != 0)
		return (1);
	return (0);
}

int		basic_test(void)
{
	char *tab[4];

	tab[0] = "Hey";
	tab[1] = "you";
	tab[2] = "handsome";
	tab[3] = NULL;

	if (tabchr(tab, 'h'))
		return (1);
	return (0);
}

int		basic_test_non_existant(void)
{
	char	*tab[4];

	tab[0] = "Hey";
	tab[1] = "you";
	tab[2] = "handsome";
	tab[3] = NULL;

	if (!tabchr(tab, 'v'))
		return (1);
	return (0);
}

void	launch_tests_tabchr(void)
{
	int (*array_tests_addres[256])(void) = {nulltab, nullargs, basic_test,
		basic_test_non_existant};
	int counter;

	counter = 0;
	printf("Tabchr: ");
	while (array_tests_addres[counter])
	{
		evaluate_tests(array_tests_addres[counter++]());
	}
	printf("\n");
}
