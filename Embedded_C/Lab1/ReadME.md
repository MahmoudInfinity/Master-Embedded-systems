# Lab1 - Bare-metal ARM Embedded C Project

## Overview
This project contains a simple bare-metal program for the ARM Versatile PB platform. It demonstrates basic GPIO/UART operations using direct register access without any operating system. The program is designed to run on QEMU for testing and STM32/ARM boards for real hardware.

## Features
- UART communication (printf-style output)
- GPIO control (example: LED toggle)
- Minimal startup code using a custom linker script
- Runs on QEMU Versatile PB emulator (`-nographic`)

## Project Structure

Lab1/
├── main.c # Main program code
├── startup.s # ARM startup code
├── Versatilepb.elf # Compiled ELF file for QEMU / ARM
├── README.md # Project documentation
└── linker.ld # Custom linker script

Notes
Ensure arm-none-eabi-gcc and qemu-system-arm are installed.
Audio warnings from QEMU can be ignored when using -nographic.
Modify main.c for custom GPIO/UART experiments.



Author

Mahmoud Elmowafy