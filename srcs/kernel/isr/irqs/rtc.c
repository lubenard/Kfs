/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 13:54:18 by lubenard          #+#    #+#             */
/*   Updated: 2022/08/19 20:18:26 by luca             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../io/io.h"
#include "../../../lib/iolib.h"
#include "../isr.h"
#include "irqs.h"

#define CMOS_ADDRESS 0x70
#define CMOS_DATA 0x71

t_cmos_date cmos_date = {
	.second = 0,
	.minute = 0,
	.hour = 0,
	.day = 0,
	.month = 0,
	.year = 0
};

t_cmos_date *get_bios_date() {
	return &cmos_date;
}

unsigned char get_RTC_register(int reg) {
	outb(CMOS_ADDRESS, reg);
	return inb(CMOS_DATA);
}

void	get_datetime_from_rtc() {
	unsigned char registerB;
	unsigned char registerC;

	cmos_date.second = get_RTC_register(0x00);
	cmos_date.minute = get_RTC_register(0x02);
	cmos_date.hour = get_RTC_register(0x04);
	cmos_date.day = get_RTC_register(0x07);
	cmos_date.month = get_RTC_register(0x08);
	cmos_date.year = get_RTC_register(0x09);

	registerB = get_RTC_register(0x0B);

	// Get register to thrash it after (done by inb in get_RTC_register)
	// If we dont read register C, irq 8 is triggered only once
	registerC = get_RTC_register(0x0C);

	(void)registerC;

	/*if (registerC & 0x10) {
		printd(KERN_INFO, "Rtc: interrupt happened after update-ended interrupt");
	} else if (registerC & 0x20) {
		printd(KERN_INFO, "Rtc: interrupt happened after alarm interrupt flag");
	} else if (registerC & 0x40) {
		printd(KERN_INFO, "Rtc: interrupt happened after alarm interrupt flag");
	}*/

	if (!(registerB & 0x04)) {
		cmos_date.second = (cmos_date.second & 0x0F) + ((cmos_date.second / 16) * 10);
		cmos_date.minute = (cmos_date.minute & 0x0F) + ((cmos_date.minute / 16) * 10);
		cmos_date.hour = ( (cmos_date.hour & 0x0F) + (((cmos_date.hour & 0x70) / 16) * 10) ) | (cmos_date.hour & 0x80);
		cmos_date.day = (cmos_date.day & 0x0F) + ((cmos_date.day / 16) * 10);
		cmos_date.month = (cmos_date.month & 0x0F) + ((cmos_date.month / 16) * 10);
		cmos_date.year = (cmos_date.year & 0x0F) + ((cmos_date.year / 16) * 10);
	}

	// Convert 12 hour clock to 24 hour clock if necessary
	if (!(registerB & 0x02) && (cmos_date.hour & 0x80)) {
		cmos_date.hour = ((cmos_date.hour & 0x7F) + 12) % 24;
	}
	//printk(KERN_INFO, "Date from bios is currently: %d/%d/%d @ %d:%d:%d", cmos_date.day, cmos_date.month, cmos_date.year, cmos_date.hour, cmos_date.minute, cmos_date.second);
}

void nmi_enable() {
	outb(0x70, inb(0x70) & 0x7F);
	inb(0x71);
}

void nmi_disable() {
	outb(0x70, inb(0x70) | 0x80);
	inb(0x71);
}

void init_rtc() {
	register_interrupt_handler(8, &get_datetime_from_rtc);
	//asm volatile ("int $0x28");
	nmi_disable();

	asm volatile ("cli");
	outb(0x70, 0x8B); // Select register B
	char prev = inb(0x71);	// read the current value of register B
	outb(0x70, 0x8B);		// set the index again (a read will reset the index to register D)
	printk(KERN_INFO, "RTC: %x, but passed %x", prev, prev | 0x10);
	// Write the previous value with 0x10.
	// This set the bit 4 (UIE) in register B of RTC register
	outb(0x71, prev | 0x10);
	asm volatile ("sti");
	// Enable nmi (Non masquable interrupts)
	nmi_enable();
	printk(KERN_INFO, "RTC initialised");
}
