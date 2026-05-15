# STM32 GPIO Register-Level Programming

This project demonstrates low-level (bare-metal) GPIO programming on STM32 using direct register access without HAL or LL drivers.

## 🚀 Features
- Direct register manipulation
- Bit-field struct mapping
- GPIO configuration without libraries
- Simple LED toggle example

## 🧠 What You Learn
- Memory-mapped I/O
- Register struct design
- Bitfields in embedded C
- STM32 GPIO configuration (CRL / CRH)

## ⚙️ Hardware
- STM32F1 series (e.g., STM32F103)
- LED connected to PB11

## 🔧 How It Works
1. Enable GPIOB clock via RCC
2. Configure PB11 as output push-pull
3. Toggle pin in infinite loop

## 📂 Code Structure
- `main.c`: Full register-level implementation

## 📌 Notes
- No HAL used
- Designed for learning purposes
- Delay is software-based (not accurate)

## 🛠️ Future Improvements
- Replace delay with timer
- Add input handling
- Expand to driver-style architecture

## 👨‍💻 Author
Mahmoud Elmowafy