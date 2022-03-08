/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minwidth.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 17:50:10 by lubenard          #+#    #+#             */
/*   Updated: 2022/03/08 18:28:35 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/printk.h"
#include "../../lib/intlib.h"

int		get_minwidth_size(const char *str, int *i) {
	int output = 0;
	int j;
	if ((j = strchri(&(str[*i]), '.')) >= 0) {
		(*i)++;
		j += *i;
		while (str[j] >= '0' && str[j] <= '9') {
			output = output * 10 + str[j] - 48;
			j++;
			(*i)++;
		}
	}
	return output;
}

void	apply_minwidth_uint(t_output *output, int minwidth, unsigned int *arg) {
	int j = 0;
	int k = 0;

	minwidth -= uintlen(*arg);
	if (minwidth > 0) {
		while (j != minwidth) {
			fill_buffer(output, '0' , &k);
			j++;
		}
	}
}

void	apply_minwidth_hex(t_output *output, int minwidth, char ret[17]) {
	int j = 0;
	int k = 0;

	minwidth -= strlen(ret);
	while (j != minwidth) {
		fill_buffer(output, '0' , &k);
		j++;
	}
}

void	apply_minwidth_int(t_output *output, int minwidth, int *arg) {
	int j = 0;
	int k = 0;

	minwidth -= intlen(*arg);
	if (*arg < 0) {
		fill_buffer(output, '-', &k);
		(*arg) *= -1;
		minwidth++;
	}
	while (j != minwidth) {
		fill_buffer(output, '0' , &k);
		j++;
	}
}
