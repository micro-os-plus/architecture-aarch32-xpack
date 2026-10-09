/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2022-2026 Liviu Ionescu. All rights reserved.
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
 * @brief Inline definitions of the AArch32 functions accessing CPU
 * registers.
 *
 * @details
 * The architecture specific C functions contain the inline assembly; all
 * the other forms forward to them.
 *
 * This file is included at the end of `registers.h` and must not be
 * included directly.
 */

#ifndef MICRO_OS_PLUS_ARCHITECTURE_AARCH32_INLINES_REGISTERS_INLINES_H_
#define MICRO_OS_PLUS_ARCHITECTURE_AARCH32_INLINES_REGISTERS_INLINES_H_

// ----------------------------------------------------------------------------

#include <stdint.h>

// ----------------------------------------------------------------------------
// Inline implementations for the AArch32 architecture registers.

#if defined(__cplusplus)
extern "C"
{
#endif // defined(__cplusplus)

  // --------------------------------------------------------------------------

  /**
   * @details
   * The assembly statement is `volatile`, since the value changes at run
   * time without the compiler knowing it, and each call must read the
   * register again.
   */
  MICRO_OS_PLUS_ALWAYS_INLINE aarch32_architecture_register_t
  aarch32_architecture_get_sp (void)
  {
    aarch32_architecture_register_t result;

    __asm__ volatile (

        "mov %0, sp"

        : "=r"(result) /* Outputs */
        : /* Inputs */
        : /* Clobbers */
    );

    return result;
  }

  /**
   * @details
   * The `"memory"` clobber prevents the compiler from moving memory
   * accesses, including stack accesses, across the change.
   */
  MICRO_OS_PLUS_ALWAYS_INLINE void
  aarch32_architecture_set_sp (aarch32_architecture_register_t top_of_stack)
  {
    __asm__ volatile (

        " mov sp, %0 "

        : /* Outputs */
        : "r"(top_of_stack) /* Inputs */
        : "memory" /* Clobbers */
    );
  }

  /**
   * @details
   * Forwards to `aarch32_architecture_get_sp()`.
   */
  MICRO_OS_PLUS_ALWAYS_INLINE
  micro_os_plus_architecture_register_t
  micro_os_plus_architecture_get_sp (void)
  {
    return aarch32_architecture_get_sp ();
  }

  /**
   * @details
   * Forwards to `aarch32_architecture_set_sp()`.
   */
  MICRO_OS_PLUS_ALWAYS_INLINE void
  micro_os_plus_architecture_set_sp (
      micro_os_plus_architecture_register_t top_of_stack)
  {
    aarch32_architecture_set_sp (top_of_stack);
  }

  // --------------------------------------------------------------------------

#if defined(__cplusplus)
}
#endif // defined(__cplusplus)

// ============================================================================

#if defined(__cplusplus)

namespace aarch32::architecture::registers
{
  // --------------------------------------------------------------------------

  /**
   * @details
   * Forwards to `aarch32_architecture_get_sp()`.
   */
  [[nodiscard]] MICRO_OS_PLUS_ALWAYS_INLINE register_t
  sp (void) noexcept
  {
    return aarch32_architecture_get_sp ();
  }

  /**
   * @details
   * Forwards to `aarch32_architecture_set_sp()`.
   */
  MICRO_OS_PLUS_ALWAYS_INLINE void
  sp (register_t top_of_stack) noexcept
  {
    aarch32_architecture_set_sp (top_of_stack);
  }

  // --------------------------------------------------------------------------
} // namespace aarch32::architecture::registers

namespace micro_os_plus::architecture::registers
{
  // --------------------------------------------------------------------------

  /**
   * @details
   * Forwards to `micro_os_plus_architecture_get_sp()`.
   */
  [[nodiscard]] MICRO_OS_PLUS_ALWAYS_INLINE register_t
  sp (void) noexcept
  {
    return micro_os_plus_architecture_get_sp ();
  }

  /**
   * @details
   * Forwards to `micro_os_plus_architecture_set_sp()`.
   */
  MICRO_OS_PLUS_ALWAYS_INLINE void
  sp (register_t top_of_stack) noexcept
  {
    micro_os_plus_architecture_set_sp (top_of_stack);
  }

  // --------------------------------------------------------------------------
} // namespace micro_os_plus::architecture::registers

#endif // defined(__cplusplus)

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_ARCHITECTURE_AARCH32_INLINES_REGISTERS_INLINES_H_

// ----------------------------------------------------------------------------
