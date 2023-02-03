/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iolib.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 23:32:03 by lubenard          #+#    #+#             */
/*   Updated: 2022/03/11 11:49:13 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../io/io.h"
#include "strlib.h"

void get_line() {
	
}

int init_com_port(uint16_t port) {
   // Disable interrupts
   outb(port + 1, 0x00);
   // enable DLAB
   outb(port + 3, 0x80);
   // Set divisor to 3, 38400 buad
   outb(port + 0, 0x03);
   // hi byte
   outb(port + 1, 0x00);
   // 8 bits, no parity, one stop bit
   outb(port + 3, 0x03);
   // enable FIFO, clear them, with 14-byte threshold
   outb(port + 2, 0xC7);
   // IRQs enabled RTS/DSR set
   outb(port + 4, 0x0B);
   // Set in loopback mode, test serial chhip
   outb(port + 4, 0x1E);
   // Test serial chip
   outb(port + 0, 0xAE);

   // Check if serial is faulty
   if (inb(port + 0) != 0xAE)
      return 0;

   // if serial is not faulty set it in normal operation mode
   outb(port + 4, 0x0F);
   return 1;
}

int is_transmit_empty(uint16_t port) {
   return inb(port + 5) & 0x20;
}

void serial_putchar(uint16_t port, char a) {
   while (!is_transmit_empty(port));

   outb(port, a);
}

void serial_write(uint16_t port, const char *string, size_t n) {
   for (size_t i = 0; i < n; ++i) {
      serial_putchar(port, string[i]);
   }
}
void serial_writestring(uint16_t port, const char *string) {
   serial_write(port, string, strlen(string));
}
