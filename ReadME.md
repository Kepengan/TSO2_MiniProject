# TSO2_MiniProject

**ESP32 Sensor Data Logger Spreadsheet Link:** 
[https://docs.google.com/spreadsheets/d/1ecLzY7PB0DUF-ZrQSnoN37upfJeIOG9RTQaefqciR4A/edit?usp=sharing](https://docs.google.com/spreadsheets/d/1-NZUAoeyVhlIQBim3DVndZz6Xn0yQPj-s3ySU_oPM9M/edit?usp=sharing)

**ESP32 Sensor Data Logger Looker Studio Link:** 
[https://lookerstudio.google.com/reporting/5129ab63-2056-4b79-a0d1-d588c0209ff2](https://lookerstudio.google.com/reporting/a56d57b7-3a51-40a8-ac58-d0d798f5df37)

**Components Required**

1. ESP32 
2. BME280 Temperature, Humidity, and Pressure Sensor.
3. HC-SR04 Ultrasonic Distance Sensor.
4. Female to Female Jumper Wires. Micro
5. USB Cable.

**Sensors Connection Table**

| BME280 Pin | ESP32 Pin | Description  |
| ---------- | --------- | ------------ |
| VIN        | 3.3V      | Power supply |
| GND        | GND       | Ground       |
| SDA        | GPIO 21   | I2C Data     |
| SCL        | GPIO 22   | I2C Clock    |

| HC-SR04 Pin | ESP32 Pin | Description    |
| ----------- | --------- | -------------- |
| VCC         | 5V        | Power supply   |
| GND         | GND       | Ground         |
| TRIG        | GPIO 13   | Trigger signal |
| ECHO        | GPIO 12   | Echo signal    |

**Assembly Steps**

1. Connect the BME280 sensor to the ESP32 using jumper wires following the wiring table above.
2. Connect the HC-SR04 ultrasonic sensor to the ESP32 using jumper wires following the wiring table above.
3. Double check VCC and GND connections to avoid short circuits.
4. Connect the ESP32 to your computer using a USB cable.

**Verification**

1. Upload the ESP32 firmware.
2. Open the Serial Monitor at the correct baud rate.
3. Verify that temperature, humidity, pressure, and distance values are printed correctly.
4. Ensure sensor readings are stable.
