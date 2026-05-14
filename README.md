# ESP32 Gold Price Tracker

A real-time Gold (XAU/USD) price tracker using an ESP32-C3 and a 0.96" I2C OLED display. This project fetches live financial data via API and graphs a rolling sparkline directly on the screen.

## Hardware Requirements
* ESP32-C3 Development Board
* 0.96" I2C OLED Display (SSD1306)
* Jumper wires

## Wiring

| OLED Pin | ESP32-C3 Pin |
| :--- | :--- |
| VCC | 3.3V (or 3V3) |
| GND | GND |
| SCL | GPIO 9 |
| SDA | GPIO 8 |

## Setup Instructions

1. **Clone the repository:**
   ```bash
   git clone https://github.com/petanquex/ESP32-Gold-Price-Tracker.git
   ```

2. **Add your Wi-Fi Credentials:**
   For security, Wi-Fi credentials are intentionally excluded from version control. You must create a local `secret.h` file before compiling.
   
   Create a file named `secret.h` in the same directory as your main code and add the following template:
   ```cpp
   // secret.h
   #pragma once
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```

3. **Install Dependencies:**
   Ensure you have the following libraries installed in your environment:
   * `Adafruit SSD1306`
   * `Adafruit GFX Library`
   * `ArduinoJson` (v7.x or higher)

4. **Compile and Upload:**
   Select the ESP32-C3 board in your IDE and upload. (Note: If using Arduino IDE, ensure "USB CDC On Boot" is enabled to view Serial Monitor output).

## Features
* Fetches real-time XAU/USD prices every 90 seconds.
* Plots a live auto-scaling sparkline graph on the OLED display.
* Secure credential management via isolated header file.
