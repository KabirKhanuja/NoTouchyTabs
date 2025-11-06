# NoTouchyTabs

**NoTouchTabs** is an Arduino-based system that automatically switches your computer’s tab or application window when a door opens.  
It helps users quickly hide or change their on-screen content when someone (like a parent) unexpectedly enters the room.

The system uses an ultrasonic sensor positioned near the door to detect movement.  
When triggered, the Arduino sends a signal to a Python script on the laptop (via Bluetooth or USB), which simulates a keypress such as **Ctrl + Tab** or **Alt + Tab** to instantly switch tabs or applications.

---

## Overview

The project supports two modes:
1. **Wired Mode** – communicates via USB connection (uses `ultrasonic_wired.ino`)  
2. **Wireless Mode** – communicates via Bluetooth using HC-05 module (uses `ultrasonic_bluetooth.ino`)  

In both cases, the Arduino continuously monitors the door’s movement using the HC-SR04 ultrasonic sensor and sends a trigger signal when the door opens.

---

## Features

- Automatic tab or window switch when door opens  
- Works via **Bluetooth (HC-05)** or **USB**  
- Adjustable distance sensitivity  
- Configurable key actions: next tab, app switch, or any custom shortcut  
- Compatible with Windows, macOS, and Linux  

---

## Hardware Requirements

- Arduino UNO (or compatible board)  
- HC-SR04 Ultrasonic Sensor  
- (Optional) HC-05 Bluetooth Module for wireless communication  
- Jumper Wires  
- Breadboard (optional)  
- 2 kΩ + 1 kΩ resistors (for voltage divider if using Bluetooth)

---

### Wiring 
**HC-SR04**
- VCC -> 5V
- GND -> GND
- TRIG -> Arduino D2
- ECHO -> Arduino D3

**HC-05**
- VCC -> 5V
- GND -> GND
- TXD -> Arduino D10 (SoftwareSerial RX)
- RXD -> Arduino D11 (SoftwareSerial TX) **(use voltage divider)**

**Voltage divider for HC-05 RX** (make RX tolerant to 3.3V):
- Arduino TX (D11) -> 2kΩ -> node -> 1kΩ -> GND
- node -> HC-05 RX

> Note: If you have a logic level converter, prefer that. Do NOT apply 5V directly to HC-05 RX.

---

## Arduino Code Description

### `ultrasonic_wired.ino`
This file is used for **wired communication** through USB.  
It measures the distance using the HC-SR04 ultrasonic sensor. When an object (door) comes within a defined threshold (for example, less than 5 cm), the Arduino sends the keyword **"SWITCH"** to the computer through the serial port.  
The Python program on the computer listens for this message and performs the tab-switch action.

Key logic inside this code:
- Defines pins for TRIG (2) and ECHO (3).  
- Uses ultrasonic timing (`pulseIn`) to calculate distance.  
- Prints `"SWITCH"` to serial output when the distance is below threshold.  
- Includes a small delay to avoid repeated triggers.

### `ultrasonic_bluetooth.ino`
This file is used for **wireless communication** with the HC-05 module.  
It performs the same logic as the wired version but sends `"SWITCH"` via Bluetooth instead of USB serial.

---

## Python Receiver

### Requirements
- Python 3.8+  
- `pyserial` – for reading serial/Bluetooth data  
- `pyautogui` – for simulating keyboard shortcuts 
See `pc/requirements.txt`.

**Important**: The script simulates keypresses. The target application must be focused for the shortcut to apply.

---

## How to Run

### Step 1: Upload Arduino Code
- Open Arduino IDE
- Go to `File > Open > arduino/ultrasonic_bluetooth.ino`
- Load either:
	•	arduino/ultrasonic_wired.ino for USB mode
	•	arduino/ultrasonic_bluetooth.ino for Bluetooth mode
- Select your board & COM port, then click **Upload**

### Step 2: Connect to Laptop
- Power the Arduino setup
- For wireless mode, pair the HC-05 Bluetooth module with your laptop (PIN 1234 or 0000). Pair your HC-05 with laptop
- For wired mode, connect Arduino via USB
- Note the COM port (COM5 or /dev/tty.*)

### Step 3: Run the Python Listener
Run one of the following commands depending on the desired behavior:
```bash
cd pc
pip install -r requirements.txt
# to switch to next browser tab
python tab_switcher.py --port COM5 --action next_tab
# to switch between applications
python tab_switcher.py --port COM5 --action app_switch
# to use a custom key combination
python tab_switcher.py --port COM5 --action custom --keys "ctrl+shift+tab"
```
Replace COM5 with your actual port.
Add --debug to view incoming serial data.

---

## Platform notes & shortcuts
- **Windows / Linux**: `ctrl + tab` (next tab), `alt + tab` (app switch)
- **macOS**: `command + option + right` (next tab in many browsers), `cmd + tab` (app tab)

Shortcuts can be customized in `tab_switcher.py`.


---

## Pairing HC-05 
- Put HC-05 in discoverable mode (power it up). Default PIN is usually `1234` or `0000`.
- Windows: Settings -> Bluetooth & devices -> Add device. After pairing, check Device Manager -> Ports (COM & LPT) for the COM port.
- macOS: System Settings -> Bluetooth. After pairing, you may need to determine the `/dev/tty.*` device from `/dev` or `Bluetooth Explorer` (part of Additional Tools), or use `ls /dev/tty.*` before/after pairing.
- Linux: pair using the desktop Bluetooth manager or `bluetoothctl`, then bind with `rfcomm` if needed.

---

## Security & permissions
- On **macOS** you must grant Accessibility permission to the terminal / Python app to allow pyautogui to send keypresses.
- Be careful: the script will send keypresses to the active window — avoid running while typing sensitive information.

---

## Troubleshooting
- No `/dev` or COM port: ensure pairing is complete and use Device Manager / `ls /dev` to find port.
- `pyautogui` fails to send keys: grant accessibility/automation permissions (macOS) or run with proper privileges.
- Wrong COM port: double-check ports before running.

