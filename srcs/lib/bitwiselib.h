/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bitwiselib.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 18:16:02 by lubenard          #+#    #+#             */
/*   Updated: 2021/07/02 18:27:18 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITWISELIB_H
# define BITWISELIB_H

int setBit(unsigned int data, unsigned int pos);
int clearBit(unsigned int data, unsigned int pos);
int checkBit(unsigned int data, unsigned int pos);
int extractBit(int number, int len, int start);

#endif
