/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strarraylib.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 13:23:16 by lubenard          #+#    #+#             */
/*   Updated: 2021/02/08 16:12:23 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRARRAYLIB_H
# define STRARRAYLIB_H

int     tabchr(char **tab, int pattern);
int     tablen(char **tab);
char	*tabstr(char **tab, char const *to_find);
void	deltab(char ***tab);

#endif