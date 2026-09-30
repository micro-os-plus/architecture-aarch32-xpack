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

/**
 * @file
 * @brief AArch32 implementation of the semihosting call.
 *
 * @details
 * Implements `micro_os_plus_semihosting_call_host()`, declared in the
 * `semihosting` package, using the `svc` instruction, as defined by the
 * Arm semihosting specification for A-profile and R-profile cores.
 *
 * Nothing is compiled unless the architecture is enabled, the
 * `semihosting` package is available, and semihosting is enabled.
 */

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
  /**
   * @brief Semihosting trap number, used as the `svc` immediate.
   *
   * @details
   * `0xAB` in Thumb state and `0x123456` in Arm state, as defined by the
   * Arm semihosting specification for A-profile and R-profile cores
   * (historically the Angel debug monitor SWI numbers).
   *
   * The 32-bit Arm target is already validated by
   * `micro-os-plus/architecture.h`, so only the instruction set state
   * needs to be tested here.
   */
#if defined(__thumb__)
  constexpr micro_os_plus_architecture_register_t semihosting_svc_number
      = 0xAB;
#else
  constexpr micro_os_plus_architecture_register_t semihosting_svc_number
      = 0x123456;
#endif // defined(__thumb__)
} // namespace

/**
 * @details
 * Traps to the debugger, which performs the operation identified by
 * `reason`, using the parameters in the block pointed to by `arg`, and
 * returns its result.
 *
 * The call is synchronous. If the debugger or emulator (such as QEMU)
 * does not intercept the `svc` instruction, it raises a Supervisor Call
 * exception, handled by the application SVC handler, which normally does
 * not expect it; therefore semihosting must be enabled only when running
 * under a debugger or an emulator with semihosting support.
 *
 * @param reason The semihosting operation number, passed in `r0`.
 * @param arg Pointer to the parameter block (or, for some operations, a
 * single value), passed in `r1`.
 * @return The operation result, returned in `r0`; its meaning depends on
 * the operation, and many operations return -1 on error.
 */
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
