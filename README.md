# Firmware
Firmware for the rocket's flight computer (UAV) and its launch control platforms. The code is based on Arduino C++ and the Rocket's firmware is made explicitly for Arduino Nano on the custom flight computer board.

The code is capable of communicate remotely to each platform. The rocket should be able to deploy its parachute remotely and automaticly and report all data to launch control and write it to the SD card module.

Launch pad only turns on the engine after both rocket's firmware and launch control are ready for start. It will have a countback of 60 seconds.