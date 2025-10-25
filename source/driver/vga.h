/*
 * Copyright (c) 2025 The Cicada Project. All rights reserved.
 *
 * THE WORK IS PROVIDED “AS IS”, WITHOUT ANY WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES, OR OTHER LIABILITY ARISING FROM, OUT OF, OR IN CONNECTION
 * WITH THE WORK OR ITS USE.
*/

#include "../inc.h"
#include <stdint.h>
#include <stdbool.h>

#ifndef __VGA_H__
#define __VGA_H__ 1

__HEADER_BEGIN

/*** I/O Ports and addresses ***/
#define VGA_MEMADDR	0xB8000 /* VGA Video Memory address */
#define VGA_CRTC_INDEX	0x3D4 /* CRT Controller Index Register */
#define VGA_CRTC_DATA	0x3D5 /* CRT Controller Data Register */

typedef void ( *vga_draw_cback ) ( uint16_t,uint16_t,void* );
typedef void ( *vga_set_cback ) ( void );

enum	vga_modes
{
	VM_TEXT_7 = 0x0007,
	VM_TEXT_0 = 0x0000,

	VM_GRAPHIC_13 = 0x0013
};

/** A VGA mode */
struct	vga_mode
{
	/* Callback to set the mode*/
	vga_set_cback  *setm;

	/* Callback to, set a pixel */
	vga_draw_cback *setpix;

	uint16_t w;
	uint16_t h;

	uint8_t depth;

	enum vga_modes id;
};

enum	vga_tm_colors
{
	VC_BLACK   = 0x000,
	VC_BLUE    = 0x001,
	VC_GREEN   = 0x002,
	VC_CYAN    = 0x003,
	VC_RED     = 0x004,
	VC_MAGENTA = 0x005,
	VC_BROWN   = 0x006,
	VC_LGRAY   = 0x007,

	VC_BBLACK   = 0x008,
	VC_BBLUE    = 0x009,
	VC_BGREEN   = 0x00A,
	VC_BCYAN    = 0x00B,
	VC_BRED     = 0x00C,
	VC_BMAGENTA = 0x00D,
	VC_YELLOW   = 0x00E,
	VC_WHITE    = 0x00F,
};

void	        vga_setcol ( enum vga_tm_colors fg, enum vga_tm_colors bg );
inline void	vga_char   ( char c, uint16_t x, uint16_t y );
void		vga_cls    ( void );
void		vga_delc   ( uint16_t x, uint16_t y );
void		vga_setcur ( uint16_t x, uint16_t y );
void		vga_getcur ( uint16_t x, uint16_t y );
void		vga_puts   ( const char *s );
void		vga_init   ( enum vga_modes m );

inline uint16_t	vga_chattr ( void );

static enum vga_tm_colors	vga_fg;
static enum vga_tm_colors	vga_bg;
static bool                     vga_blinking;

static struct vga_mode	*vga_curr_mode;

__HEADER_END

#endif /* !__VGA_H__ */

