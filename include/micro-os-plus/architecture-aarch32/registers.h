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
 * @brief Declarations of the AArch32 functions accessing CPU registers.
 *
 * @details
 * As for the instructions, each register accessor is available as
 * architecture specific and portable C functions, and in the
 * `aarch32::architecture::registers` and
 * `micro_os_plus::architecture::registers` C++ namespaces.
 *
 * All functions are always inlined; their definitions are in
 * `inlines/registers-inlines.h`, included at the end of this file.
 */

#ifndef MICRO_OS_PLUS_ARCHITECTURE_AARCH32_REGISTERS_H_
#define MICRO_OS_PLUS_ARCHITECTURE_AARCH32_REGISTERS_H_

// ----------------------------------------------------------------------------

#include "micro-os-plus/architecture-aarch32/defines.h"
#include "micro-os-plus/architecture-aarch32/types.h"

#include <stdint.h>

// ----------------------------------------------------------------------------
// Declarations of AArch32 functions to wrap architecture instructions.

#if defined(__cplusplus)
extern "C"
{
#endif // defined(__cplusplus)

  // --------------------------------------------------------------------------
  // Architecture registers getters and mutators in C.

  /**
   * @brief Get the current Stack Pointer.
   *
   * @details
   * Reads the banked SP (R13) of the current processor mode.
   * A-profile and R-profile AArch32 cores have no separate
   * Main/Process stack pointers, unlike M-profile cores.
   *
   * @return The value of the Stack Pointer.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE aarch32_architecture_register_t
  aarch32_architecture_get_sp (void);

  // TODO: add setter.

  // --------------------------------------------------------------------------
  // Portable architecture assembly instructions in C.

  /**
   * @brief Get the current Stack Pointer.
   *
   * @details
   * Portable wrapper, available on all µOS++ architectures; on AArch32
   * it reads the banked SP (R13) of the current processor mode.
   *
   * @return The value of the Stack Pointer.
   */
  MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE
  micro_os_plus_architecture_register_t
  micro_os_plus_architecture_get_sp (void);

  // TODO: add setter.

  // --------------------------------------------------------------------------

#if defined(__cplusplus)
}
#endif // defined(__cplusplus)

// ============================================================================

#if defined(__cplusplus)

namespace aarch32::architecture::registers
{
  // --------------------------------------------------------------------------
  // Architecture getters in C++.

  /**
   * @brief Get the current Stack Pointer.
   *
   * @details
   * C++ wrapper for `aarch32_architecture_get_sp()`; reads the
   * banked SP (R13) of the current processor mode.
   *
   * @return The value of the Stack Pointer.
   */
  [[nodiscard]] MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE register_t
  sp (void) noexcept;

  // TODO: add setter.

  // --------------------------------------------------------------------------
} // namespace aarch32::architecture::registers

namespace micro_os_plus::architecture::registers
{
  // --------------------------------------------------------------------------
  // Portable architecture assembly instructions in C++.

  /**
   * @brief Get the current Stack Pointer.
   *
   * @details
   * Portable wrapper for `micro_os_plus_architecture_get_sp()`,
   * available on all µOS++ architectures.
   *
   * @return The value of the Stack Pointer.
   */
  [[nodiscard]] MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE register_t
  sp (void) noexcept;

  // TODO: add setter.

  // --------------------------------------------------------------------------
} // namespace micro_os_plus::architecture::registers

#endif // defined(__cplusplus)

// ============================================================================
// Templates, inlines & constexpr implementations.

#include "inlines/registers-inlines.h"

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_ARCHITECTURE_AARCH32_REGISTERS_H_

// ----------------------------------------------------------------------------
