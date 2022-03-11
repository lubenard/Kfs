/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 13:54:18 by lubenard          #+#    #+#             */
/*   Updated: 2022/03/09 14:44:35 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../io/io.h"
#include "../../lib/iolib.h"
#include "../../kernel/isr/isr.h"

#define cmos_address 0x70
#define cmos_data 0x71

unsigned char get_RTC_register(int reg) {
	outb(cmos_address, reg);
	return inb(cmos_data);
}

void	get_datetime_from_rtc() {
	unsigned char second;
	unsigned char minute;
	unsigned char hour;
	unsigned char day;
	unsigned char month;
	unsigned int year;
	unsigned char registerB;

	second = get_RTC_register(0x00);
	minute = get_RTC_register(0x02);
	hour = get_RTC_register(0x04);
	day = get_RTC_register(0x07);
	month = get_RTC_register(0x08);
	year = get_RTC_register(0x09);

	registerB = get_RTC_register(0x0B);

	if (!(registerB & 0x04)) {
		second = (second & 0x0F) + ((second / 16) * 10);
		minute = (minute & 0x0F) + ((minute / 16) * 10);
		hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
		day = (day & 0x0F) + ((day / 16) * 10);
		month = (month & 0x0F) + ((month / 16) * 10);
		year = (year & 0x0F) + ((year / 16) * 10);
	}

	// Convert 12 hour clock to 24 hour clock if necessary
	if (!(registerB & 0x02) && (hour & 0x80)) {
		hour = ((hour & 0x7F) + 12) % 24;
	}
	printk(KERN_INFO, "Date is currently: %d/%d/%d @ %d:%d:%d", day, month, year, hour, minute, second);
}

/*void init_rtc() {
	register_interrupt_handler(8, &get_datetime_from_rtc);
	nmi_disable();
	// Disable interrupts
	asm volatile ("cli");
	outb(0x70, 0x8A);	// select Status Register A, and disable NMI (by setting the 0x80 bit)
	outb(0x71, 0x20);
	outb(0x70, 0x8B);			// select register A, and disable NMI
	char prev = inb(0x71);		// read the current value of register B
	outb(0x70, 0x8B);			// set the index again (a read will reset the index to register D)
	outb(0x71, prev | 0x40);	// write the previous value ORed with 0x40. This turns on bit 6 of register B
	// Enable interrupts
	asm volatile ("sti");
	// Enable nmi (Non masquable interrupts)
	nmi_enable();
	printk(KERN_INFO, "RTC initialised");
}*/
