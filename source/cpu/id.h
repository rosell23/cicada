/*
 * Copyright (c) 2025 The Cicada Project. All rights reserved.
 *
 * THE WORK IS PROVIDED "AS IS", WITHOUT ANY WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES, OR OTHER LIABILITY ARISING FROM, OUT OF, OR IN CONNECTION
 * WITH THE WORK OR ITS USE.
 */

#ifndef CPU_ID_H
#define CPU_ID_H

#include <stdint.h>

/** Structure to hold CPUID results */
typedef struct {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
} cpuid_result_t;

/** Execute CPUID with a given function ID */
static inline cpuid_result_t cpuid(uint32_t function_id)
{
    cpuid_result_t result;
    asm volatile (
        "cpuid"
        : "=a"(result.eax),
          "=b"(result.ebx),
          "=c"(result.ecx),
          "=d"(result.edx)
        : "a"(function_id)
    );
    return result;
}

/** Check if the CPU supports the CPUID instruction */
static inline int hascpuid(void)
{
    int supported;
    asm volatile (
        "pushf\n\t"              /* Save original EFLAGS */
        "pushf\n\t"
        "pop %%eax\n\t"
        "mov %%eax, %%ecx\n\t"
        "xor $0x200000, %%eax\n\t" /* Flip ID bit (bit 21) */
        "push %%eax\n\t"
        "popf\n\t"
        "pushf\n\t"
        "pop %%eax\n\t"
        "xor %%ecx, %%eax\n\t"      /* Check if bit changed */
        "shr $21, %%eax\n\t"
        "and $1, %%eax\n\t"
        "mov %%eax, %0\n\t"
        "popf"                       /* Restore original EFLAGS */
        : "=r"(supported)
        :
        : "eax", "ecx"
    );
    return supported;
}

#endif /* CPU_ID_H */

