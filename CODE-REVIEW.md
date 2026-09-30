# Code review: architecture-aarch32 (v4.2.0, `xpack-development`)

Scope: `include/`, `src/`, and `tests/`, at commit `f488c07`. Every file in
scope was read in full.

Verification was performed with `arm-none-eabi-gcc` 15.2.1 (`-mcpu=cortex-a7`,
both `-marm` and `-mthumb`, `-O2 -Wall -Wextra -Wpedantic -Wconversion
-Wsign-conversion -Wmissing-declarations -Wshadow -Wundef`), with
`clang --target=armv7a-none-eabi -Weverything`, and with the project
`clang-format`. The headers were also compiled standalone to check that they
are self-contained, and one assembly file that includes the public header was
built as well.

## Summary

The library is small and generally tidy, but it has several structural
defects:

- two internal headers are not self-contained;
- the semihosting entry point silently changes linkage when the semihosting
  header is missing;
- nothing prevents the A-profile semihosting trap from being built for
  M-profile targets;
- Doxygen documentation is missing or incomplete for almost every declaration.

There are no tests of any kind. No undefined behaviour affecting the generated
code was found, and the inline assembly produced the expected instructions.

| Severity | Count |
| -------- | ----- |
| High     | 2     |
| Medium   | 6     |
| Low      | 12    |

## Issues

Issues 1 to 19 are closed; the others remain open.

### Correctness and maintainability

1. **[High] `registers.h` is not self-contained.**
   `include/micro-os-plus/architecture-aarch32/registers.h:42` and `:53` use
   `aarch32_architecture_register_t` and
   `micro_os_plus_architecture_register_t`, but the header includes only
   `defines.h` and `<stdint.h>`. Compiling it on its own fails with
   `'aarch32_architecture_register_t' does not name a type`. It works only
   because `architecture.h` happens to include `types.h` first. Add
   `#include "micro-os-plus/architecture-aarch32/types.h"`.

   **Fixed:** `registers.h` now includes `types.h`; verified standalone in C
   and C++.

2. **[High] `inlines/semihosting-inlines.h` is not self-contained.**
   Lines 30 to 39 use `micro_os_plus_architecture_register_t` and
   `micro_os_plus_architecture_signed_register_t` without including
   `types.h`. Compiling it standalone fails. Include `types.h` from it.

   **Fixed:** the header now includes `types.h`; verified standalone in C
   and C++.

3. **[Medium] Silent C++ linkage for `micro_os_plus_semihosting_call_host()`.**
   `src/semihosting.cpp:16-18` includes `micro-os-plus/semihosting.h` only if
   it exists, but line 22 compiles the definition whenever
   `MICRO_OS_PLUS_SEMIHOSTING_ENABLED` is defined. If that macro is defined
   and the semihosting package is not on the include path, the function is
   emitted with C++ linkage (verified: symbol
   `_Z35micro_os_plus_semihosting_call_hostiPm`). C callers and the
   semihosting library then fail at link time, far from the cause. GCC also
   reports `-Wmissing-declarations`. Replace the `__has_include` guard with an
   explicit `#error` when semihosting is enabled but the header is missing.
   Alternatively, declare the function `extern "C"` locally.

   **Fixed:** the whole implementation is now inside the
   `#if __has_include("micro-os-plus/semihosting.h")` block, and the header
   is still included before `MICRO_OS_PLUS_SEMIHOSTING_ENABLED` is tested,
   because that macro may be defined in `semihosting-defines.h`. Without
   the header, nothing is compiled, so a C++-linkage symbol can no longer
   be emitted; with it, the symbol has C linkage.

