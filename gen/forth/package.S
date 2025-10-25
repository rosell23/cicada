.section .forth,"aw",@progbits
.global _forth_package
_forth_package:
.incbin "mforth/package.4th"
.byte 0
