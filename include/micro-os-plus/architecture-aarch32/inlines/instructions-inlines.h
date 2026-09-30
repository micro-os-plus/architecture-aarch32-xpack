/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2017-2026 Liviu Ionescu. All rights reserved.
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
 * @brief Inline definitions of the AArch32 functions wrapping CPU
 * instructions.
 *
 * @details
 * The architecture specific C functions contain the inline assembly; all
 * the other forms (portable C, and both C++ namespaces) forward to them,
 * so that each instruction is written only once.
 *
 * This file is included at the end of `instructions.h` and must not be
 * included directly.
 */

#ifndef MICRO_OS_PLUS_ARCHITECTURE_AARCH32_INLINES_INSTRUCTIONS_INLINES_H_
#define MICRO_OS_PLUS_ARCHITECTURE_AARCH32_INLINES_INSTRUCTIONS_INLINES_H_

// ----------------------------------------------------------------------------

#include <stdint.h>

// ----------------------------------------------------------------------------
// Inline implementations for the AArch32 architecture instructions.

#if defined(__cplusplus)
extern "C"
{
#endif // defined(__cplusplus)

  // --------------------------------------------------------------------------

  /**
   * @details
   * The assembly statement is `volatile`, so that it is not removed; it
   * has no clobbers, since the instruction has no effect on registers or
   * memory.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  aarch32_architecture_nop (void)
  {
    __asm__ volatile (

        " nop "

        : /* Outputs */
        : /* Inputs */
        : /* Clobbers */
    );
  }

  /**
   * @details
   * The `"memory"` clobber prevents the compiler from moving or
   * eliminating stores across the breakpoint.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  aarch32_architecture_bkpt (void)
  {
    __asm__ volatile (

        " bkpt 0 "

        : /* Outputs */
        : /* Inputs */
        : "memory" /* Clobbers */
    );
  }

  /**
   * @details
   * The `"memory"` clobber forces the compiler to read again, after the
   * instruction, any values that interrupt handlers may have changed.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  aarch32_architecture_wfi (void)
  {
    __asm__ volatile (

        " wfi "

        : /* Outputs */
        : /* Inputs */
        : "memory" /* Clobbers */
    );
  }

  /**
   * @details
   * The `"memory"` clobber makes the hardware barrier also a compiler
   * barrier, so that memory accesses are not moved across it.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  aarch32_architecture_dsb (void)
  {
    __asm__ volatile (

        " dsb sy "

        : /* Outputs */
        : /* Inputs */
        : "memory" /* Clobbers */
    );
  }

  /**
   * @details
   * The `"memory"` clobber makes the hardware barrier also a compiler
   * barrier, so that memory accesses are not moved across it.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  aarch32_architecture_isb (void)
  {
    __asm__ volatile (

        " isb sy "

        : /* Outputs */
        : /* Inputs */
        : "memory" /* Clobbers */
    );
  }

  /**
   * @details
   * Forwards to `aarch32_architecture_nop()`.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  micro_os_plus_architecture_nop (void)
  {
    aarch32_architecture_nop ();
  }

  /**
   * @details
   * Forwards to `aarch32_architecture_bkpt()`.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  micro_os_plus_architecture_brk (void)
  {
    aarch32_architecture_bkpt ();
  }

  /**
   * @details
   * Forwards to `aarch32_architecture_wfi()`.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  micro_os_plus_architecture_wfi (void)
  {
    aarch32_architecture_wfi ();
  }

  /**
   * @details
   * Forwards to `aarch32_architecture_dsb()`.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  micro_os_plus_architecture_data_barrier (void)
  {
    aarch32_architecture_dsb ();
  }

  /**
   * @details
   * Forwards to `aarch32_architecture_isb()`.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  micro_os_plus_architecture_instruction_barrier (void)
  {
    aarch32_architecture_isb ();
  }

  // --------------------------------------------------------------------------

#if defined(__cplusplus)
}
#endif // defined(__cplusplus)

// ============================================================================

#if defined(__cplusplus)

namespace aarch32::architecture
{
  // --------------------------------------------------------------------------

  /**
   * @details
   * Forwards to `aarch32_architecture_nop()`.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  nop (void) noexcept
  {
    aarch32_architecture_nop ();
  }

  /**
   * @details
   * Forwards to `aarch32_architecture_bkpt()`.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  bkpt (void) noexcept
  {
    aarch32_architecture_bkpt ();
  }

  /**
   * @details
   * Forwards to `aarch32_architecture_wfi()`.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  wfi (void) noexcept
  {
    aarch32_architecture_wfi ();
  }

  /**
   * @details
   * Forwards to `aarch32_architecture_dsb()`.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  dsb (void) noexcept
  {
    aarch32_architecture_dsb ();
  }

  /**
   * @details
   * Forwards to `aarch32_architecture_isb()`.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  isb (void) noexcept
  {
    aarch32_architecture_isb ();
  }

  // --------------------------------------------------------------------------
} // namespace aarch32::architecture

namespace micro_os_plus::architecture
{
  // --------------------------------------------------------------------------

  /**
   * @details
   * Forwards to `aarch32::architecture::nop()`.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  nop (void) noexcept
  {
    aarch32::architecture::nop ();
  }

  /**
   * @details
   * Forwards to `aarch32::architecture::bkpt()`.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  brk (void) noexcept
  {
    aarch32::architecture::bkpt ();
  }

  /**
   * @details
   * Forwards to `aarch32::architecture::wfi()`.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  wfi (void) noexcept
  {
    aarch32::architecture::wfi ();
  }

  /**
   * @details
   * Forwards to `aarch32::architecture::dsb()`.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  data_barrier (void) noexcept
  {
    aarch32::architecture::dsb ();
  }

  /**
   * @details
   * Forwards to `aarch32::architecture::isb()`.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  instruction_barrier (void) noexcept
  {
    aarch32::architecture::isb ();
  }

  // --------------------------------------------------------------------------
} // namespace micro_os_plus::architecture

#endif // defined(__cplusplus)

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_ARCHITECTURE_AARCH32_INLINES_INSTRUCTIONS_INLINES_H_

// ----------------------------------------------------------------------------
