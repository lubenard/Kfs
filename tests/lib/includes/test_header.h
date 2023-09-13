/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_header.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 14:46:20 by lubenard          #+#    #+#             */
/*   Updated: 2021/02/12 12:12:51 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_HEADER_H
# define TEST_HEADER_H

# include <stdio.h>
# include "../../../srcs/lib/strlib.h"
# include "../../../srcs/lib/iolib.h"
# include "../../../srcs/lib/strarraylib.h"

# define COLOR_RESET  "\x1B[0m"
# define COLOR_RED  "\x1B[31m"
# define COLOR_GREEN  "\x1B[32m"
# define COLOR_YELLOW  "\x1B[33m"
void evaluate_tests(int code);

void launch_tests_bzero(void);
void launch_tests_strlen(void);
void launch_tests_tabchr(void);
void launch_tests_printf(void);
#endif
