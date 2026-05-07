# ARM7TDMI Hardware Cryptography Benchmarker 🔒⚡

![Architecture](https://img.shields.io/badge/Architecture-ARM7TDMI-blue)
![Microcontroller](https://img.shields.io/badge/MCU-NXP_LPC2148-green)
![Language](https://img.shields.io/badge/Language-C_|_ARM_Assembly-orange)
![Environment](https://img.shields.io/badge/Environment-Keil_uVision_ISS-lightgrey)

## Overview
A mixed-language embedded system designed to deploy, execute, and profile the RC4 stream cipher on an LPC2148 microcontroller. This project serves as a rigorous exploration of the **Time-Space Tradeoff** in edge computing environments, comparing the execution speed and memory footprint of a high-level C compiler against hand-written, low-level ARM Assembly.

All execution, peripheral communication, and exact clock-cycle benchmarking were simulated and verified at the instruction-set level using the Keil uVision ARM Instruction Set Simulator (ISS).

## System Architecture
The system utilizes a dual-layer architecture, bridging high-level control logic with low-level computational engines via the ARM Application Binary Interface (ABI).

* **The Control Layer (C):** Manages asynchronous hardware state, interactive UART serial communication, and controls the internal 32-bit Vectored Hardware Timer (Timer0) for nanosecond-accurate benchmarking.
* **The Execution Layer (Assembly):** A hand-written, register-optimized implementation of the RC4 cryptographic algorithm, designed to minimize ROM footprint by aggressively managing the CPU pipeline and utilizing `R4-R11` for volatile state retention.

## Performance Profiling & Results
The system was benchmarked using Hardware Timer0 (prescaler = 0) to capture exact CPU clock cycles, and profiled using the Keil Linker `.map` file to calculate the physical silicon footprint. 

The results demonstrate the classic Time vs. Space tradeoff in firmware engineering:

| Metric | Keil C Compiler (-O3) | Hand-Written ARM Assembly | Total System Overhead |
| :--- | :--- | :--- | :--- |
| **Execution Speed** | 4,711 clock cycles | 6,797 clock cycles | N/A |
| **Algorithm ROM Footprint** | 332 Bytes | ~120 Bytes | **2.62 kB** (Total Flash used) |
| **Algorithm RAM Footprint** | Dynamic (Stack-heavy) | Static (Register-heavy) | **1.23 kB** (Total RAM used) |

### Key Findings
1. **Speed Superiority (C Compiler):** Utilizing `-O3` optimization, the compiler leveraged aggressive loop unrolling and advanced pipeline scheduling to outpace human assembly by ~30%.
2. **Footprint Superiority (Assembly):** The compiler's speed came at the cost of ROM bloat (332 bytes). The hand-written assembly sacrificed clock cycles but dramatically reduced the physical instruction footprint by ~60% (~120 bytes).
3. **Conclusion:** For edge-computing applications where execution speed is the primary constraint, C compilation is optimal. However, in severely Flash-constrained microcontrollers, manual assembly architecture remains superior.


## File Structure
```text
├── main.c       # System orchestrator, ABI linking, and benchmarking logic
├── rc4.s        # Low-level ARM Assembly cryptographic engine
├── uart.c/h     # UART0 hardware peripheral drivers (Serial I/O)
├── timer.c/h    # Timer0 hardware peripheral drivers (Cycle counting)
└── Startup.s    # LPC2148 Bootloader and stack initialization
