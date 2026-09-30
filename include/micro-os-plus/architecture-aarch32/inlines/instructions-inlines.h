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

  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  aarch32_architecture_bkpt (void)
  {
    __asm__ volatile (

        " bkpt 0 "

        : /* Outputs */
        : /* Inputs */
        : /* Clobbers */
    );
  }

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

  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  micro_os_plus_architecture_nop (void)
  {
    aarch32_architecture_nop ();
  }

  /**
   * `break` instruction.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  micro_os_plus_architecture_brk (void)
  {
    aarch32_architecture_bkpt ();
  }

  /**
   * `wfi` instruction.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  micro_os_plus_architecture_wfi (void)
  {
    aarch32_architecture_wfi ();
  }

  /**
   * Data synchronisation barrier (`dsb sy`).
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  micro_os_plus_architecture_data_barrier (void)
  {
    aarch32_architecture_dsb ();
  }

  /**
   * Instruction synchronisation barrier (`isb`).
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

  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  nop (void) noexcept
  {
    aarch32_architecture_nop ();
  }

  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  bkpt (void) noexcept
  {
    aarch32_architecture_bkpt ();
  }

  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  wfi (void) noexcept
  {
    aarch32_architecture_wfi ();
  }

  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  dsb (void) noexcept
  {
    aarch32_architecture_dsb ();
  }

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

  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  nop (void) noexcept
  {
    aarch32::architecture::nop ();
  }

  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  brk (void) noexcept
  {
    aarch32::architecture::bkpt ();
  }

  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  wfi (void) noexcept
  {
    aarch32::architecture::wfi ();
  }

  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  data_barrier (void) noexcept
  {
    aarch32::architecture::dsb ();
  }

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
