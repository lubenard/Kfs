/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vmm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 21:33:32 by lubenard          #+#    #+#             */
/*   Updated: 2021/11/23 21:13:27 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VMM_H
# define VMM_H

void munmap(void *ptr, unsigned int size);
void *mmap(unsigned int size, int flags);

#endif
