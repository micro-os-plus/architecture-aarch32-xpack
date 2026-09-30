[![GitHub package.json version](https://img.shields.io/github/package-json/v/micro-os-plus/architecture-aarch32-xpack)](https://github.com/micro-os-plus/architecture-aarch32-xpack/blob/xpack/package.json)
[![GitHub tag (latest by date)](https://img.shields.io/github/v/tag/micro-os-plus/architecture-aarch32-xpack)](https://github.com/micro-os-plus/architecture-aarch32-xpack/tags/)
[![npm (scoped)](https://img.shields.io/npm/v/@micro-os-plus/architecture-aarch32.svg?color=blue)](https://www.npmjs.com/package/@micro-os-plus/architecture-aarch32/)
[![license](https://img.shields.io/github/license/micro-os-plus/architecture-aarch32-xpack)](https://github.com/micro-os-plus/architecture-aarch32-xpack/blob/xpack/LICENSE)
[![CI on Push](https://github.com/micro-os-plus/architecture-aarch32-xpack/actions/workflows/ci.yml/badge.svg)](https://github.com/micro-os-plus/architecture-aarch32-xpack/actions/workflows/ci.yml)

# A source code library with the µOS++ architecture definitions for Arm AArch32 (32-bit Cortex-A and Cortex-R)

This project provides the **architecture-aarch32** source library as an `xpm`
dependency and includes architecture definitions for embedded projects
running on Arm Cortex-A and Cortex-R devices in AArch32 (32-bit) state.

Cortex-M devices are **not** supported by this package; they are supported
by the separate
[architecture-cortexm](https://github.com/micro-os-plus/architecture-cortexm-xpack)
package.

The project is hosted on GitHub as
[micro-os-plus/architecture-aarch32-xpack](https://github.com/micro-os-plus/architecture-aarch32-xpack).

This page is addressed to developers who plan to include this source
library into their own projects.

For maintainer information, please see the
[README-MAINTAINER](README-MAINTAINER.md) file.

## Install

As a source library xpm package, the easiest way to add it to a project is via
**xpm**, but it can also be used as any Git project, for example as a submodule.

### Prerequisites

A recent [xpm](https://xpack.github.io/xpm/),
which is a portable [Node.js](https://nodejs.org/) command line application.

It is recommended to update to the latest version with:

```sh
npm install --global xpm@latest
```

For details please follow the instructions in the
[xPack install](https://xpack.github.io/install/) page.

### xpm

This package is available as
[`@micro-os-plus/architecture-aarch32`](https://www.npmjs.com/package/@micro-os-plus/architecture-aarch32)
from the `npmjs.com` registry:

```sh
cd my-project
xpm init # Unless a package.json is already present

xpm install @micro-os-plus/architecture-aarch32@latest

ls -l xpacks/@micro-os-plus/architecture-aarch32
```

### Git submodule

If, for any reason, **xpm** is not available, the next recommended
solution is to link it as a Git submodule below an `xpacks` folder.

```sh
cd my-project
git init # Unless already a Git project
mkdir -p xpacks

git submodule add https://github.com/micro-os-plus/architecture-aarch32-xpack.git \
  xpacks/@micro-os-plus/architecture-aarch32
```

## Branches

Apart from the unused `master` branch, there are two active branches:

- `xpack`, with the latest stable version (default)
- `xpack-development`, with the current development version

All development is done in the `xpack-development` branch, and contributions via
Pull Requests should be directed to this branch.

When new releases are published, the `xpack-development` branch is merged
into `xpack`.

## Developer info

### Overview

This source xpm package provides general AArch32 definitions for
Cortex-A and Cortex-R devices.

### Supported devices

Only Arm Cortex-A (A-profile) and Cortex-R (R-profile) devices running in
AArch32 (32-bit) state, in either the Arm or the Thumb instruction set, are
supported. Both profiles use the same semihosting trap (`svc 0x123456` in
Arm state, `svc 0xAB` in Thumb state) and the same instructions wrapped by
this package.

Cortex-M (M-profile) devices differ in their exception model, stack pointers,
and semihosting trap instruction, and are supported by the separate
[architecture-cortexm](https://github.com/micro-os-plus/architecture-cortexm-xpack)
package.

When `MICRO_OS_PLUS_ARCHITECTURES_AARCH32_ENABLED` is defined, the
`micro-os-plus/architecture.h` header checks the ACLE `__ARM_ARCH_PROFILE`
macro and stops the build with an `#error` for:

- M-profile (Cortex-M) devices;
- AArch64 (64-bit) targets;
- classic Arm cores (for example ARM9 or ARM11), which have no
  architecture profile.

### Status

The **architecture-aarch32** source library is fully functional,
but minimalistic, for running semihosted tests.

### Build & integration info

The project is written in C++, C, and inline assembly, and it is
expected to be used in C and C++ projects.

The source code is tested with arm-none-eabi-gcc 11 to 15, for Cortex-A
and Cortex-R devices (for example `-mcpu=cortex-a7` or `-mcpu=cortex-r5`,
with `-marm` or `-mthumb`), and should be warning free.

To ease the integration of this package into user projects,
ready-made CMake and meson configuration files are provided (see below).

For other build systems, consider the following details:

#### Include folders

The following folders should be passed to the compiler during the build:

- `include`

The header files to be included in user projects are:

```c++
#include "micro-os-plus/architecture.h"
```

#### Source files

The source files to be added to user projects are:

- `src/show-cpuid.cpp` - empty, this architecture has no CPUID
- `src/semihosting.cpp` - the semihosting call for this platform
- `src/_init_fini.c` - empty definitions required by newlib

#### Preprocessor definitions

The configuration definitions are expected either in the specific header file
`micro-os-plus/architecture-defines.h` or in the common project configuration
file `micro-os-plus/project-config.h`:

- `MICRO_OS_PLUS_ARCHITECTURES_AARCH32_ENABLED` - enable the inclusion of the
  architecture files in the build; without it, the headers and the source
  files are empty; it must be defined only for Cortex-A and Cortex-R
  devices
- `MICRO_OS_PLUS_SEMIHOSTING_ENABLED` - enable the semihosting call in
  `src/semihosting.cpp`; requires the `semihosting` package

#### Compiler options

- `-std=c++20` or higher for C++ sources
- `-std=c11` for C sources

#### C++ Namespaces

Portable:

- `micro_os_plus::architecture`
- `micro_os_plus::architecture::registers`

Architecture specific:

- `aarch32::architecture`
- `aarch32::architecture::registers`

#### C++ Classes

- none

#### Dependencies

- `@micro-os-plus/semihosting` - optional, required only when semihosting is used

#### CMake

To integrate the architecture-aarch32 source library into a CMake application,
add this folder to the build:

```cmake
add_subdirectory("xpacks/@micro-os-plus/architecture-aarch32")
```

The result is an interface library that can be added as an application
dependency with:

```cmake
target_link_libraries(your-target PRIVATE

  micro-os-plus::architectures-aarch32
)
```

The generic alias `micro-os-plus::architecture` refers to the same
library, and can be used when the application is architecture agnostic.

#### meson

To integrate the architecture-aarch32 library into a meson application,
add this folder to the build:

```meson
subdir('xpacks/@micro-os-plus/architecture-aarch32')
```

The result is a dependency object that can be added
to an application with:

```meson
exe = executable(
  'your-target',
  c_args: micro_os_plus_architectures_aarch32_dependency_compile_c_args,
  cpp_args: micro_os_plus_architectures_aarch32_dependency_compile_cpp_args,
  link_with: [
    # Nothing, not static.
  ],
  dependencies: [
    micro_os_plus_architectures_aarch32_dependency,
  ]
)
```

The generic `micro_os_plus_architecture_dependency` object (with the
corresponding `_compile_c_args` and `_compile_cpp_args` variables) refers
to the same sources, and can be used when the application is
architecture agnostic.

#### xCDL

The package metadata for the xCDL build configuration tools is
available in `xcdl-package.jsonc`.

#### Linker scripts

The `linker-scripts` folder includes `sections-ram.ld`, a generic
linker script for configurations running entirely from RAM; it may be
redefined at device level.

### Examples

With `MICRO_OS_PLUS_ARCHITECTURES_AARCH32_ENABLED` defined:

```c++
#include "micro-os-plus/architecture.h"

using namespace micro_os_plus;

architecture::register_t reg;

reg = architecture::registers::sp();
```

### Known problems

- does not use CMSIS Core (yet)

### Tests

There are no tests in this package yet; the functionality is exercised
by the tests of the packages that depend on it, like
[micro-test-plus](https://github.com/micro-os-plus/micro-test-plus-xpack).

## Change log - incompatible changes

According to [semver](https://semver.org) rules:

> Major version X (X.y.z | X > 0) MUST be incremented if any
> backwards incompatible changes are introduced to the public API.

The incompatible changes, in reverse chronological order,
are:

- v4.x:
  - the CMake alias was renamed `micro-os-plus::architectures-aarch32`
    (the generic `micro-os-plus::architecture` was preserved)
  - the specific meson dependency is
    `micro_os_plus_architectures_aarch32_dependency`
  - `MICRO_OS_PLUS_ARCHITECTURES_AARCH32_ENABLED` must be defined to enable this library
  - `src/semihosting.cpp` and `src/show-cpuid.cpp` were added
    and must be compiled
  - the inline headers were moved to the `inlines` folder
  - `aarch32::architecture::registers::msp()` was renamed `sp()`
- v3.x: rework as aarch32

(previous versions were part of `architecture-cortexa`)

## License

Unless otherwise stated, the content is released under the terms of the
[MIT License](https://opensource.org/licenses/mit),
with all rights reserved to
[Liviu Ionescu](https://github.com/ilg-ul).
