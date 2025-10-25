/*
 * Copyright (c) 2025 The Cicada Project. All rights reserved.
 *
 * THE WORK IS PROVIDED “AS IS”, WITHOUT ANY WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES, OR OTHER LIABILITY ARISING FROM, OUT OF, OR IN CONNECTION
 * WITH THE WORK OR ITS USE.
*/

#include "panic.h"
#include "../cpu/op.h"

void __attribute__((noreturn))
gmed ( const char * cause )
{
	

	for (;;)
		nop ();
}

