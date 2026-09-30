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

#ifndef MICRO_OS_PLUS_ARCHITECTURE_AARCH32_INSTRUCTIONS_H_
#define MICRO_OS_PLUS_ARCHITECTURE_AARCH32_INSTRUCTIONS_H_

// ----------------------------------------------------------------------------

#include "micro-os-plus/architecture-aarch32/defines.h"

#include <stdint.h>

// ----------------------------------------------------------------------------
// Declarations of AArch32 functions to wrap architecture instructions.

#if defined(__cplusplus)
extern "C"
{
#endif // defined(__cplusplus)

  // --------------------------------------------------------------------------
  // Architecture assembly instructions in C.

  /**
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  aarch32_architecture_nop (void);

  /**
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  aarch32_architecture_bkpt (void);

  /**
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  aarch32_architecture_wfi (void);

  /**
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  aarch32_architecture_dsb (void);

  /**
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  aarch32_architecture_isb (void);

  // --------------------------------------------------------------------------
  // Portable architecture assembly instructions in C.

  /**
   * `nop` instruction.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  micro_os_plus_architecture_nop (void);

  /**
   * `break` instruction.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  micro_os_plus_architecture_brk (void);

  /**
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  micro_os_plus_architecture_wfi (void);

  /**
   * @brief Data synchronisation barrier.
   *
   * @details
   * Ensures that all explicit memory accesses, and all system register
   * writes, issued before this call complete before any instruction
   * after it executes. It also acts as a compiler memory barrier.
   *
   * On AArch32, it is implemented with the `dsb sy` instruction.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  micro_os_plus_architecture_data_barrier (void);

  /**
   * @brief Instruction synchronisation barrier.
   *
   * @details
   * Ensures that the instructions after this call are fetched and
   * executed only after the effects of the preceding context-changing
   * operations (such as system register writes, or code written to
   * memory) are visible. Usually called right after
   * `micro_os_plus_architecture_data_barrier()`.
   *
   * On AArch32, it is implemented with the `isb` instruction.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  micro_os_plus_architecture_instruction_barrier (void);

  // --------------------------------------------------------------------------

#if defined(__cplusplus)
}
#endif // defined(__cplusplus)

// ============================================================================

#if defined(__cplusplus)

namespace aarch32::architecture
{
  // --------------------------------------------------------------------------
  // Architecture assembly instructions in C++.

  /**
   * The assembler `nop` instruction.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  nop (void) noexcept;

  /**
   * The assembler `bkpt` instruction.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  bkpt (void) noexcept;

  /**
   * The assembler `wfi` instruction.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  wfi (void) noexcept;

  /**
   * The assembler `dsb sy` (Data Synchronization Barrier) instruction.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  dsb (void) noexcept;

  /**
   * The assembler `isb` (Instruction Synchronization Barrier) instruction.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  isb (void) noexcept;

  // --------------------------------------------------------------------------
} // namespace aarch32::architecture

namespace micro_os_plus::architecture
{
  // --------------------------------------------------------------------------
  // Portable architecture assembly instructions in C++.

  /**
   * The assembler `nop` instruction.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  nop (void) noexcept;

  /**
   * The assembler `bkpt` instruction.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  brk (void) noexcept;

  /**
   * The assembler `wfi` instruction.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  wfi (void) noexcept;

  /**
   * @brief Data synchronisation barrier.
   *
   * @details
   * The C++ equivalent of `micro_os_plus_architecture_data_barrier()`;
   * on AArch32, it is implemented with the `dsb sy` instruction.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  data_barrier (void) noexcept;

  /**
   * @brief Instruction synchronisation barrier.
   *
   * @details
   * The C++ equivalent of
   * `micro_os_plus_architecture_instruction_barrier()`; on AArch32,
   * it is implemented with the `isb` instruction.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE void
  instruction_barrier (void) noexcept;

  // --------------------------------------------------------------------------
} // namespace micro_os_plus::architecture

#endif // defined(__cplusplus)

// ============================================================================
// Templates, inlines & constexpr implementations.

#include "inlines/instructions-inlines.h"

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_ARCHITECTURE_AARCH32_INSTRUCTIONS_H_

// ----------------------------------------------------------------------------
