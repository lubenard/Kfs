/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   date.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luca </var/spool/mail/luca>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 20:09:45 by luca              #+#    #+#             */
/*   Updated: 2022/08/19 20:24:31 by luca             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../lib/iolib.h"
#include "builtins.h"
#include "../../../kernel/isr/irqs/irqs.h"

void date(t_command *command) {
	t_cmos_date *cmos_date;

	(void)command;
	cmos_date = get_bios_date();
	printk(KERN_NORMAL, "Date is: %d/%d/%d @ %d:%d:%d\n", cmos_date->day, cmos_date->month, cmos_date->year, cmos_date->hour, cmos_date->minute, cmos_date->second);
}
