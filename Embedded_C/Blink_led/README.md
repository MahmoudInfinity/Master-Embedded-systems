STM32 GPIO PB11 Toggle Example

This project demonstrates how to toggle an STM32 microcontroller GPIO pin (PB11) using direct register access in C. The code is designed for bare-metal programming without any HAL or CMSIS libraries.

Features:
Direct register manipulation for GPIO control.
Toggles GPIOB pin 11 continuously with a simple delay loop.
Demonstrates basic peripheral clock enabling and pin configuration.
Hardware Requirements
STM32 microcontroller (any STM32 with GPIOB and RCC registers compatible, e.g., STM32F103).
LED or other output device connected to PB11 (optional).
Software Requirements
ARM toolchain (arm-none-eabi-gcc)
QEMU for ARM (optional, for simulation)
Make or any build system to compile the project
Pin Configuration
PB11: Configured as output push-pull at 50 MHz.
Code Overview
#include <stdint.h>

// RCC registers
#define RCC_BASE    0x40021000
#define RCC_APB2ENR (*(volatile uint32_t*)(RCC_BASE + 0x18))

// GPIOB registers
#define GPIOB_BASE  0x40010C00
#define GPIOB_CRH   (*(volatile uint32_t*)(GPIOB_BASE + 0x04))
#define GPIOB_ODR   (*(volatile uint32_t*)(GPIOB_BASE + 0x0C))

int main(void)
{
    // Enable clock for GPIOB
    RCC_APB2ENR |= (1 << 3);

    // Configure PB11 as output push-pull (50 MHz)
    GPIOB_CRH &= ~(0xF << 12);  // clear CNF11 + MODE11
    GPIOB_CRH |=  (0x3 << 12);  // set MODE11 = 50MHz output

    while(1)
    {
        // Toggle PB11
        GPIOB_ODR ^= (1 << 11);

        // Simple delay
        for(volatile int i = 0; i < 1000000; i++);
    }
}




How it Works:

Enable GPIOB Clock: Set bit 3 of RCC_APB2ENR to enable the GPIOB peripheral.

Configure PB11: Clear and set appropriate bits in GPIOB_CRH to set PB11 as output push-pull with 50 MHz speed.

Toggle PB11: Continuously XOR the corresponding bit in GPIOB_ODR to toggle the pin.

Delay: A simple software delay to make toggling visible.


Compilation & Flashing:
Compile the code with the ARM GCC toolchain:

arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O2 -o pb11_toggle.elf pb11_toggle.c

Flash the compiled binary to your STM32 device using your preferred programmer (ST-Link, J-Link, etc.).

Optional: Simulate with QEMU:

qemu-system-arm -M stm32-p103 -kernel pb11_toggle.elf -nographic

Notes:

This project is intended for learning bare-metal GPIO control in STM32.
Adjust the delay loop for your specific clock speed to get visible LED blinking.