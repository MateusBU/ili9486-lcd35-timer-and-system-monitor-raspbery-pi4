# System Monitor — Raspberry Pi / Linux

A lightweight C library that reads real-time system information from Linux kernel interfaces (`/proc` and `/sys`). Designed for embedded Linux boards such as the Raspberry Pi, but compatible with any standard Linux system.

---

## Table of Contents

- [Overview](#overview)
- [Project Structure](#project-structure)
- [How It Works](#how-it-works)
  - [RAM Usage](#ram-usage)
  - [CPU Temperature](#cpu-temperature)
  - [IP Address](#ip-address)
- [Build & Run](#build--run)
- [API Reference](#api-reference)
- [Future Plans — ILI9486 Display Integration](#future-plans--ili9486-display-integration)

---

## Overview

This project exposes three system metrics through a clean C API:

| Metric | Source |
|---|---|
| RAM usage (used / total) | `/proc/meminfo` |
| CPU temperature | `/sys/class/thermal/thermal_zone0/temp` |
| Local IP address | `hostname -I` (shell command) |

---

## Project Structure

```
.
├── main.c            # Entry point — demonstrates the API
├── systemMonitor.c   # Implementation of all monitoring functions
└── systemMonitor.h   # Public API declarations
```

### `systemMonitor.h`

Declares the three public functions and includes `<stdio.h>` and `<stdint.h>`. Any file that needs to query system metrics only needs to include this header.

### `systemMonitor.c`

Contains the full implementation. Key constants defined internally:

```c
#define dPATH_RAM_USAGE    "/proc/meminfo"
#define dPATH_TEMPERATURE  "/sys/class/thermal/thermal_zone0/temp"
#define dPATH_IP           "hostname -I 2>/dev/null | awk '{print $1}'"
```

### `main.c`

A minimal usage example — calls each function and prints the results to stdout, then enters an infinite loop (placeholder for a future event/display loop).

---

## How It Works

### RAM Usage

**Function:** `systemMonitor_getRamUsage(float *usedMB, float *totalMB)`

Reads `/proc/meminfo` line by line using `fscanf`. It extracts five fields:

| Field | Meaning |
|---|---|
| `MemTotal` | Total physical RAM |
| `MemFree` | Completely unused RAM |
| `Buffers` | Kernel I/O buffers |
| `Cached` | Page cache |
| `SReclaimable` | Reclaimable slab memory |

**Used memory** is calculated as:

```
used = MemTotal - MemFree - Buffers - Cached - SReclaimable
```

This formula matches what tools like `htop` and `free -m` report, because it excludes memory that the kernel will reclaim on demand.

Both output values are converted from kilobytes to **megabytes** before being written to the output pointers.

> **Note:** The source includes an alternative implementation using `fgets` + `sscanf` (commented out), which is slightly more robust for malformed lines.

---

### CPU Temperature

**Function:** `systemMonitor_getTemperature(void)` → `int16_t`

Reads a single integer from `/sys/class/thermal/thermal_zone0/temp`. The kernel exposes temperature in **millidegrees Celsius**, so the raw value is divided by 1000 before being returned.

Returns `-100` if the file cannot be opened (sensor unavailable or wrong path).

> On Raspberry Pi, `thermal_zone0` corresponds to the SoC temperature. Other boards may expose additional zones.

---

### IP Address

**Function:** `systemMonitor_getIP(char *buffer, int bufsz)`

Uses `popen()` to execute:

```sh
hostname -I 2>/dev/null | awk '{print $1}'
```

This returns the first IPv4 address assigned to the machine. The trailing newline is stripped before the result is written to the caller's buffer. If the command fails or returns an empty string, the buffer is set to `"N/A"`.

> `popen` spawns a shell process, which is fine for display/monitoring purposes but should not be called in tight loops.

---

## Build & Run

### Prerequisites

- GCC or any C99-compatible compiler
- Linux system with `/proc/meminfo` and `/sys/class/thermal/` available (Raspberry Pi, Ubuntu, Debian, etc.)

### Compile

```bash
gcc main.c systemMonitor.c -o systemMonitor
```

### Run

```bash
./systemMonitor
```

**Expected output:**

```
USED: 412.30 MB, TOTAL: 3841.00 MB
The temperature is 47 C.
IP: 192.168.1.105
```

---

## API Reference

```c
// Returns used and total RAM in megabytes via output pointers.
// Sets both to 0.0 if /proc/meminfo cannot be opened.
void systemMonitor_getRamUsage(float *usedMB, float *totalMB);

// Returns the SoC temperature in degrees Celsius.
// Returns -100 on error (sensor not found).
int16_t systemMonitor_getTemperature(void);

// Writes the primary local IP address into buffer (max bufsz bytes).
// Writes "N/A" if the address cannot be determined.
void systemMonitor_getIP(char *buffer, int bufsz);
```

---

## Future Plans — ILI9486 Display Integration

The next phase of this project is to drive an **ILI9486 TFT LCD** (typically 3.5", 480×320 px, SPI interface) connected to the Raspberry Pi GPIO header.

Planned features:

- **Live dashboard** — RAM bar, temperature gauge, and IP address rendered directly on screen, refreshed periodically
- **Analog clock widget** — a drawn clock face using sin/cos math, with smooth second-hand animation
- **Digital clock** — large digit rendering using a custom font or bitmap glyphs
- **Display driver layer** — a thin abstraction over the ILI9486 SPI protocol (init sequence, pixel/rect drawing, text rendering)
- **Render loop** — a timed loop (or POSIX timer) that calls `systemMonitor_*` functions and pushes updated frames to the display

The `systemMonitor` library will remain unchanged — the display layer will consume its API directly.