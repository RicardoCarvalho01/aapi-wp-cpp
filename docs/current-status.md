# Current Development Checkpoint

This document describes the source snapshot from which the repository history is being reconstructed.

## Project objective

The project is a C++ implementation of a **wireless Android Auto dongle**, with the **Raspberry Pi Zero 2 W** as the primary target.

The intended topology is:

```text
Android phone
     │
 Wi-Fi / BT
     │
     ▼
Raspberry Pi Zero 2 W
     │
 USB / OTG
     │
     ▼
Car head unit
```

The Pi is intended to bridge the phone's wireless Android Auto connection to a head unit that only provides wired Android Auto.

## Present in the snapshot

- C++23/CMake project structure;
- event queue and dispatcher;
- state-machine core;
- Android Auto connection/session state graph;
- service interfaces;
- dummy BLE adapter;
- Linux/BlueZ-oriented drivers;
- application entry point;
- GoogleTest/GoogleMock tests;
- development scripts and static-analysis configuration;
- Docker development/build environments;
- Buildroot/AArch64 integration for Raspberry Pi Zero 2 W.

## Not finished

The snapshot is not an end-to-end wireless Android Auto dongle yet.

In particular:

- Android Auto protocol functionality is incomplete;
- several service implementations are skeletal;
- the application/runtime loop is minimal;
- hardware validation has not been completed;
- the Pi image/deployment path is not yet a production-ready release.

## Known CI build failure

The supplied CI run fails while linking the application. Representative errors include:

```text
undefined reference to `vtable for aa::drivers::linux::LinuxLogger'
undefined reference to `typeinfo for aa::drivers::linux::LinuxProtocol'
undefined reference to `aa::drivers::linux::LinuxBluetooth::LinuxBluetooth()'
```

The important detail is that this is a **link-time dependency problem**, not a compiler failure.

The current project has:

```text
BUILD_LINUX_DRIVERS=OFF
```

for the default host build, while `Session` directly owns `LinuxServices`. As a result, the application/session code can reference Linux driver symbols even though the Linux driver target is disabled.

This architectural issue is intentionally preserved as the final checkpoint. It should be fixed in a later commit by improving dependency injection/ownership rather than silently changing the current snapshot while reconstructing history.

## Docker development issue

The Windows Docker CLI was also observed failing to connect to:

```text
npipe:////./pipe/dockerDesktopLinuxEngine
```

That indicates a Docker Desktop Linux-engine/daemon availability problem. It is independent of the C++ linker failure.
