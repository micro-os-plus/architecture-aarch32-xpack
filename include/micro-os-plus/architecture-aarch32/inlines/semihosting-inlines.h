/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2020-2026 Liviu Ionescu. All rights reserved.
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
 * @brief AArch32 type definitions used by the semihosting package.
 *
 * @details
 * The `semihosting` package uses these architecture specific types in
 * its declarations; the call itself,
 * `micro_os_plus_semihosting_call_host()`, is implemented in
 * `src/semihosting.cpp`.
 */

#ifndef MICRO_OS_PLUS_ARCHITECTURE_AARCH32_INLINES_SEMIHOSTING_INLINES_H_
#define MICRO_OS_PLUS_ARCHITECTURE_AARCH32_INLINES_SEMIHOSTING_INLINES_H_

// ----------------------------------------------------------------------------

#include "micro-os-plus/architecture-aarch32/types.h"

#include <stdint.h>

// ----------------------------------------------------------------------------
// AArch32 type definitions used by the semihosting call; the call itself
// is implemented in `src/semihosting.cpp`.

#if defined(__cplusplus)
extern "C"
{
#endif // defined(__cplusplus)

  // --------------------------------------------------------------------------

  /**
   * @brief Type of each field in the semihosting structures.
   *
   * @details
   * Semihosting structures, such as the heap information block, have
   * fields of the register size.
   */
  typedef micro_os_plus_architecture_register_t
      micro_os_plus_semihosting_register_t;

  /**
   * @brief Type of each entry in a semihosting parameter block.
   *
   * @details
   * The parameter block, passed by address in `r1`, is an array of
   * register size words.
   */
  typedef micro_os_plus_architecture_register_t
      micro_os_plus_semihosting_param_block_t;

  /**
   * @brief Type of the value returned by a semihosting call.
   *
   * @details
   * Signed, since several operations return -1 to report errors.
   */
  typedef micro_os_plus_architecture_signed_register_t
      micro_os_plus_semihosting_response_t;

  // --------------------------------------------------------------------------

#if defined(__cplusplus)
}
#endif // defined(__cplusplus)

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_ARCHITECTURE_AARCH32_INLINES_SEMIHOSTING_INLINES_H_

// ----------------------------------------------------------------------------
