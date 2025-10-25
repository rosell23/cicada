/*
 * Copyright (c) 2025 The Cicada Project. All rights reserved.
 *
 * THE WORK IS PROVIDED "AS IS", WITHOUT ANY WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES, OR OTHER LIABILITY ARISING FROM, OUT OF, OR IN CONNECTION
 * WITH THE WORK OR ITS USE.
*/

#include "vga.h" /* for vga defs */
#include <stdint.h> /* for fixed-size types */
#include "../error/panic.h" /* for gmed */
#include "../cpu/io.h" /* for outb/inb */

/** Set the colors for text mode.
 *
 * Equivalent to just modifying vga_fg/vga_bg */
void
vga_setcol ( enum vga_tm_colors fg, enum vga_tm_colors bg )
	{ vga_fg = fg; vga_bg = bg; }

/** Clear the screen (text mode) */
void
vga_cls ( void )
{
	asm volatile (
		"xor %%di, %%di\n\t" /* null out %di */
		"mov $0x0720, %%ax\n\t" /* space */
		"mov %1, %%cx" /* get count */
		"rep stosw\t\n" /* boom */
		: /* no outputs */
		: "r" (vga_curr_mode->w * vga_curr_mode->h ) /* chars per screen */
		: "ax", "di", "cx", "memory" /* clobbered registers */);
}

/** Get the 16-bit VGA character attribute for current colors.
 *
 * Format: [bg(4)][blinking(1)][unused(3)][fg(4)]
 */

inline uint16_t vga_chattr ( void )
{
	return ( ( (uint16_t)vga_bg & 0x0F ) << 12 ) |
	       ( ( (uint16_t)vga_blinking & 0x01 ) << 11 ) |
	       ( ( (uint16_t)vga_fg & 0x0F) << 8);
}

/** Put a character at the current cursor position */
void
vga_char ( char c, uint16_t x, uint16_t y )
{
	uint16_t *vmem = (uint16_t *)VGA_MEMADDR;
	vmem[(y * vga_curr_mode->w) + x] = ( vga_chattr ( ) | (uint16_t)c );
}

/** Initialize VGA subsysstem (identified as SYBSYS:VIDEO) */
void
vga_init ( enum vga_modes mode )
{
	switch (vga_curr_mode->id) {
	case (VM_TEXT_0):
		setmode_text0 ();
		break;
	case (VM_TEXT_7):
		setmode_text7 ();
		break;
	case (VM_GRAPHIC_13):
		setmode_graphic13 ();
		break;
	default:
		/* Unsupported mode; guru meditaion */
		gmed_panic ("SUBSYS:VIDEO reported an unexpected VGA mode.");
		break;
	}
}

