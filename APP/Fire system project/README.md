## 🔥Fire System🔥
✨ Measure temperature through the sensor, and filter with a first-degree filter. ‎When temperature exceeds turn from Fine state to Heat state. And yellow ‎LED turn on, send a Warring message through UART, and starts taking ‎reading from the smoke sensor (Take Read Every 1 sec.)‎

✨ If Smoke increases, fire State Start (turning on the red LED, Siren sound, ‎Pump, and smoke fan and sending an Alarm message through UART).‎

✨ Sensors read and system states are shown on the LCD screen.‎

✨ The system can be restored only by a password, that can be written ‎otherwise by Keypad or received from UART if the password writes wrong ‎five times user is blocked for will. ‎

✨ Password, temp level, and smoke percentage are CFG and can be changed ‎by the user From CFG Menu and saved in EEPROM.‎

✨ Pump and Smoke fan speed control by reading off the temp sensor and ‎smoke sensor respectively.‎

##  👷Implementations:‎
‎✅ Implement MCAL layer (a timer, ADC, DIO, EEPROM, EX interrupt, ‎UART …)‎

✅ Implement HAL layer Drivers (Motor, LCD, Keypad, Sensors,…..)

‎✅ Implement a service layer for timer and UART.‎

‎✅ Service of UART Sends and Receives string and frame through UART ‎‎(using Queues and Stacks).‎

‎✅ Implement a Temperature filter for the temp sensor.‎

‎✅ Control Motor Speed by PWM

✅ Make a user-friendly interface with LCD and Keypad.‎

‎✅ Make a Schedule for Tasks.‎

‎✅ External Interrupt for showing the Cfg menu. ‎
