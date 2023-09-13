/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 14:12:54 by lubenard          #+#    #+#             */
/*   Updated: 2021/02/12 12:12:34 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/test_header.h"
#include <stdio.h>

int	main(void)
{
	launch_tests_bzero();
	launch_tests_strlen();
	launch_tests_tabchr();
	launch_tests_printf();
	return (0);
}

void	evaluate_tests(int code)
{
	if (code == 2)
		printf("%so%s", COLOR_YELLOW, COLOR_RESET);
	else if (code == 1)
		printf("%sx%s", COLOR_RED, COLOR_RESET);
	else if (code == 0)
		printf("%sv%s", COLOR_GREEN, COLOR_RESET);
}
