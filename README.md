# Medi__Box

## Overview
The Medi Box project is an ESP32-based smart medicine reminder system developed using Wokwi. It features an OLED display, buzzer, LEDs, buttons, and a DHT22 sensor to monitor temperature and humidity. The system connects to WiFi and synchronizes time using an NTP server to trigger alarms for medicine reminders.

## Features
- **Time Synchronization:** Retrieves real-time clock data via an NTP server.
- **Alarm System:** Two configurable alarms for medicine reminders.
- **Buzzer Alerts:** Sound notifications when it's time to take medicine.
- **LED Indicators:** LED_1 and LED_2 provide visual status for alarms and environmental conditions.
- **Temperature & Humidity Monitoring:** Uses a DHT22 sensor to check environmental conditions and alerts if values exceed defined thresholds.
- **User Interface:** OLED display and buttons (UP, DOWN, OK, CANCEL) for navigating menus and setting alarms.
- **WiFi Connectivity:** Connects to WiFi for real-time clock updates.

## Components Used
- ESP32
- OLED Display (SSD1306)
- DHT22 Temperature & Humidity Sensor
- Buzzer
- LEDs
- Push Buttons

## Pin Configuration
| Component | Pin |
|-----------|-----|
| Buzzer    | 18  |
| LED_1     | 15  |
| LED_2     | 2   |
| CANCEL    | 34  |
| UP        | 35  |
| DOWN      | 32  |
| OK        | 33  |
| DHT22     | 12  |

## How to Use
1. Power on the ESP32 and connect to WiFi.
2. The system will synchronize time via NTP.
3. Navigate the menu using UP and DOWN buttons.
4. Set alarms for medicine reminders.
5. The buzzer and LED_1 will activate when an alarm is triggered.
6. Press CANCEL to stop the alarm or OK to snooze it.
7. Monitor temperature and humidity status(High/Low) on the display.

## Installation & Setup
1. Load the code into the ESP32 using Arduino IDE.
2. Ensure required libraries are installed:
   - `Adafruit_GFX.h`
   - `Adafruit_SSD1306.h`
   - `WiFi.h`
   - `DHTesp.h`
3. Connect the ESP32 to Wokwi for simulation or real hardware.

## Future Enhancements
- Add cloud integration for remote monitoring.
- Implement voice alerts.
- Extend alarm storage to multiple reminders.

## License
This project is open-source and can be modified or redistributed under the MIT License.
