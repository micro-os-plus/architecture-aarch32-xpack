# Change & release log

Releases in reverse chronological order.

Please check
[GitHub](https://github.com/micro-os-plus/architecture-aarch32-xpack/issues/)
and close existing issues and pull requests.

## 2023-05-08

* v3.2.0
* 75217ed #3: add _init_fini.c
* 75217ed #3: add _init_fini.c

## 2022-08-16

* d2b4ab7 package.json min 0.14.0 & defaults

## 2022-08-03

* 22b494b .vscode/settings.json: cmake.ignoreCMakeListsMissing
* e8c242e .vscode/settings.json: makefile.configureOnOpen

## 2022-07-28

* v3.1.0
* 8bf31a0 #2: Add signed_register_t for semihosting result

## 2022-07-25

* d2a2e25 defines.h: comment out MICRO_OS_PLUS_HAS_INTERRUPTS_STACK
* ed04aca add preliminary xpack.json

## 2022-07-12

* v3.0.0
* 314d7e6 rework as aarch32
* copy/paste architecture-cortexa-xpack

## 2022-06-09

* v2.0.0 released
* 7dcab86 sections-ram.ld update stack & heap; cleanups
* 3419641 remove sections-flash.ld
* 26f568c architecture.h: include registers-inlines.h
* 8f9cbab registers-inlines.h: fix getters & setters
* d2c90ae move exceptions & vectors to qemu device for now

## 2022-06-03

* v1.0.0 released

## 2022-05-22

* v1.0.0 prepared
* copied from architecture-cortexm-xpack
