/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2023-2026 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can be
 * obtained from https://opensource.org/licenses/mit.
 */

// ----------------------------------------------------------------------------

#include "micro-os-plus/architecture.h"

#if defined(MICRO_OS_PLUS_ARCHITECTURES_AARCH32_ENABLED)

// The whole implementation depends on this header: it must be included
// before testing MICRO_OS_PLUS_SEMIHOSTING_ENABLED, which may be defined
// in `micro-os-plus/semihosting-defines.h`, and its declaration gives the
// definition below C linkage.
#if __has_include("micro-os-plus/semihosting.h")
#include "micro-os-plus/semihosting.h"

// ----------------------------------------------------------------------------

#if defined(MICRO_OS_PLUS_SEMIHOSTING_ENABLED)

// ----------------------------------------------------------------------------

namespace
{
// ---------------------------------------------------------------------------

// SWI numbers and reason codes for RDI (Angel) monitors.
#define AngelSWIInsn "swi"
// The order is important, since Cortex-M defines both.
#if defined(__thumb__)
  constexpr micro_os_plus_architecture_register_t semihosting_svc_number
      = 0xAB;
#else
  constexpr micro_os_plus_architecture_register_t semihosting_svc_number
      = 0x123456;
#endif // defined(__thumb__)
} // namespace

micro_os_plus_semihosting_response_t
micro_os_plus_semihosting_call_host (
    int reason, micro_os_plus_semihosting_param_block_t* arg)
{
  // The semihosting ABI passes the reason in r0 and the parameter block
  // address in r1, and returns the result in r0. Binding the operands
  // directly to these registers (GNU explicit register variables, still
  // valid in C++17 and later) avoids the extra moves the compiler would
  // otherwise generate. r1 is also declared as an in/out operand, so
  // that the compiler conservatively does not assume it is preserved.
  register micro_os_plus_semihosting_response_t value __asm__ ("r0") = reason;
  register micro_os_plus_semihosting_param_block_t* param __asm__ ("r1") = arg;

  __asm__ volatile (

      " svc %[swi] \n"

      : [val] "+r"(value), [arg] "+r"(param) /* Outputs */
      : [swi] "i"(semihosting_svc_number) /* Inputs */
      : "r2", "r3", "ip", "lr", "memory", "cc" /* Clobbers */
  );

  // According to page 13-77 of ARM DUI 0040D, other registers
  // can also be clobbered (lr in supervisor mode, since `svc`
  // overwrites it). Some memory positions may also be
  // changed by a system call, so they should not be kept in
  // registers. Note: we are assuming the manual is right and
  // Angel is respecting the APCS.
  return value;
}

// ----------------------------------------------------------------------------

#endif // defined(MICRO_OS_PLUS_SEMIHOSTING_ENABLED)

// ----------------------------------------------------------------------------

#endif // __has_include("micro-os-plus/semihosting.h")

// ----------------------------------------------------------------------------

#endif // defined(MICRO_OS_PLUS_ARCHITECTURES_AARCH32_ENABLED)

// ----------------------------------------------------------------------------
