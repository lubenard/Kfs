/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intlib.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 17:34:55 by lubenard          #+#    #+#             */
/*   Updated: 2022/03/08 18:12:45 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int intlen(int n) {
	long int	nbr;
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

int uintlen(unsigned int n) {
	unsigned int	nbr;
	int				i;

	nbr = n;
	i = 0;
	while (nbr / 10 > 0) {
		i++;
		nbr = nbr / 10;
	}
	return (i + 1);
}
