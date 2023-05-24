## 🕹️ Control Servo and Stepper Using Joystick 🕹️

🤖 Read two Voltage of the Joystick using ADC First for X-axis and second for ‎Y-axis

🤖 Adjust the Voltage reading to the origin of the X-axis and Y-axis. And by ‎mapping volt to coordinates detect the quadrant by volt polarity and angle ‎from inverse tan of axis’s

🤖 Make a lookup table for inverse tan multiplied by the amplification factor ‎looking for inverse tan to get the angle ‎

🤖 Make a dead zone to avoid a false reading.‎

🤖 Use this angle and map it to control PWM to control the Servo motor ‎position angle.‎

🤖 Stepper acts like servo my compute its position from reference and save the ‎last position in EEPROM so it always knows its position after system restart.‎

🤖 The Stepper motor decided to move CCW or CW According to the smallest ‎distance it will cut to reach this is the position angle Taken from Joystick.‎


## 🎮 Implement ‎

✅ Implement MCAL layer (ADC, Dio, timer, EEPROM …)‎.

✅ Implement HAL layer Drivers (Stepper, Joystick, Servo,…).
‎
✅ Control Servo position by PWM.

✅ Implement a Look-up table for Joystick.
 ‎
✅ Track Stepper position by EEPROM.‎

## Related

Here are some related Driver

[Joustick Drive](https://github.com/Mina267/Embedded-systems-projects/blob/master/HAL/Joystick.c)

[Stepper motor Drive](https://github.com/Mina267/Embedded-systems-projects/blob/master/HAL/Stepper.c)

[Servo motor Drive](https://github.com/Mina267/Embedded-systems-projects/blob/master/HAL/Servo.c)

