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
 * @brief AArch32 implementation of the CPU identification display.
 *
 * @details
 * Decodes the `MIDR` (Main ID Register) fields and writes a line like
 * `Arm Cortex-A7 r0p5` to the trace output channel.
 *
 * The implementer codes and the output format are the same as in the
 * AArch64 and Cortex-M packages. The Cortex-A7, A8, A9, A15, R5, and R52
 * part numbers were checked on QEMU; the others are the values published
 * by Arm.
 */

#include "micro-os-plus/architecture.h"

// ----------------------------------------------------------------------------

#if defined(MICRO_OS_PLUS_ARCHITECTURE_AARCH32_ENABLED)

// The trace output is optional; without the `diag-trace` package, the
// function is empty.
#if __has_include("micro-os-plus/diag/trace.h")
#include "micro-os-plus/diag/trace.h"

#include <cstdint>

// ----------------------------------------------------------------------------

namespace
{
  /**
   * @brief Get the name of a CPU implementer.
   *
   * @details
   * The implementer codes are assigned by Arm; only the most common ones
   * are listed.
   *
   * @param implementer The `MIDR.Implementer` field (bits [31:24]).
   * @return A pointer to a string literal with the implementer name, or
   * `nullptr` if the code is not known.
   */
  [[nodiscard]] const char*
  get_implementer_name (std::uint32_t implementer) noexcept
  {
    switch (implementer)
      {
      case 0x41U:
        return "Arm";
      case 0x42U:
        return "Broadcom";
      case 0x43U:
        return "Cavium";
      case 0x46U:
        return "Fujitsu";
      case 0x48U:
        return "HiSilicon";
      case 0x4EU:
        return "NVIDIA";
      case 0x50U:
        return "APM";
      case 0x51U:
        return "Qualcomm";
      case 0x61U:
        return "Apple";
      case 0x6DU:
        return "Microsoft";
      case 0xC0U:
        return "Ampere";
      default:
        return nullptr;
      }
  }

  /**
   * @brief Get the name of an Arm Ltd. core.
   *
   * @details
   * Only meaningful when the implementer is Arm (`0x41`); the part
   * numbers of the other implementers overlap with these.
   *
   * Only the cores that can run in AArch32 state at PL1 (EL1) are
   * listed; the later 64-bit cores support AArch32 only at EL0, or not at
   * all.
   *
   * @param part_num The `MIDR.PartNum` field (bits [15:4]).
   * @return A pointer to a string literal with the core name, or
   * `nullptr` if the part number is not known.
   */
  [[nodiscard]] const char*
  get_arm_part_name (std::uint32_t part_num) noexcept
  {
    switch (part_num)
      {
      // Armv7-A.
      case 0xC05U:
        return "Cortex-A5";
      case 0xC07U:
        return "Cortex-A7";
      case 0xC08U:
        return "Cortex-A8";
      case 0xC09U:
        return "Cortex-A9";
      case 0xC0DU:
        return "Cortex-A12";
      case 0xC0EU:
        return "Cortex-A17";
      case 0xC0FU:
        return "Cortex-A15";
      // Armv7-R.
      case 0xC14U:
        return "Cortex-R4";
      case 0xC15U:
        return "Cortex-R5";
      case 0xC17U:
        return "Cortex-R7";
      case 0xC18U:
        return "Cortex-R8";
      // Armv8-A, with AArch32 at all exception levels.
      case 0xD01U:
        return "Cortex-A32";
      case 0xD03U:
        return "Cortex-A53";
      case 0xD04U:
        return "Cortex-A35";
      case 0xD07U:
        return "Cortex-A57";
      case 0xD08U:
        return "Cortex-A72";
      case 0xD09U:
        return "Cortex-A73";
      // Armv8-R.
      case 0xD13U:
        return "Cortex-R52";
      case 0xD16U:
        return "Cortex-R52+";
      default:
        return nullptr;
      }
  }
} // namespace

#endif // __has_include("micro-os-plus/diag/trace.h")

// ----------------------------------------------------------------------------

/**
 * @details
 * Reads `MIDR` (CP15 c0, c0, 0), which requires a privileged mode (PL1
 * or higher; in User mode the instruction is undefined), decodes its
 * fields, and writes them to the trace output channel:
 *
 * - Implementer, bits [31:24], as a name when known (e.g. `Arm`);
 * - PartNum, bits [15:4], as a core name when the implementer is Arm and
 *   the part is known (e.g. `Cortex-A7`), otherwise in hexadecimal;
 * - Variant, bits [23:20], and Revision, bits [3:0], as `rVpR`;
 * - Architecture, bits [19:16], only when it is not `0xF`, the value
 *   used by all Armv7 and later cores.
 *
 * When the `diag-trace` package is not available, the function is empty.
 */
void
micro_os_plus_architecture_show_cpuid (void)
{
#if __has_include("micro-os-plus/diag/trace.h")
  using namespace micro_os_plus;

  std::uint32_t midr;
  __asm__ volatile (

      " mrc p15, 0, %0, c0, c0, 0 "

      : "=r"(midr) /* Outputs */
      : /* Inputs */
      : /* Clobbers */
  );

  // On arm-none-eabi `std::uint32_t` is `unsigned long`; the fields are
  // converted to `unsigned int`, to match the `%u` and `%X` formats.
  const auto implementer = static_cast<unsigned int> ((midr >> 24) & 0xFFU);
  const auto variant = static_cast<unsigned int> ((midr >> 20) & 0x0FU);
  const auto architecture = static_cast<unsigned int> ((midr >> 16) & 0x0FU);
  const auto part_num = static_cast<unsigned int> ((midr >> 4) & 0xFFFU);
  const auto revision = static_cast<unsigned int> (midr & 0x0FU);

  const char* implementer_name = get_implementer_name (implementer);
  const char* part_name
      = (implementer == 0x41U) ? get_arm_part_name (part_num) : nullptr;

  if (implementer_name != nullptr)
    {
      trace::printf ("%s ", implementer_name);
    }
  else
    {
      trace::printf ("Implementer 0x%02X ", implementer);
    }

  if (part_name != nullptr)
    {
      trace::printf ("%s", part_name);
    }
  else
    {
      trace::printf ("part 0x%03X", part_num);
    }

  trace::printf (" r%up%u", variant, revision);

  if (architecture != 0xFU)
    {
      trace::printf (" (architecture 0x%X)", architecture);
    }

  trace::printf ("\n");
#endif // __has_include("micro-os-plus/diag/trace.h")
}

// ----------------------------------------------------------------------------

#endif // defined(MICRO_OS_PLUS_ARCHITECTURE_AARCH32_ENABLED)

// ----------------------------------------------------------------------------
