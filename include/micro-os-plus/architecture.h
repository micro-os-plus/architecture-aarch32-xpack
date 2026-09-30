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
 * @brief Main header of the µOS++ AArch32 architecture package.
 *
 * @details
 * This is the only header applications should include. It first includes
 * the optional `micro-os-plus/project-config.h` and
 * `micro-os-plus/architecture-defines.h` configuration headers, then,
 * only when `MICRO_OS_PLUS_ARCHITECTURES_AARCH32_ENABLED` is defined,
 * validates the target and includes the internal headers from the
 * `micro-os-plus/architecture-aarch32` folder.
 *
 * Only Cortex-A and Cortex-R devices in AArch32 state are supported; the
 * build stops with an error for Cortex-M devices (supported by the
 * separate architecture-cortexm package), for AArch64 targets, and for
 * classic Arm cores without an architecture profile.
 *
 * When included from assembly sources (`__ASSEMBLER__` defined), only the
 * definitions usable from assembly are included.
 *
 * C++ sources require C++20 or later.
 */

#ifndef MICRO_OS_PLUS_ARCHITECTURE_AARCH32_ARCHITECTURE_H_
#define MICRO_OS_PLUS_ARCHITECTURE_AARCH32_ARCHITECTURE_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)
#if !(__cplusplus >= 202002L || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L))
#error "C++20 or higher is required"
#endif /* !(__cplusplus >= 202002L
          || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)) */
#endif // defined(__cplusplus)

#if __has_include("micro-os-plus/project-config.h")
#include "micro-os-plus/project-config.h"
#endif // __has_include("micro-os-plus/project-config.h")

#if __has_include("micro-os-plus/architecture-defines.h")
#include "micro-os-plus/architecture-defines.h"
#endif // __has_include("micro-os-plus/architecture-defines.h")

// ----------------------------------------------------------------------------

#if defined(MICRO_OS_PLUS_ARCHITECTURES_AARCH32_ENABLED)

// ----------------------------------------------------------------------------

// This package supports only Cortex-A (A-profile) and Cortex-R (R-profile)
// devices in AArch32 state. Cortex-M (M-profile) devices differ in
// exception model, stack pointers, and semihosting trap, and are supported
// by the separate architecture-cortexm package. AArch64 targets and the
// classic Arm cores, which have no architecture profile, are rejected too.
#if defined(__ARM_ARCH_PROFILE) && (__ARM_ARCH_PROFILE == 'M')
#error "Cortex-M devices are not supported, use architecture-cortexm"
#elif !defined(__arm__) || defined(__aarch64__)
#error "Only 32-bit Arm (AArch32) targets are supported"
#elif !defined(__ARM_ARCH_PROFILE) \
    || !((__ARM_ARCH_PROFILE == 'A') || (__ARM_ARCH_PROFILE == 'R'))
#error "Only Cortex-A and Cortex-R devices are supported"
#endif // defined(__ARM_ARCH_PROFILE) && (__ARM_ARCH_PROFILE == 'M')

#include "micro-os-plus/architecture-aarch32/defines.h"

#if !defined(__ASSEMBLER__)

#include "micro-os-plus/architecture-aarch32/types.h"
#include "micro-os-plus/architecture-aarch32/functions.h"
#include "micro-os-plus/architecture-aarch32/instructions.h"
#include "micro-os-plus/architecture-aarch32/registers.h"

#include "micro-os-plus/architecture-aarch32/inlines/semihosting-inlines.h"

#endif // !defined(__ASSEMBLER__)

// ----------------------------------------------------------------------------

#endif // defined(MICRO_OS_PLUS_ARCHITECTURES_AARCH32_ENABLED)

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_ARCHITECTURE_AARCH32_ARCHITECTURE_H_

// ----------------------------------------------------------------------------
