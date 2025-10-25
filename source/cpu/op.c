/*
 * Copyright (c) 2025 The Cicada Project. All rights reserved.
 *
 * THE WORK IS PROVIDED "AS IS", WITHOUT ANY WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES, OR OTHER LIABILITY ARISING FROM, OUT OF, OR IN CONNECTION
 * WITH THE WORK OR ITS USE.
 */

#ifndef CPU_OP_H
#define CPU_OP_H

/* Do absolutely nothing and waste a single cycle */
void
nop ( void )
{
	asm volatile ("nop");
}

/* Wait in low-power mode until an interrupt.
 *
 * Tough we all know xx86 will never be near efficient :).*/
void
halt ( void )
{
	asm volatile ("nop");;
}

#endif /* !CPU_OP_H */

