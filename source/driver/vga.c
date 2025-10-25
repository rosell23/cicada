/*
 * Copyright (c) 2025 The Cicada Project. All rights reserved.
 *
 * THE WORK IS PROVIDED "AS IS", WITHOUT ANY WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES, OR OTHER LIABILITY ARISING FROM, OUT OF, OR IN CONNECTION
 * WITH THE WORK OR ITS USE.
*/

 /*---------------------------------------------------------------------------\
 *                                                                            *
 * ATTENTION: VGA may be old, but it still obeys laws of physics.             *
 *                                                                            *
 * These functions directly and literally modify how to generate timings,     *
 * electrons, voltages, and sync.                                             *
 *   By changing some tables present on this file, you **will** burn monitors,*
 * you **won't** get nothing but complains (and with a lot of reason).        *
 *   These values are safe & tested, found from the official VGA manuals from *
 * IBM.                                                                       *
 *                                                                            *
 * This is well-known behaviour and has happened with both CRTs and LCDs.     *
 *                                                                            *
 * Do NOT poke with any values labeled with CRTC or similar.                  *
 * You do NOT have any need to change these, nor you will ever have.          *
 *                                                                            *
 * That being said, you do need these values to do some of this               *
 *                                                                            *
 \---------------------------------------------------------------------------*/

#include "vga.h" /* for vga defs */
#include <stdint.h> /* for fixed-size types */
#include <stddef.h> /* for NULL */
#include "../error/panic.h" /* for gmed */
#include "../cpu/io.h" /* for outb/inb */

/* Forward declarations */
void setmode_text0(void);
void setmode_text7(void);
void setmode_graphic13(void);