4. **[Medium] The wrong semihosting trap is emitted for M-profile targets.**
   `src/semihosting.cpp:27-36` selects `svc 0xAB` whenever `__thumb__` is
   defined. On M-profile cores semihosting requires `bkpt 0xAB`. Building
   with `-mcpu=cortex-m3 -mthumb` compiles without any diagnostic and
   produces a trap that causes a fault instead of reaching the debugger. The
   comment "Cortex-M defines both" is inherited from the Cortex-M package
   and is misleading here. Add
   `#if defined(__ARM_ARCH_PROFILE) && (__ARM_ARCH_PROFILE == 'M')` with an
   `#error`, and remove the comment.

   **Fixed:** the package supports only Cortex-A and Cortex-R devices in
   AArch32 state. The check is now in
   `include/micro-os-plus/architecture.h`, so it applies to every source,
   not only to the semihosting one. When
   `MICRO_OS_PLUS_ARCHITECTURES_AARCH32_ENABLED` is defined, it accepts only
   `__arm__` targets whose `__ARM_ARCH_PROFILE` is `'A'` or `'R'`; the
   M-profile case has a dedicated message. The misleading comment was
   removed, and the README files document the restriction. Verified with
   Cortex-A7, A53, R4, R5, R52, M3, and M7, with ARM926 and ARM1176, and with
   AArch64.

5. **[Medium] Header-local `static` functions called from external-linkage
   `inline` C++ functions.**
   The C wrappers are `static inline`, and therefore have internal linkage.
   The C++ `inline` functions in `inlines/instructions-inlines.h:150-215` and
   `inlines/registers-inlines.h:67-84` have external linkage and call them.
   Under the C++ one-definition rule, each translation unit's definition then
   refers to a different entity, which is ill-formed, with no diagnostic
   required. It is harmless with GCC and clang in practice because every call
   is always inlined, but it is formally incorrect. Consider making the C++
   wrappers issue the `__asm__` directly, or giving them internal linkage.
   This finding is uncertain in impact; it is reported for completeness.

   **Fixed:** a new `MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE` macro in
   `defines.h` expands to `static inline __attribute__ ((always_inline))`
   in C and to `inline __attribute__ ((always_inline))` in C++. It is used
   on all declarations and definitions of the C and C++ wrappers, so in C++
   the C functions have external linkage and every translation unit refers
   to the same entity. Verified by linking two C++ translation units that
   take `&micro_os_plus::architecture::nop`: a single weak symbol results.
   C objects still contain no local copies, at `-O0` and at `-O2`.

6. **[Medium] `_init()` is not only "added for completeness".**
   `src/_init_fini.c:30-31` states that only `_fini()` is required. Newlib's
   `__libc_init_array()` also calls `_init()` when `_HAVE_INIT_FINI` is
   defined, which is the default for `arm-none-eabi`. Correct the comment so
   that nobody removes `_init()` later.

   **Fixed:** the comment now states that newlib calls `_init()` from
   `__libc_init_array()` and `_fini()` from `__libc_fini_array()`, quotes
   both link errors, and explains that the weak definitions are needed
   only when the toolchain `crti.o` is not linked (`-nostartfiles`).
   Verified with arm-none-eabi-gcc 15.2.1: `libc_a-init.o` references
   `_init`, `libc_a-fini.o` references `_fini`, a `-nostartfiles` link
   fails on both without this file, and a default link resolves both from
   `crti.o`.

7. **[Low] Wrong local type in `aarch32_architecture_get_sp()`.**
   `inlines/registers-inlines.h:32` declares `uint32_t result;` instead of
   `aarch32_architecture_register_t`. The two types are currently identical,
   but the code should use the abstraction it defines.

   **Fixed:** the local is now `aarch32_architecture_register_t`; the
   generated code is unchanged.

8. **[Low] No `"memory"` clobber on `wfi` and `bkpt`.**
   Without it, `inlines/instructions-inlines.h:42-66` lets the compiler move
   stores past a breakpoint, so a debugger may show stale memory. It can also
   move memory accesses across `wfi` in idle or wait loops. CMSIS `__WFI()`
   uses a `"memory"` clobber; CMSIS `__BKPT()` does not (checked in both
   CMSIS Core and Core_A), so the case for `bkpt` rests on the debugging
   argument alone. (The original text of this item wrongly stated that
   both CMSIS macros use the clobber.)

   **Fixed:** both `aarch32_architecture_bkpt()` and
   `aarch32_architecture_wfi()` now declare a `"memory"` clobber; only
   `nop` remains without one. Verified with arm-none-eabi-gcc 15.2.1 at
   `-O2`. Before the change, `while (!ready) wfi();` with a non-volatile
   flag was compiled to a loop that never re-read the flag, and in
   `value = v; brk(); value = 0;` the first store was eliminated, so a
   debugger stopped at `bkpt` could not see it. After the change, the flag
   is re-read after each `wfi` and the store precedes `bkpt`. The severity
   was understated: the old code was miscompiled, not merely at risk.

   Calling code that must also have outstanding memory writes completed
   in hardware before entering low power (as Linux does with
   `dsb; wfi`) should call `micro_os_plus_architecture_data_barrier()`
   first; the clobber only constrains the compiler.

