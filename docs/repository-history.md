# Reconstructed Repository History

The Git history is being reconstructed from a current source snapshot. The purpose is to create a useful engineering narrative without claiming that these commits are the literal historical commits from the original development process.

The final source tree remains the supplied current state.

## Milestones

### 1. `docs: define wireless Android Auto dongle architecture`

Establish the project's purpose, hardware target, terminology, and high-level architecture.

The project is explicitly framed as a **C++ wireless Android Auto dongle for Raspberry Pi Zero 2 W**, bridging a wireless phone and a wired-only car head unit.

### 2. `build: establish CMake project and developer tooling`

Introduce CMake, presets, compiler options, formatting, static analysis, scripts, and the development container.

### 3. `test: define core and session behavior`

Introduce the first behavioral tests for the event system, state machine, session lifecycle, and development BLE adapter.

### 4. `feat: implement event and state-machine core`

Implement the generic event queue, dispatcher, state abstractions, and state machine.

This is the reusable mechanism underneath the dongle's connection lifecycle.

### 5. `feat: implement Android Auto session lifecycle`

Add the session states that represent connection setup, readiness, running, and recovery.

The state machine is treated as a component of the dongle rather than the project's end goal.

### 6. `feat: add platform service contracts and BLE development adapter`

Introduce interfaces for Bluetooth, Wi-Fi, USB, transport, protocol, timing, and logging, plus a dummy BLE implementation for host development.

### 7. `feat: add application and transport scaffolding`

Add the application boundary and early transport/protocol integration needed to assemble the session.

### 8. `feat: add Linux drivers and Raspberry Pi target integration`

Add Linux/BlueZ-oriented drivers and the Buildroot/AArch64 integration targeting Raspberry Pi Zero 2 W.

### 9. `docs: record current development checkpoint`

Document the current implementation boundary and known build/deployment problems without fixing them.

## Why this structure

The sequence is deliberately organized around engineering intent:

```text
project definition
       ↓
build foundation
       ↓
behavioral tests
       ↓
core implementation
       ↓
session lifecycle
       ↓
platform abstractions
       ↓
application integration
       ↓
Linux / Raspberry Pi target
       ↓
current checkpoint
```

It should therefore be possible to read the Git history from oldest to newest and understand why each layer exists.

## Inspirations

The project takes inspiration from two open-source projects:

- [WirelessAndroidAutoDongle](https://github.com/nisargjhaveri/WirelessAndroidAutoDongle), which demonstrates a Raspberry Pi-based wireless Android Auto dongle for wired-only head units.
- [aa-proxy-rs](https://github.com/aa-proxy/aa-proxy-rs), which provides another implementation of the wireless-phone ↔ SBC ↔ USB-head-unit proxy model and evolved from the `aawgd` component of WirelessAndroidAutoDongle.

Those repositories are references and inspiration for this C++ project, not claims of shared implementation.
