#include "vga.h"            /* for vga defs */
#include "../cpu/io.h"      /* for outb/inb */
#include "../error/panic.h" /* for gmed */
#include <stddef.h>         /* for NULL */
#include <stdint.h>         /* for fixed-size types */

#define VGA_MEMADDR 0xB8000
#define VGA_SCREEN_WIDTH 80
#define VGA_SCREEN_HEIGHT 25
#define VGA_DEFAULT_ATTR 0x07 /* light gray on black */

uint32_t _vga_cur_x = 0;
uint32_t _vga_cur_y = 0;

inline uint16_t
vga_chattrs (char c)
{
  return ((uint16_t)VGA_DEFAULT_ATTR << 8) | (uint8_t)c;
}

/* Scroll the screen up by one line */
void
vga_scroll (void)
{
  volatile uint16_t *vga_mem = (volatile uint16_t *)VGA_MEMADDR;

  for (size_t y = 1; y < VGA_SCREEN_HEIGHT; y++)
    {
      for (size_t x = 0; x < VGA_SCREEN_WIDTH; x++)
        {
          vga_mem[(y - 1) * VGA_SCREEN_WIDTH + x]
              = vga_mem[y * VGA_SCREEN_WIDTH + x];
        }
    }

  /* Clear the last line */
  uint16_t blank = vga_chattrs (' ');
  for (size_t x = 0; x < VGA_SCREEN_WIDTH; x++)
    {
      vga_mem[(VGA_SCREEN_HEIGHT - 1) * VGA_SCREEN_WIDTH + x] = blank;
    }

  if (_vga_cur_y > 0)
    _vga_cur_y--;
}

/* Write a character at the current cursor position */
void
vga_putch (char c)
{
  volatile uint16_t *vga_mem = (volatile uint16_t *)VGA_MEMADDR;

  if (c == '\n')
    {
      _vga_cur_x = 0;
      _vga_cur_y++;
    }
  else if (c == '\r')
    {
      _vga_cur_x = 0;
    }
  else
    {
      vga_mem[_vga_cur_y * VGA_SCREEN_WIDTH + _vga_cur_x] = vga_chattrs (c);
      _vga_cur_x++;
      if (_vga_cur_x >= VGA_SCREEN_WIDTH)
        {
          _vga_cur_x = 0;
          _vga_cur_y++;
        }
    }

  if (_vga_cur_y >= VGA_SCREEN_HEIGHT)
    {
      vga_scroll ();
    }
}

/* Write a null-terminated string */
void
vga_puts (const char *s)
{
  if (!s)
    return;
  while (*s)
    {
      vga_putch (*s++);
    }
}
