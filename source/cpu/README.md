# CPU dir #

Here there are a few CPU operations.

> [!IMPORTANT]
> This directory is a major source of machine-dependent code.

## Files ##

+ `id.c/.h` &mdash; [_CPUID_](https://en.wikipedia.org/wiki/CPUID) (CPU
	identification) wrapper.
+ `io.c/.h` &mdash; Input and output from specific ports. Treat with care!
+ op.c/.h` &mdash; A few CPU operations; mainly (and ironically) no-op
	and halting.
