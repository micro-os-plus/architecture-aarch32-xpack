/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2021-2026 Liviu Ionescu. All rights reserved.
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
 * @brief AArch32 architecture preprocessor definitions.
 *
 * @details
 * The definitions in this file are also used from assembly sources,
 * therefore it must contain only preprocessor directives.
 */

#ifndef MICRO_OS_PLUS_ARCHITECTURE_AARCH32_DEFINES_H_
#define MICRO_OS_PLUS_ARCHITECTURE_AARCH32_DEFINES_H_

// ----------------------------------------------------------------------------

// Not defined for AArch32, therefore the RTOS does not provide a separate
// interrupts stack object.
// #define MICRO_OS_PLUS_HAS_INTERRUPTS_STACK

/**
 * @brief Value used to fill the stack at startup.
 *
 * @details
 * The device and RTOS port packages fill stacks with this 32-bit value,
 * so that the unused part can later be identified, and the maximum stack
 * usage computed. On little-endian cores it is stored in memory as the
 * bytes `DE AD BE EF`, which are easy to recognise in a memory dump.
 */
#define MICRO_OS_PLUS_INTEGER_STARTUP_STACK_FILL_MAGIC (0xEFBEADDE)

/**
 * @brief Specifiers for the always inlined architecture wrappers.
 *
 * @details
 * Used on both the declarations and the definitions of the functions
 * wrapping architecture instructions and registers, in C and in C++.
 *
 * In C, the functions are `static inline`, since plain `inline` would
 * require an external definition in one translation unit.
 *
 * In C++, the functions are `inline` with external linkage, so that the
 * external linkage C++ wrappers refer to the same entity in all
 * translation units, as required by the One Definition Rule; with
 * `static`, each translation unit would get a distinct function.
 *
 * In both languages, inlining is forced, also in non-optimised builds;
 * in C++ with the standard attribute syntax (`[[gnu::always_inline]]`),
 * in C with `__attribute__ ((always_inline))`, since C11 has no standard
 * attribute syntax.
 *
 * In C++ the macro expands to an attribute, therefore it must be the
 * first element of the declaration, after any other standard attributes
 * such as `[[nodiscard]]`.
 */
#if defined(__cplusplus)
#define MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE [[gnu::always_inline]] inline
#else
#define MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE \
  static inline __attribute__ ((always_inline))
#endif // defined(__cplusplus)

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_ARCHITECTURE_AARCH32_DEFINES_H_

// ----------------------------------------------------------------------------
