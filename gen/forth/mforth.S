.section .forth,"aw",@progbits
.global _forth_mforth
_forth_mforth:
.incbin "mforth/mforth.4th"
.byte 0
