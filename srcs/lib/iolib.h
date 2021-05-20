/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iolib.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 00:11:13 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/17 00:16:37 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IOLIB_H
# define IOLIB_H

# define KERN_INFO 0
# define KERN_ERROR 1
# define KERN_NORMAL 2

void	printk(int info_type, const char *str, ...);

#endif
