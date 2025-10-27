/*
 * Copyright (c) 2025 The Cicada Project. All rights reserved.
 *
 * THE WORK IS PROVIDED “AS IS”, WITHOUT ANY WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES, OR OTHER LIABILITY ARISING FROM, OUT OF, OR IN
 * CONNECTION WITH THE WORK OR ITS USE.
 */

#include "../inc.h"
#include <stdbool.h>
#include <stdint.h>

#ifndef __VGA_H__
#define __VGA_H__ 1

__HEADER_BEGIN

/*** I/O Ports and addresses ***/
#define VGA_MEMADDR 0xB8000 /* VGA Video Memory address */
#define VGA_SCREEN_WIDTH 80
#define VGA_SCREEN_HEIGHT 25

enum vga_tm_colors
{
  VC_BLACK = 0x000,
  VC_BLUE = 0x001,
  VC_GREEN = 0x002,
  VC_CYAN = 0x003,
  VC_RED = 0x004,
  VC_MAGENTA = 0x005,
  VC_BROWN = 0x006,
  VC_LGRAY = 0x007,

  VC_BBLACK = 0x008,
  VC_BBLUE = 0x009,
  VC_BGREEN = 0x00A,
  VC_BCYAN = 0x00B,
  VC_BRED = 0x00C,
  VC_BMAGENTA = 0x00D,
  VC_YELLOW = 0x00E,
  VC_WHITE = 0x00F,
};

extern uint32_t _vga_cur_x;
extern uint32_t _vga_cur_y;

void vga_putch (char c);
void vga_cls (void);
void vga_puts (const char *s);
inline uint16_t vga_chattrs (char c);

__HEADER_END
#endif /* !__VGA_H__ */
