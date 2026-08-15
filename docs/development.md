# Development Guide

## Local workflow

The intended development loop is:

```text
change
  -> format
  -> configure/build
  -> unit tests
  -> clang-tidy
  -> commit
```

Run the complete scripted workflow with:

```bash
./scripts/all.sh
```

## CMake presets

The repository defines two main environments:

| Preset | Purpose |
|---|---|
| `Debug-host` | Host development and unit tests |
| `Debug-target` | AArch64 cross compilation through Buildroot |
| `Release-target` | Release-oriented AArch64 build |

Build presets use the corresponding configure preset:

```bash
cmake --build --preset host-debug
cmake --build --preset target-debug
cmake --build --preset target-release
```

## Docker development

The development container is intended to make the compiler/tooling environment consistent:

```bash
./scripts/dev.sh
```

The container currently contains the general C++/CMake toolchain. Linux-driver-specific dependencies still need to be made explicit before the container can be treated as the complete target-development environment.

## Tests

Tests use GoogleTest and GoogleMock through CMake `FetchContent`.

Run them with:

```bash
./scripts/test.sh debug
```

Or:

```bash
ctest --preset host-debug --output-on-failure
```

## Formatting and static analysis

Formatting is checked with clang-format:

```bash
./scripts/check-format.sh
```

To apply formatting:

```bash
./scripts/format.sh
```

Static analysis is run with clang-tidy:

```bash
./scripts/tidy.sh
```

Pre-commit configuration is also provided in `.pre-commit-config.yaml`.

## Commit conventions

The reconstructed history uses short Conventional Commit-style prefixes:

- `docs:` documentation only;
- `build:` build/tooling changes;
- `test:` tests and test infrastructure;
- `feat:` user-visible or architectural implementation milestones;
- `ci:` automation;
- `refactor:` structural changes without intended behavior changes;
- `fix:` bug fixes.

A good commit should answer one question clearly. Avoid mixing a refactor, a feature, formatting churn, and three unrelated build changes in one commit. Git history is not a landfill.

## Pull request expectations

A useful PR should include:

- a short statement of the behavior or architecture change;
- tests added or updated;
- build/test commands used;
- known limitations or follow-up work;
- hardware validation notes when the change affects the target platform.