9. **[Low] Redundant register moves in the semihosting call.**
   `src/semihosting.cpp:43-54` moves the operands through `%[rsn]` and
   `%[arg]`, then into `r0` and `r1`. The disassembly shows four extra `mov`
   instructions, through `r4` and `r5`. Binding the operands directly with
   `register ... __asm__("r0")` local variables removes them, together with
   the need to clobber `r0` and `r1`.

   **Fixed:** the reason and the parameter block pointer are now bound to
   `r0` and `r1` with GNU explicit register variables, used as in/out
   operands; the `mov` instructions and the `r0`/`r1` clobbers are gone.
   At `-O2` the function is now `push {lr}; svc; pop {pc}` in both Arm and
   Thumb states, on Cortex-A7 and Cortex-R5. No `-Wregister` warning is
   issued in C++20 by arm-none-eabi-gcc 15.2.1 or, with `-Weverything`, by
   Apple clang 17. The comment typo ("Accordingly") was also corrected.

10. **[Low] `__ASSEMBLY__` is not predefined by the compiler.**
    `include/micro-os-plus/architecture.h:40` tests `__ASSEMBLY__`, but GCC
    and clang predefine `__ASSEMBLER__` for `.S` files. Including the header
    from an assembly file therefore pulls in `<stdint.h>` typedefs and fails
    (verified). The Cortex-M package works around this with a manual
    `#define __ASSEMBLY__ 1` in its `.S` files, so this is a family
    convention rather than a local bug. Consider accepting both macros.

    **Fixed:** `architecture.h` now tests `!defined(__ASSEMBLER__)`, which
    the compiler predefines for `.S` files and for `-x assembler-with-cpp`.
    Verified with arm-none-eabi-gcc 15.2.1 on Cortex-A7 and Cortex-R5, in
    Arm and Thumb states: an assembly file that includes the header without
    any extra definition assembles, and can use
    `MICRO_OS_PLUS_INTEGER_STARTUP_STACK_FILL_MAGIC`; files that still
    define `__ASSEMBLY__` by hand (the Cortex-M convention) also assemble;
    the Cortex-M `#error` is also issued from assembly; C and C++ are
    unaffected. The Cortex-M package still tests `__ASSEMBLY__`, so the two
    packages now differ; aligning it would let its `.S` files drop the
    manual definition.

11. **[Low] The C++ API is missing for `show_cpuid`.**
    `functions.h` exposes only the C form. Every other feature has C and C++,
    architecture-specific and portable variants. Either add
    `micro_os_plus::architecture::show_cpuid()` or document why it is omitted.

    **Closed, by design:** the function is internal to µOS++; its only
    caller is `run-main.cpp` in the `startup` package, when debug or trace
    output is enabled. It is not part of the application API, so no C++
    variant is needed. This is now documented in `functions.h` and in the
    API conventions of `CLAUDE.md`.

### Naming and style

12. **[Low] Line too long, and a `clang-format` violation.**
    `src/semihosting.cpp:68` is 108 characters long, and `clang-format`
    reports it. It also conflicts with the rule that each `#endif` comment
    repeats the full expression. Either split the expression or accept the
    long line explicitly. The same applies to the awkward wrapped comment at
    `include/micro-os-plus/architecture.h:20-21`.

    **Fixed:** the long `#endif` line in `src/semihosting.cpp` disappeared
    when the file was restructured with one condition per `#if`. The broken
    comment in `include/micro-os-plus/architecture.h` is now a two-line
    `/* ... */` comment that repeats the full expression, following the
    convention already used in `startup-xpack/src/run-main.cpp`; the
    project `config/.clang-format` (`ReflowComments: false`) leaves it
    unchanged. No line in `include/` or `src/` exceeds 80 characters. The
    C++20 check still accepts `-std=c++20` and rejects `-std=c++17`. The
    same `#if` in the semihosting package's `semihosting.h` has a bare
    `#endif`, which should be fixed in that package.

