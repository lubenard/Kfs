/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vmm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 21:33:32 by lubenard          #+#    #+#             */
/*   Updated: 2021/11/16 21:41:23 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VMM_H
# define VMM_H

typedef struct	frame {
	int				status;
	int				owner;
	struct *frame	prev;
	struct *frame	next;
}					t_frame;

#endif