/* VGA mode definitions */
static struct vga_mode vga_modes[] = {
	[VM_TEXT_0] = {
		.w = 40,
		.h = 25,
		.depth = 4,
		.id = VM_TEXT_0,
		.setm = setmode_text0,
		.setpix = NULL
	},
	[VM_TEXT_3] = {
		.w = 80,
		.h = 25,
		.depth = 4,
		.id = VM_TEXT_3,
		.setm = setmode_text7,
		.setpix = NULL
	},
	[VM_GRAPHIC_13] = {
		.w = 320,
		.h = 200,
		.depth = 8,
		.id = VM_GRAPHIC_13,
		.setm = setmode_graphic13,
		.setpix = NULL
	}
};

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
	uint16_t count = vga_curr_mode->w * vga_curr_mode->h;
	asm volatile (
		"xor %%di, %%di\n\t" /* null out %di */
		"mov $0x0720, %%ax\n\t" /* space */
		"mov %0, %%cx\n\t" /* get count */
		"rep stosw" /* boom */
		: /* no outputs */
		: "r" (count) /* chars per screen */
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

void
vga_setcur ( uint16_t x, uint16_t y )
	{ vga_ptr_x = x;	vga_ptr_y = y; }

void
setmode_text0 ( void )
{
	/* Set VGA to 40x25 text mode (mode 0) */
	outb(0x3D4, 0x00); /* CRTC register 0x00: Horizontal Total */
	outb(0x3D5, 0x28);
	outb(0x3D4, 0x01); /* CRTC register 0x01: Horizontal Display Enable End */
	outb(0x3D5, 0x28);
	outb(0x3D4, 0x02); /* CRTC register 0x02: Start Horizontal Blanking */
	outb(0x3D5, 0x28);
	outb(0x3D4, 0x03); /* CRTC register 0x03: End Horizontal Blanking */
	outb(0x3D5, 0x90);
	outb(0x3D4, 0x04); /* CRTC register 0x04: Start Horizontal Retrace */
	outb(0x3D5, 0x2A);
	outb(0x3D4, 0x05); /* CRTC register 0x05: End Horizontal Retrace */
	outb(0x3D5, 0x0A);
	outb(0x3D4, 0x06); /* CRTC register 0x06: Vertical Total */
	outb(0x3D5, 0x1F);
	outb(0x3D4, 0x07); /* CRTC register 0x07: Overflow */
	outb(0x3D5, 0x1F);
	outb(0x3D4, 0x08); /* CRTC register 0x08: Preset Row Scan */
	outb(0x3D5, 0x00);
	outb(0x3D4, 0x09); /* CRTC register 0x09: Maximum Scan Line */
	outb(0x3D5, 0x0F);
	outb(0x3D4, 0x0A); /* CRTC register 0x0A: Cursor Start */
	outb(0x3D5, 0x06);
	outb(0x3D4, 0x0B); /* CRTC register 0x0B: Cursor End */
	outb(0x3D5, 0x07);
	outb(0x3D4, 0x0C); /* CRTC register 0x0C: Start Address High */
	outb(0x3D5, 0x00);
	outb(0x3D4, 0x0D); /* CRTC register 0x0D: Start Address Low */
	outb(0x3D5, 0x00);
	outb(0x3D4, 0x0E); /* CRTC register 0x0E: Cursor Location High */
	outb(0x3D5, 0x00);
	outb(0x3D4, 0x0F); /* CRTC register 0x0F: Cursor Location Low */
	outb(0x3D5, 0x00);
	
	/* Set mode 0 via VGA mode register */
	outb(0x3C0, 0x10); /* Enable video */
	outb(0x3C0, 0x00); /* Set palette */
}

void
setmode_text7 ( void )
{
	/* Set VGA to 80x25 text mode (mode 3) */
	outb(0x3D4, 0x00); /* CRTC register 0x00: Horizontal Total */
	outb(0x3D5, 0x5F);
	outb(0x3D4, 0x01); /* CRTC register 0x01: Horizontal Display Enable End */
	outb(0x3D5, 0x4F);
	outb(0x3D4, 0x02); /* CRTC register 0x02: Start Horizontal Blanking */
	outb(0x3D5, 0x50);
	outb(0x3D4, 0x03); /* CRTC register 0x03: End Horizontal Blanking */
	outb(0x3D5, 0x82);
	outb(0x3D4, 0x04); /* CRTC register 0x04: Start Horizontal Retrace */
	outb(0x3D5, 0x55);
	outb(0x3D4, 0x05); /* CRTC register 0x05: End Horizontal Retrace */
	outb(0x3D5, 0x81);
	outb(0x3D4, 0x06); /* CRTC register 0x06: Vertical Total */
	outb(0x3D5, 0xBF);
	outb(0x3D4, 0x07); /* CRTC register 0x07: Overflow */
	outb(0x3D5, 0x1F);
	outb(0x3D4, 0x08); /* CRTC register 0x08: Preset Row Scan */
	outb(0x3D5, 0x00);
	outb(0x3D4, 0x09); /* CRTC register 0x09: Maximum Scan Line */
	outb(0x3D5, 0x4F);
	outb(0x3D4, 0x0A); /* CRTC register 0x0A: Cursor Start */
	outb(0x3D5, 0x0E);
	outb(0x3D4, 0x0B); /* CRTC register 0x0B: Cursor End */
	outb(0x3D5, 0x0F);
	outb(0x3D4, 0x0C); /* CRTC register 0x0C: Start Address High */
	outb(0x3D5, 0x00);
	outb(0x3D4, 0x0D); /* CRTC register 0x0D: Start Address Low */
	outb(0x3D5, 0x00);
	outb(0x3D4, 0x0E); /* CRTC register 0x0E: Cursor Location High */
	outb(0x3D5, 0x00);
	outb(0x3D4, 0x0F); /* CRTC register 0x0F: Cursor Location Low */
	outb(0x3D5, 0x00);
	
	/* Set mode 3 via VGA mode register */
	outb(0x3C0, 0x10); /* Enable video */
	outb(0x3C0, 0x00); /* Set palette */
}

void
setmode_graphic13 ( void )
{
	/* Set VGA to 320x200 256-color graphics mode (mode 13h) */
	/* Disable video during mode change */
	outb(0x3C4, 0x01); /* Sequencer register 0x01: Clocking Mode */
	outb(0x3C5, 0x00);
	
	/* Set sequencer registers */
	outb(0x3C4, 0x00); /* Sequencer register 0x00: Reset */
	outb(0x3C5, 0x03);
	outb(0x3C4, 0x01); /* Sequencer register 0x01: Clocking Mode */
	outb(0x3C5, 0x01);
	outb(0x3C4, 0x02); /* Sequencer register 0x02: Map Mask */
	outb(0x3C5, 0x0F);
	outb(0x3C4, 0x03); /* Sequencer register 0x03: Character Map Select */
	outb(0x3C5, 0x00);
	outb(0x3C4, 0x04); /* Sequencer register 0x04: Memory Mode */
	outb(0x3C5, 0x0E);
	
	/* Set CRTC registers for 320x200 */
	outb(0x3D4, 0x00); /* CRTC register 0x00: Horizontal Total */
	outb(0x3D5, 0x5F);
	outb(0x3D4, 0x01); /* CRTC register 0x01: Horizontal Display Enable End */
	outb(0x3D5, 0x4F);
	outb(0x3D4, 0x02); /* CRTC register 0x02: Start Horizontal Blanking */
	outb(0x3D5, 0x50);
	outb(0x3D4, 0x03); /* CRTC register 0x03: End Horizontal Blanking */
	outb(0x3D5, 0x82);
	outb(0x3D4, 0x04); /* CRTC register 0x04: Start Horizontal Retrace */
	outb(0x3D5, 0x55);
	outb(0x3D4, 0x05); /* CRTC register 0x05: End Horizontal Retrace */
	outb(0x3D5, 0x81);
	outb(0x3D4, 0x06); /* CRTC register 0x06: Vertical Total */
	outb(0x3D5, 0xBF);
	outb(0x3D4, 0x07); /* CRTC register 0x07: Overflow */
	outb(0x3D5, 0x1F);
	outb(0x3D4, 0x08); /* CRTC register 0x08: Preset Row Scan */
	outb(0x3D5, 0x00);
	outb(0x3D4, 0x09); /* CRTC register 0x09: Maximum Scan Line */
	outb(0x3D5, 0x4F);
	outb(0x3D4, 0x0A); /* CRTC register 0x0A: Cursor Start */
	outb(0x3D5, 0x00);
	outb(0x3D4, 0x0B); /* CRTC register 0x0B: Cursor End */
	outb(0x3D5, 0x00);
	outb(0x3D4, 0x0C); /* CRTC register 0x0C: Start Address High */
	outb(0x3D5, 0x00);
	outb(0x3D4, 0x0D); /* CRTC register 0x0D: Start Address Low */
	outb(0x3D5, 0x00);
	outb(0x3D4, 0x0E); /* CRTC register 0x0E: Cursor Location High */
	outb(0x3D5, 0x00);
	outb(0x3D4, 0x0F); /* CRTC register 0x0F: Cursor Location Low */
	outb(0x3D5, 0x00);
	
	/* Set graphics controller registers */
	outb(0x3CE, 0x00); /* Graphics register 0x00: Set/Reset */
	outb(0x3CF, 0x00);
	outb(0x3CE, 0x01); /* Graphics register 0x01: Enable Set/Reset */
	outb(0x3CF, 0x00);
	outb(0x3CE, 0x02); /* Graphics register 0x02: Color Compare */
	outb(0x3CF, 0x00);
	outb(0x3CE, 0x03); /* Graphics register 0x03: Data Rotate */
	outb(0x3CF, 0x00);
	outb(0x3CE, 0x04); /* Graphics register 0x04: Read Map Select */
	outb(0x3CF, 0x00);
	outb(0x3CE, 0x05); /* Graphics register 0x05: Graphics Mode */
	outb(0x3CF, 0x40);
	outb(0x3CE, 0x06); /* Graphics register 0x06: Miscellaneous */
	outb(0x3CF, 0x05);
	outb(0x3CE, 0x07); /* Graphics register 0x07: Color Don't Care */
	outb(0x3CF, 0x0F);
	outb(0x3CE, 0x08); /* Graphics register 0x08: Bit Mask */
	outb(0x3CF, 0xFF);
	
	/* Set attribute controller */
	outb(0x3C0, 0x10); /* Enable video */
	outb(0x3C0, 0x41); /* Mode control */
	outb(0x3C0, 0x00); /* Overscan color */
	outb(0x3C0, 0x0F); /* Color plane enable */
	outb(0x3C0, 0x00); /* Horizontal pixel panning */
	outb(0x3C0, 0x00); /* Color select */
	
	/* Re-enable video */
	outb(0x3C4, 0x00); /* Sequencer register 0x00: Reset */
	outb(0x3C5, 0x03);
}


/** Delete a character at the specified position */
void
vga_delc ( uint16_t x, uint16_t y )
{
	uint16_t *vmem = (uint16_t *)VGA_MEMADDR;
	vmem[(y * vga_curr_mode->w) + x] = ( vga_chattr ( ) | (uint16_t)' ' );
}

/** Get current cursor position */
void
vga_getcur ( uint16_t x, uint16_t y )
{
	/* This function should probably be implemented differently */
	/* For now, we'll just set the cursor position */
	vga_ptr_x = x;
	vga_ptr_y = y;
}

/** Print a string at the current cursor position */
void
vga_puts ( const char *s )
{
	while (*s) {
		if (*s == '\n') {
			vga_ptr_x = 0;
			vga_ptr_y++;
			if (vga_ptr_y >= vga_curr_mode->h) {
				vga_ptr_y = vga_curr_mode->h - 1;
			}
		} else {
			vga_char(*s, vga_ptr_x, vga_ptr_y);
			vga_ptr_x++;
			if (vga_ptr_x >= vga_curr_mode->w) {
				vga_ptr_x = 0;
				vga_ptr_y++;
				if (vga_ptr_y >= vga_curr_mode->h) {
					vga_ptr_y = vga_curr_mode->h - 1;
				}
			}
		}
		s++;
	}
}

/** Initialize VGA subsystem (identified as SYBSYS:VIDEO) */
void
vga_init ( enum vga_modes mode )
{
	/* Set current mode */
	vga_curr_mode = &vga_modes[mode];
	
	/* Initialize cursor position */
	vga_ptr_x = 0;
	vga_ptr_y = 0;
	
	/* Initialize colors */
	vga_fg = VC_WHITE;
	vga_bg = VC_BLACK;
	vga_blinking = false;
	
	/* Call the appropriate mode setting function */
	switch (mode) {
	case (VM_TEXT_0):
		setmode_text0();
		break;
	case (VM_TEXT_3):
		setmode_text7();
		break;
	case (VM_GRAPHIC_13):
		setmode_graphic13();
		break;
	default:
		gmed("Unsupported VGA mode");
		break;
	}
}

