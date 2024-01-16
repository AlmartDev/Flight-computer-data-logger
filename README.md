# Rocket Flight Computer Firmware

This repository contains the firmware for the rocket's flight computer (UAV) and its launch control platforms. The code is written in Arduino C++ and is specifically tailored for the Arduino Nano on a custom flight computer board.

> [!NOTE]
> For newer code please take a look into ```tests``` branch. Code might not work or not be organized.

## Features
- **Remote Communication:** The firmware enables remote communication with each platform.
- **Parachute Deployment:** Capable of deploying the parachute remotely and automatically.
- **Data Reporting:** Reports all data to the launch control and writes it to the SD card module.

The launch pad initiates engine activation only when both the rocket's firmware and launch control are ready for launch. A countdown of 60 seconds ensures a systematic start.

## Software Environment
The code is developed using VSCode, but it is compatible with any IDE you want. Ensure that the required components have their own libraries to function correctly.

## How to Use
1. Install all required libraries listed in `requirements.txt`.
2. Upload the code to the respective rocket, launch pad, or launch control. (if needed)
3. Verify functionality by performing component tests initially.

## Components Used
- Custom printed board (see below)
- Adafruit MPU6050
- Adafruit BMP180
- Adafruit breakout board+ with 8GB MicroSD card (FAT32 format)
- SX1278 LoRa-02 with antenna using the 433 MHz frequency
- Arduino Nano
- 5 gram. servos
- 7.4V 2S LiPo battery with Deans T connector

## PCB Design
The PCB design is available at [this link](https://oshwlab.com/alonsomartinezcarratala/rocket). But you can download the gerber file from the repository.

## It is Open Source!

Feel free to contribute and make improvements to the firmware for a more successful rocket launch!
