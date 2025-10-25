/*
 * Copyright (c) 2025 The Cicada Project. All rights reserved.
 *
 * THE WORK IS PROVIDED “AS IS”, WITHOUT ANY WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES, OR OTHER LIABILITY ARISING FROM, OUT OF, OR IN CONNECTION
 * WITH THE WORK OR ITS USE.
*/

#include "driver/vga.h"
#include "cpu/op.h"

void
_xkst ( void )
{
	vga_init (VM_TEXT_0);
	vga_setcol (VC_WHITE, VC_BBLACK);

	vga_cls ();
	vga_puts ("== CICADA V0.1.0 ==");

	nop ();
	while(1) {
		halt ();
	}
}
