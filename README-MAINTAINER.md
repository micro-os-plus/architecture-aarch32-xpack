[![license](https://img.shields.io/github/license/micro-os-plus/architecture-aarch32-xpack)](https://github.com/micro-os-plus/architecture-aarch32-xpack/blob/xpack/LICENSE)
[![CI on Push](https://github.com/micro-os-plus/architecture-aarch32-xpack/actions/workflows/test-ci.yml/badge.svg)](https://github.com/micro-os-plus/architecture-aarch32-xpack/actions/workflows/test-ci.yml)
[![GitHub issues](https://img.shields.io/github/issues/micro-os-plus/architecture-aarch32-xpack.svg)](https://github.com/micro-os-plus/architecture-aarch32-xpack/issues/)
[![GitHub pulls](https://img.shields.io/github/issues-pr/micro-os-plus/architecture-aarch32-xpack.svg)](https://github.com/micro-os-plus/architecture-aarch32-xpack/pulls/)

# Maintainer info

## Project repository

The project is hosted on GitHub:

- <https://github.com/micro-os-plus/architecture-aarch32-xpack.git>

To clone the stable branch (`xpack`), run the following commands in a
terminal (on Windows use the _Git Bash_ console):

```sh
rm -rf ~/Work/micro-os-plus/architecture-aarch32-xpack.git && \
mkdir -p ~/Work/micro-os-plus && \
git clone \
  https://github.com/micro-os-plus/architecture-aarch32-xpack.git \
  ~/Work/micro-os-plus/architecture-aarch32-xpack.git
```

For development purposes, clone the development branch (`xpack-development`):

```sh
rm -rf ~/Work/micro-os-plus/architecture-aarch32-xpack.git && \
mkdir -p ~/Work/micro-os-plus && \
git clone \
  --branch xpack-development \
  https://github.com/micro-os-plus/architecture-aarch32-xpack.git \
  ~/Work/micro-os-plus/architecture-aarch32-xpack.git
```

## Prerequisites

A recent [xpm](https://xpack.github.io/xpm/), which is a portable
[Node.js](https://nodejs.org/) command line application.

To check the sources, an `arm-none-eabi-gcc` toolchain is required.

## Code formatting

Code formatting is done using `clang-format` with the
`config/.clang-format` configuration, either manually with
`xpm run clang-format`, or automatically from Visual Studio Code.

Always reformat the source files that were changed.

## How to make new releases

### Release schedule

There are no fixed releases.

### Check Git

In the `micro-os-plus/architecture-aarch32-xpack` Git repo:

- switch to the `xpack-development` branch
- if needed, merge the `xpack` branch

No need to add a tag here, it'll be added when the release is created.

### Increase the version

Determine the upstream version (like `5.0.0`) and eventually update the
`package.json` file; the format is `5.0.0-pre`.

### Fix possible open issues

Check GitHub issues and pull requests:

- <https://github.com/micro-os-plus/architecture-aarch32-xpack/issues/>

and fix them; assign them to a milestone (like `5.0.0`).

### Update `README-MAINTAINER.md`

Update the `README-MAINTAINER.md` file to reflect the changes
related to the new version.

### Update `CHANGELOG.md`

- open the `CHANGELOG.md` file
- check if all previous fixed issues are in
- add a new entry like _\* v5.0.0_
- commit with a message like _prepare v5.0.0_

### Push changes

- reformat the source files that were changed
- commit and push

### Manual checks

There are no tests in this package yet; the functionality is exercised
by the tests of the packages that depend on it.

At least check that the sources compile without warnings with
`arm-none-eabi-gcc`, for both a Cortex-A and a Cortex-R device (for
example `-mcpu=cortex-a7` and `-mcpu=cortex-r5`), in both Arm (`-marm`)
and Thumb (`-mthumb`) states.

## Publish on the npmjs.com server

- select the `xpack-development` branch
- commit all changes
- `npm pack` and check the content of the archive, which should list
  only `package.json`, `README.md`, `LICENSE`, `CHANGELOG.md`,
  the sources and CMake/meson files;
  possibly adjust `.npmignore`
- `npm version 5.0.0`
- push the `xpack-development` branch to GitHub
- the `postversion` npm script should also update tags via `git push origin --tags`
- wait for the CI job to complete
  (<https://github.com/micro-os-plus/architecture-aarch32-xpack/actions/workflows/test-ci.yml>)

### Publish

- `npm login`
- `npm publish --tag test` (use `npm publish --access public` when
  publishing for the first time)

The version is visible at:

- <https://www.npmjs.com/package/@micro-os-plus/architecture-aarch32?activeTab=versions>

## Update the repo

When the package is considered stable:

- with a Git client (VS Code is fine)
- merge `xpack-development` into `xpack`
- push to GitHub
- select `xpack-development`

## Tag the npm package as `latest`

When the release is considered stable, promote it as `latest`:

- `npm dist-tag ls @micro-os-plus/architecture-aarch32`
- `npm dist-tag add @micro-os-plus/architecture-aarch32@5.0.0 latest`
- `npm dist-tag ls @micro-os-plus/architecture-aarch32`
