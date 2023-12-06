# Firmware
This is an alpha test.

Firmware for the rocket's flight computer (UAV) and its launch control platforms. The code is based on Arduino C++ and the Rocket's firmware is made explicitly for Arduino Nano on the custom flight computer board.

The code is capable of communicate remotely to each platform. The rocket should be able to deploy its parachute remotely and automaticly and report all data to launch control and write it to the SD card module.

Launch pad only turns on the engine after both rocket's firmware and launch control are ready for start. It will have a countback of 60 seconds.

# Software used
VScode used but works fine on Arduino IDE (v2)
**components may need their own libraries to work**

# Components used
- Custom printed board (gerber file)
- Adafruit MPU6050
- Adafruit BMP180
- Adafruit breakout board+ with 2GB MicroSD card
- SX1278 LoRa-02
- Arduino Nano
- 5 gram. servos
- 7.4V 2S LiPo battery with Deans T connector

# PCB images
![pcb](https://github.com/AlmartDev/Rocket/blob/main/Img/pic1.PNG)

![sheet](https://github.com/AlmartDev/Rocket/blob/main/Img/pic2.PNG)