13. **[Low] Macros in the semihosting source.**
    The macros `AngelSWI` and `AngelSWIInsn` in `src/semihosting.cpp:28-36`
    are not `snake_case`, are never `#undef`-ed, and could be replaced by a
    `constexpr` value together with a string literal in the `__asm__`.

    **Fixed:** `AngelSWI` and `AngelSWIInsn` were replaced by
    `semihosting_svc_number`, a documented `constexpr` constant in an
    unnamed namespace, used through the `"i"` constraint; the template now
    names `svc` (the UAL mnemonic; `swi` is the pre-UAL alias with the same
    encoding). The redundant `__arm__` / `#error` branches were removed,
    since `architecture.h` already validates the target. Verified with
    arm-none-eabi-gcc 15.2.1 (Cortex-A7 and Cortex-R5, Arm and Thumb, `-O0`
    and `-O2`) and Apple clang 17 (`-Weverything`): `svc 0x123456` in Arm
    state and `svc 0xab` in Thumb state, no warnings. At `-O0` GCC also
    emits the constant as an unused 4-byte local read-only object; it
    disappears at `-O1` and above.

14. **[Low] Unused `using namespace micro_os_plus;`.**
    It appears in `src/show-cpuid.cpp:22`.

    **Fixed:** the directive and its separator block were removed; the
    function still has C linkage and builds without warnings.

15. **[Low] Copy-and-paste comment.**
    `inlines/registers-inlines.h:20` says "architecture instructions"; it
    should say "architecture registers".

    **Fixed:** the comment now says "architecture registers". The similar
    comment in `inlines/semihosting-inlines.h`, which claimed inline
    implementations of the semihosting call although the file contains
    only type definitions, was corrected as well.

### Documentation (Doxygen)

16. **[Medium] Most declarations lack `@brief` and `@details`.**
    - `instructions.h:32-81` and `:126-180` use one-line comments without
      `@brief` or `@details`.
    - `registers.h:50-54` has a one-line comment only.
    - `functions.h:24`, all of `types.h`, all of `defines.h` (including the
      meaning and byte order of `0xEFBEADDE`), the typedefs in
      `semihosting-inlines.h`, the definition in `semihosting.cpp`, and
      `_init()` and `_fini()` have no documentation at all.
    - The file-level documentation required by Doxygen for C functions
      (`@file`) is missing everywhere, so none of the C API appears in the
      generated output.

    **Fixed:** every header and source file now has a `@file` block, placed
    after the licence as in the other µOS++ packages; every declaration has
    `@brief` and `@details` (with `@param` and `@return` where applicable),
    and every definition in `inlines/` and `src/` has a `@details` block
    describing the implementation. This covers the instruction and register
    wrappers in all four forms, the types, the preprocessor definitions,
    the semihosting types, `_init()` and `_fini()`, and the semihosting
    call. Statements were checked against the code of the dependent
    packages (the stack fill value is used by the device and RTOS ports;
    `MICRO_OS_PLUS_HAS_INTERRUPTS_STACK` controls the RTOS interrupts stack
    object) and against newlib (`_init()` runs between `.preinit_array` and
    `.init_array`, `_fini()` after `.fini_array`).

    Doxygen 1.17.0 now reports no warnings, with `WARN_IF_UNDOCUMENTED`,
    `WARN_NO_PARAMDOC`, `EXTRACT_STATIC`, and `EXTRACT_ANON_NSPACES`
    enabled, and all 12 files parsed. Doxygen does not evaluate
    `__has_include`, so it must be predefined (`"__has_include(x)=1"`) and
    the semihosting package added to `INCLUDE_PATH` for `semihosting.cpp`
    to be processed. Apple clang 17 `-Wdocumentation
    -Wdocumentation-pedantic` also reports nothing. The declaration of
    `micro_os_plus_semihosting_call_host()` in the semihosting package has
    no `@param`/`@return`; they were added to the definition here, but the
    declaration should be completed in that package.

