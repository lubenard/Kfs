/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strlen.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 15:46:07 by lubenard          #+#    #+#             */
/*   Updated: 2021/02/04 12:25:14 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/test_header.h"

int		test_strlen_null(void) {
	if (strlen(NULL) != 0)
		return (1);
	return (0);
}

int		test_strlen_one_char(void) {
	if (strlen("1") != 1)
		return (1);
	return (0);
}

int		test_strlen_cut_half(void) {
	if (strlen("abc\0test ?") != 3)
		return (1);
	return (0);
}

int		test_strlen_only_0(void) {
	if (strlen("\0\0\0") != 0)
		return (1);
	return (0);
}

void	launch_tests_strlen(void) {
	int (*array_tests_addres[10])(void) = {test_strlen_null, test_strlen_one_char,
		test_strlen_cut_half, test_strlen_only_0};
	int counter;

	counter = 0;
	printf("Strlen: ");
	while (array_tests_addres[counter])
	{
		evaluate_tests(array_tests_addres[counter++]());
	}
	printf("\n");
}
