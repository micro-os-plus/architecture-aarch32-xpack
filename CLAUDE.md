# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with
code in this repository.

@.github/copilot-instructions.md

## Project Overview

`@micro-os-plus/architecture-aarch32` is a small µOS++ source library (an xpm
package, also usable as a Git submodule) that provides the architecture layer
for Arm **Cortex-A and Cortex-R** devices in AArch32 (32-bit) state: register
types, wrappers for a few CPU instructions, a Stack Pointer getter, and the
Angel semihosting call.
It is minimalistic and exists mainly to run semihosted tests (for example
under QEMU). It does not use CMSIS Core.

Only the A and R profiles are supported. Cortex-M devices are supported by
the separate `architecture-cortexm` package. `architecture.h` accepts only
`__arm__` targets with `__ARM_ARCH_PROFILE` equal to `'A'` or `'R'`, and
issues an `#error` for M-profile, AArch64, and classic (profile-less) cores.
Any new instruction wrapper must exist on both Armv7-A/Armv8-A (AArch32) and
Armv7-R/Armv8-R. Do not add M-profile code paths (for example `bkpt 0xAB`
semihosting, or MSP/PSP handling) here.

There is no library to build here; consumers compile the sources as part of
their own application.

## Branches

- `xpack-development`: all development; pull requests target this branch.
- `xpack`: latest stable release; `xpack-development` is merged into it on
  release.
- `master`: unused.

## Layout

- `include/micro-os-plus/architecture.h`: the single public entry header.
  It includes `micro-os-plus/project-config.h` and
  `micro-os-plus/architecture-defines.h` if present, and exposes the rest only
  when `MICRO_OS_PLUS_ARCHITECTURES_AARCH32_ENABLED` is defined; in that case
  it also rejects targets other than 32-bit Cortex-A and Cortex-R. It
  requires C++20 when compiled as C++.
- `include/micro-os-plus/architecture-aarch32/`:
  - `defines.h`: architecture macros (also safe for assembly sources).
  - `types.h`: `aarch32_architecture_register_t` (`uint32_t`), the signed
    variant, and the portable `micro_os_plus_architecture_*` aliases.
  - `instructions.h`: `nop`, `bkpt`/`brk`, `wfi`, `dsb`/`data_barrier`, and
    `isb`/`instruction_barrier`.
  - `registers.h`: `sp()` getter (setters are still TODO).
  - `functions.h`: `micro_os_plus_architecture_show_cpuid()`.
  - `inlines/`: the inline definitions (inline assembly) of the declarations
    above, plus the semihosting type aliases in `semihosting-inlines.h`.
- `src/`:
  - `semihosting.cpp`: `micro_os_plus_semihosting_call_host()` using `swi`
    (`0xAB` in Thumb, `0x123456` in Arm state); compiled only when
    `micro-os-plus/semihosting.h` is available and
    `MICRO_OS_PLUS_SEMIHOSTING_ENABLED` is also defined. The header must be
    included before that macro is tested, since it may be defined in
    `semihosting-defines.h`.
  - `show-cpuid.cpp`: intentionally empty (AArch32 has no CPUID here).
  - `_init_fini.c`: empty `_init()`/`_fini()` required by newlib.
- `linker-scripts/sections-ram.ld`: generic RAM-only linker script, which
  device packages may override.
- `scripts/`: Node.js helpers run through xpm actions (`clang-format`,
  `cmake-format`, `jsonc-format`, `xcdl-export`), plus Liquid templates.

## API conventions

Every application-facing feature (instructions, registers) is exposed in four
equivalent forms, which must be kept in sync when adding or changing
functionality:

1. C, architecture specific: `aarch32_architecture_*()`.
2. C, portable: `micro_os_plus_architecture_*()`.
3. C++, architecture specific: `aarch32::architecture[::registers]`.
4. C++, portable: `micro_os_plus::architecture[::registers]`.

Internal functions used only by other µOS++ packages, such as
`micro_os_plus_architecture_show_cpuid()` (called by the `startup` package),
are exposed only as portable C functions and need no C++ variant.

The C functions are declared `static` in the headers and defined in the
matching `inlines/*-inlines.h` file. The portable names are part of the
contract shared by all µOS++ architecture packages (for example
`architecture-cortexm`), so renaming them is a breaking change and must be
recorded in the README "incompatible changes" list and `CHANGELOG.md`.

Note the naming asymmetry: the AArch32 specific breakpoint is `bkpt`, whilst
the portable name is `brk`.

## Build integration files are generated

`CMakeLists.txt` and `meson.build` are generated from `xcdl-package.jsonc`
using `scripts/xcdl-export.mjs` and the templates in `scripts/templates/`. Do
not edit them by hand; edit `xcdl-package.jsonc` (for example to add a source
file) or the templates, then regenerate:

```sh
xpm run xcdl-export
```

Exported targets:

- CMake: `micro-os-plus::architectures-aarch32` and the generic
  `micro-os-plus::architecture` (an `INTERFACE` library; sources are compiled
  by the consumer).
- meson: `micro_os_plus_architectures_aarch32_dependency` and the generic
  `micro_os_plus_architecture_dependency`, with the corresponding
  `_compile_c_args` and `_compile_cpp_args` variables.

The dependency on `@micro-os-plus/semihosting` is deliberately not declared
(it would be circular); `semihosting.cpp` uses `__has_include` instead.

Other files marked `DO NOT EDIT! Automatically generated` (for example
`.github/workflows/test-ci.yml`) come from `@xpack/npm-packages-helper`
templates via `npm run generate-top-commons`.

## Common commands

```sh
npm install                  # install the Node.js helpers
xpm install                  # install xpm devDependencies (clang)
xpm run clang-format         # format C/C++ sources
xpm run cmake-format         # format CMake files
xpm run jsonc-format         # format JSON/JSONC files
xpm run xcdl-export          # regenerate CMakeLists.txt and meson.build
```

Always run `clang-format` on the C/C++ files that were changed.

## Testing

This package has no tests of its own: `tests/CMakeLists.txt` is a placeholder
and `tests/` has no `package.json`, so the `xpm run test*` commands listed in
`.github/copilot-instructions.md` do not apply to this repository. The code
is exercised by dependent packages, such as `micro-test-plus-xpack`. Changes
should at least be checked to compile warning-free with `arm-none-eabi-gcc`
(versions 11 to 15 are supported) for both a Cortex-A and a Cortex-R target
(for example `-mcpu=cortex-a7` and `-mcpu=cortex-r5`), in both Arm and Thumb
states.

## Releases

The release procedure (version bump to `x.y.z-pre`, `CHANGELOG.md`,
`npm pack`, `npm version`, `npm publish --tag test`, merging into `xpack`,
and `npm dist-tag`) is described in `README-MAINTAINER.md`.
