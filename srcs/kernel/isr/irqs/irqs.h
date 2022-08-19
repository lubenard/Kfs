/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irqs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 20:30:26 by lubenard          #+#    #+#             */
/*   Updated: 2022/08/19 20:22:25 by luca             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRQS_H
# define IRQS_H

typedef struct s_cmos_date {
	unsigned char second;
	unsigned char minute;
	unsigned char hour;
	unsigned char day;
	unsigned char month;
	unsigned int year;
}				t_cmos_date;

// PIT functions
void init_pit();
uint32_t get_tick();

// RTC functions
void init_rtc();
t_cmos_date *get_bios_date();

#endif
