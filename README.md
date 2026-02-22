# Light-Reactive Servo Controller

This project uses an Arduino to control a servo motor's position based on the ambient light level. A photoresistor acts as the light sensor, and a 16x2 LCD provides a real-time display of the sensor data and servo angle.

The code is designed to be smooth and efficient, featuring an exponential moving average (EMA) filter to reduce sensor noise and a power-saving feature that detaches the servo motor when it's idle to prevent buzzing.

## Demo Video

*A video of the project in action!*

![IMG_6887](https://github.com/user-attachments/assets/605581e3-3e19-46f8-82ee-ec08bfbc1e64)


https://github.com/user-attachments/assets/a500732a-1026-4e8d-a3b3-77a927c325a5



## Hardware List

The following components are required to build this project:

- **Microcontroller:** 1x Arduino Uno
- **Actuator:** 1x Servo Motor (e.g., SG90 or similar)
- **Sensor:** 1x Photoresistor (LDR)
- **Display:** 1x 16x2 Parallel LCD (HD44780 compatible)
- **Resistors:**
    - 1x 10kΩ (for the photoresistor voltage divider)
    - 1x 220Ω (for the LCD backlight)
- **Potentiometer:** 1x 10kΩ (for LCD contrast adjustment)
- **Prototyping:**
    - 1x Breadboard
    - Jumper Wires

## Setup & Operation

1.  Assemble the hardware according to the circuit diagram (not included) or the pin definitions in the source code.
2.  Use PlatformIO in an editor like VS Code to upload the firmware (`src/main.cpp`) to the Arduino Uno.
3.  Power on the circuit. Adjust the 10kΩ potentiometer until the text on the LCD is clear and readable.
4.  Shine a light on the photoresistor or cover it to see the servo motor react to the changing light levels. The LCD will display the raw light value and the servo's current angle in degrees.