17. **[Low] Inconsistent and inaccurate wording.**
    - `brk` is described as "`break` instruction" in C (`instructions.h:72`,
      `inlines/instructions-inlines.h:101`), but as "`bkpt` instruction" in
      C++ (`instructions.h:171`).
    - American spelling ("Synchronization") appears at `instructions.h:51`,
      `:57`, `:145`, and `:151`.
    - The `isb` documentation says `isb`, while the code emits `isb sy`
      (`inlines/instructions-inlines.h:86`). The two forms are equivalent,
      but the documentation should match the code.
    - Redundant, partial comments are repeated on the inline definitions
      (`inlines/instructions-inlines.h:100-129`), but not on the first
      definitions (lines 29-92).

    **Fixed:** `brk` is described consistently as entering the debugger
    (implemented with `bkpt 0`) in all forms; "Synchronization" was
    replaced by "Synchronisation"; the barrier documentation now names
    `isb sy`, matching the code; the partial comments repeated on the
    inline definitions were replaced by `@details` blocks on every
    definition. No American spellings or contractions remain in
    `include/` or `src/`.

Doxygen was not run during the initial review; it was run when fixing
issues 16 and 17 (see above).

### Modern C++

18. **[Low] No `noexcept` on the C++ wrappers.**
    None of the C++ wrappers in `instructions.h`, `registers.h`, or their
    inline files is declared `noexcept`, although none of them can throw.

    **Fixed:** all 12 C++ wrappers (instructions and `sp()`, in both
    namespaces) are now `noexcept`, on both declarations and definitions,
    since `noexcept` is part of the function type. Verified with
    `static_assert (noexcept (...))` for every wrapper, with GCC 15.2.1
    and Apple clang 17. The C functions are shared with C, where `noexcept`
    does not exist, so they were left unchanged.

19. **[Low] Compiler-specific attributes and old-style declarations.**
    - `sp()` should be `[[nodiscard]]`.
    - In the C++ sections, `[[gnu::always_inline]]` is preferable to
      `__attribute__ ((always_inline))`.
    - `(void)` parameter lists in C++ are C-style and redundant. If they are
      kept deliberately for symmetry with the C declarations, say so in the
      coding guidelines.

    **Fixed:** both `sp()` functions are `[[nodiscard]]`; discarding the
    result now warns with GCC and clang. The C++ branch of
    `MICRO_OS_PLUS_ARCHITECTURE_ALWAYS_INLINE` now uses the standard
    attribute syntax (`[[gnu::always_inline]] inline`); the C branch keeps
    `__attribute__`, since C11 has no standard attribute syntax. Forced
    inlining is unchanged (no out-of-line copies at `-O0`). The `(void)`
    parameter lists were kept deliberately: they are the convention used
    throughout the µOS++ sources, including the C++ declarations.

### Tests

20. **[Medium] No tests at all.**
    `tests/CMakeLists.txt` contains only `# TODO`. The `xpm run test*`
    commands in the project instructions do not exist here. At a minimum, add
    a compile-only test that:
    - includes each header standalone (this would have caught issues 1
      and 2);
    - uses every C and C++ entry point in both Arm and Thumb states;
    - runs a semihosted smoke test under `qemu-system-arm` (for example
      `-M virt -cpu cortex-a15`).

## Licensing and metadata

All files in scope carry the identical MIT licence header, with plausible
year ranges. No issues were found.

## Left out, or uncertain

- The clobber list of the semihosting call (`r2`, `r3`, `ip`, `lr`, `memory`,
  and `cc`) was not verified against the current semihosting specification.
  It is conservative, and it is correct for supervisor mode, where `svc`
  overwrites `LR_svc`.
- Sanitiser builds were not run; they are not meaningful for this
  bare-metal inline assembly. Doxygen was run later, when fixing issues 16
  and 17.
- The generated `CMakeLists.txt` and `meson.build` files, and the
  `copilot-instructions.md` inconsistencies noted earlier, are outside the
  review scope.
