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
#include <stdbool.h>

/** Structure to hold CPUID results */
typedef struct {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
} cpuid_result_t;

/** Execute CPUID with a given function ID */
static inline cpuid_result_t	cpuid ( uint32_t function_id );

/** Check if the CPU supports the CPUID instruction */
bool hascpuid(void);

#endif /* CPU_ID_H */
