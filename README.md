# DIY VR Headset Using STM32 & Raspberry Pi

This is a personal project to build a working virtual reality headset from scratch using:
- **STM32F411 "Black Pill"** for IMU data processing
- **Raspberry Pi** for display rendering and user interface
- **Custom PCB and lenses**
- Written in **C (STM32)** and **Python/Bash (Raspberry Pi)**

## Folder Structure

- `stm32-firmware/`: HAL-based STM32 firmware for IMU and UART comms
- `raspberry-pi/`: Python/Bash scripts to display visuals, read UART, and run diagnostics
- `hardware/`: Wiring diagrams, schematics, BOM
