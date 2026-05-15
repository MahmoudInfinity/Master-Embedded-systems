This project demonstrates a simple GPIO input-output control system using STM32 at the register level (bare-metal).

A push button is used as an input to control an LED:

Button pressed → LED ON
Button released → LED OFF

This is a fundamental embedded systems example for understanding:

GPIO configuration
Input reading
Output control
⚙️ Features
Direct register-level programming (no HAL)
GPIO input with internal pull-up
GPIO output (push-pull)
Real-time input-to-output mapping
🔌 Hardware Configuration
🎛️ Input
Component	Pin	Description
Button	PB8	Input with internal pull-up
💡 Output
Component	Pin	Description
LED	PB11	Output (push-pull)
🧠 System Behavior
Button State	GPIO Input	LED State
Released	HIGH (1)	OFF
Pressed	LOW (0)	ON

👉 The button is connected to GND, so:

Internal pull-up makes default state = HIGH
Pressing the button pulls it to LOW
🔁 Main Logic
if (GPIOB->IDR.bit.B8)
{
    GPIOB->ODR.bit.B11 = 0;  // LED OFF
}
else
{
    GPIOB->ODR.bit.B11 = 1;  // LED ON
}
🧱 Project Structure
/inc
  gpio.h

/src
  gpio.c
  main.c
🛠️ Key Concepts
Memory-mapped registers
Bitfield-based register access
Input configuration (pull-up)
Output configuration (push-pull)
Polling-based input handling
⚠️ Notes
No debouncing is implemented → button may produce noisy signals
Uses polling (no interrupts)
Delay-free → immediate response
🚀 Future Improvements
Add button debouncing
Use EXTI interrupt instead of polling
Replace ODR with BSRR (atomic operations)
Add multiple buttons and LEDs
Convert to reusable GPIO driver