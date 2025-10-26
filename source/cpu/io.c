/*
 * Copyright (c) 2025 The Cicada Project. All rights reserved.
 *
 * THE WORK IS PROVIDED "AS IS", WITHOUT ANY WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES, OR OTHER LIABILITY ARISING FROM, OUT OF, OR IN
 * CONNECTION WITH THE WORK OR ITS USE.
 */

#include "io.h"     /* to implement */
#include <stdint.h> /* for fixed-size types */

/** Output a byte to an I/O port */
void
outb (uint16_t port, uint8_t val)
{
  asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

/** Input a byte from an I/O port */
uint8_t
inb (uint16_t port)
{
  uint8_t ret;
  asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}
