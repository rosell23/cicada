/*
 * Copyright (c) 2025 The Cicada Project. All rights reserved.
 *
 * THE WORK IS PROVIDED “AS IS”, WITHOUT ANY WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES, OR OTHER LIABILITY ARISING FROM, OUT OF, OR IN CONNECTION
 * WITH THE WORK OR ITS USE.
*/

MEMORY
{
	ROM (rx)  : org = 0x00100000, len = 256k
	RAM (rwx) : org = 0x00800000, len = 2m
}

SECTIONS
{
	. = 1M;

	.mb2_hdr :
	{
		KEEP ( *(.mb2_hdr) )
	} > ROM

	.text : ALIGN ( 0X1000 )
	{
		KEEP ( *(.text._start) )
		_ld_text_start = .;
		*(.text*)
		_ld_text_end = .;
	} > ROM

	.data : ALIGN ( 0X1000 )
	{
	_ld_data_start = .;
		*(.data)
	_ld_data_end = .;
	} > RAM

	.bss : ALIGN ( 0X1000 )
	{
	_ld_bss_start = .;
		*(.bss)
	_ld_bss_end = .;
	} > RAM
}

