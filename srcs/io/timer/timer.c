// timer.c -- Initialises the PIT, and handles clock updates.
// Written for JamesM's kernel development tutorials.

#include "timer.h"
#include "../isr/isr.h"
#include "../../lib/lib.h"
#include "../../drivers/vga/vga.h"
#include "../../io/io.h"

int timer_ticks = 0;

/* Handles the timer. In this case, it's very simple: We
*  increment the 'timer_ticks' variable every time the
*  timer fires. By default, the timer fires 18.222 times
*  per second. Why 18.222Hz? Some engineer at IBM must've
*  been smoking something funky */
void timer_handler(struct registers *r)
{
	(void)r;
	/* Increment our 'tick count' */
	//timer_ticks++;
	terminal_writestr("Tick++\n");
}

/* Sets up the system clock by installing the timer handler
*  into IRQ0 */
void init_timer(int32_t frequency)
{
    // Firstly, register our timer callback.
    register_interrupt_handler(IRQ0, timer_handler);

    // The value we send to the PIT is the value to divide it's input clock
    // (1193180 Hz) by, to get our required frequency. Important to note is
    // that the divisor must be small enough to fit into 16-bits.
    int32_t divisor = 1193180 / frequency;

    // Send the command byte.
    outb(0x43, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    int8_t l = (int8_t)(divisor & 0xFF);
    int8_t h = (int8_t)( (divisor>>8) & 0xFF );

    // Send the frequency divisor.
    outb(0x40, l);
    outb(0x40, h);
}
