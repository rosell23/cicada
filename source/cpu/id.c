/*
 * Copyright (c) 2025 The Cicada Project. All rights reserved.
 *
 * THE WORK IS PROVIDED "AS IS", WITHOUT ANY WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES, OR OTHER LIABILITY ARISING FROM, OUT OF, OR IN
 * CONNECTION WITH THE WORK OR ITS USE.
 */

#include "id.h"
#include <stdbool.h> /* for bool */

/** Execute CPUID with a given function ID */
cpuid_result_t
cpuid (uint32_t function_id)
{
  cpuid_result_t result;
  asm volatile ("cpuid"
                : "=a"(result.eax), "=b"(result.ebx), "=c"(result.ecx),
                  "=d"(result.edx)
                : "a"(function_id));
  return result;
}

/** Check if the CPU supports the CPUID instruction */
_Bool
hascpuid (void)
{
  _Bool supported;
  asm volatile ("pushfq\n\t"               /* Save original RFLAGS */
                "mov %%rax, %%rcx\n\t"     /* Copy to RCX for comparison */
                "xor $0x200000, %%rax\n\t" /* Flip ID bit (bit 21) */
                "push %%rax\n\t"
                "popfq\n\t"
                "pushfq\n\t"
                "pop %%rax\n\t"
                "xor %%rcx, %%rax\n\t" /* Check if bit changed */
                "shr $21, %%rax\n\t"   /* Move ID bit to bit 0 */
                "and $1, %%rax\n\t"    /* Mask everything else */
                "mov %%al, %0\n\t"     /* Move lower 8 bits to _Bool output */
                "popfq"                /* Restore original RFLAGS */
                : "=r"(supported)
                :
                : "rax", "rcx");
  return supported;
}
