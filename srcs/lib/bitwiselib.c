/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bitwiselib.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 18:13:56 by lubenard          #+#    #+#             */
/*   Updated: 2021/07/02 18:22:26 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int checkBit(unsigned int data, unsigned int pos) {
	return ((data & (1 << pos)) ? 1 : 0);
}

// Function to extract len bits from start position
// and returns the extracted value as unsigned integer
unsigned int extractBit(unsigned int data, unsigned int start, unsigned int len) {
	return (((1 << len) - 1) & (data >> (start - 1)));
}
