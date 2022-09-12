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
