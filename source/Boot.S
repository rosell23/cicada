# Copyright (c) 2025 The Cicada Project. All rights reserved.
#
# THE WORK IS PROVIDED “AS IS”, WITHOUT ANY WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES, OR OTHER LIABILITY ARISING FROM, OUT OF, OR IN CONNECTION
# WITH THE WORK OR ITS USE.

	.section .mb2_hdr
	.align 8
	.long 0xE85250D6		# Magic
	.long 0x00000000		# Architecture (0 = i386)
	.long hdr_end - hdr_start	# header length
	.long 0x17adaf1a		# checksum

	.global _start
	.type  _start, @function

	.extern _xkst
	.type   _xkst, @function
hdr_start:
	.align 8
	.word 3			# Entry address tag type
	.word 0			# Flags
	.long 12		# Size
	.quad _start		# Entry point
hdr_end:

	.section .text

_start:
	call _xkst			# Call entry

	# If _xkst returns...

	cli				# Clear interrupts
.d:	hlt			# Halt in low-power mode
	jmp .d				# Just in case

