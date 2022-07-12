/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus/)
 * Copyright (c) 2020 Liviu Ionescu.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/MIT/.
 */

#ifndef MICRO_OS_PLUS_ARCHITECTURE_AARCH32_SEMIHOSTING_INLINES_H_
#define MICRO_OS_PLUS_ARCHITECTURE_AARCH32_SEMIHOSTING_INLINES_H_

// ----------------------------------------------------------------------------

#include <stdint.h>

// ----------------------------------------------------------------------------
// Inline implementations for the AArch32 semihosting call.

#if defined(__cplusplus)
extern "C"
{
#endif // defined(__cplusplus)

  // ----------------------------------------------------------------------------

  // Type of each entry in a parameter block.
  typedef micro_os_plus_architecture_register_t
      micro_os_plus_semihosting_param_block_t;
  // Type of result.
  typedef micro_os_plus_architecture_register_t
      micro_os_plus_semihosting_response_t;

// SWI numbers and reason codes for RDI (Angel) monitors.
#define AngelSWIInsn "swi"
// The order is important, since Cortex-M defines both.
#if defined(__thumb__)
#define AngelSWI 0xAB
#elif defined(__arm__)
#define AngelSWI 0x123456

#else
#error "Unsupported architecture."
#endif

  static inline __attribute__ ((always_inline))
  micro_os_plus_semihosting_response_t
  micro_os_plus_semihosting_call_host (
      int reason, micro_os_plus_semihosting_param_block_t* arg)
  {
    micro_os_plus_semihosting_response_t value;
    __asm__ volatile(

        " mov r0, %[rsn] \n"
        " mov r1, %[arg] \n"
        " " AngelSWIInsn " %[swi] \n"
        " mov %[val], r0"

        : [val] "=r"(value) /* Outputs */
        : [rsn] "r"(reason), [arg] "r"(arg), [swi] "i"(AngelSWI) /* Inputs */
        : "r0", "r1", "r2", "r3", "ip", "lr", "memory", "cc"
        // Clobbers r0 and r1, and lr if in supervisor mode
    );

    // Accordingly to page 13-77 of ARM DUI 0040D other registers
    // can also be clobbered. Some memory positions may also be
    // changed by a system call, so they should not be kept in
    // registers. Note: we are assuming the manual is right and
    // Angel is respecting the APCS.
    return value;
  }

  // --------------------------------------------------------------------------

#if defined(__cplusplus)
}
#endif // defined(__cplusplus)

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_ARCHITECTURE_AARCH32_SEMIHOSTING_INLINES_H_

// ----------------------------------------------------------------------------
