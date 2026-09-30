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
 * @brief Default empty definitions of `_init()` and `_fini()`.
 *
 * @details
 * Weak definitions of the initialisation and finalisation hooks required
 * by newlib, for applications that do not link the toolchain startup
 * files.
 */

#include "micro-os-plus/architecture.h"

// ----------------------------------------------------------------------------

#if defined(MICRO_OS_PLUS_ARCHITECTURES_AARCH32_ENABLED)

// ----------------------------------------------------------------------------

/**
 * @brief Initialisation hook called by newlib.
 *
 * @details
 * Called by `__libc_init_array()` after the functions in
 * `.preinit_array` and before those in `.init_array`, which include the
 * C++ static constructors.
 */
void
_init (void);

/**
 * @brief Finalisation hook called by newlib.
 *
 * @details
 * Called by `__libc_fini_array()` after the functions in `.fini_array`.
 */
void
_fini (void);

// ----------------------------------------------------------------------------

// Newlib calls `_init()` in `__libc_init_array()` and `_fini()` in
// `__libc_fini_array()`; both are required, and removing either one
// breaks the link:
//
// libc.a(libc_a-init.o): in function `__libc_init_array':
// undefined reference to `_init'
// libc.a(libc_a-fini.o): in function `__libc_fini_array':
// undefined reference to `_fini'
//
// They are normally provided by the toolchain `crti.o`, which is not
// linked when the application uses its own startup code
// (`-nostartfiles`). The definitions are weak, so that the `crti.o`
// ones, when present, take precedence.

/**
 * @details
 * Weak and empty; overridden by the `crti.o` definition, when present.
 */
__attribute__ ((weak)) void
_init (void)
{
}

/**
 * @details
 * Weak and empty; overridden by the `crti.o` definition, when present.
 */
__attribute__ ((weak)) void
_fini (void)
{
}

// ----------------------------------------------------------------------------

#endif // defined(MICRO_OS_PLUS_ARCHITECTURES_AARCH32_ENABLED)

// ----------------------------------------------------------------------------
