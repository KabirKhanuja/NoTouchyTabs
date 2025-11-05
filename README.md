# Ultrasonic Bluetooth Tab Switcher

Trigger a tab change on your laptop by waving your hand near an ultrasonic sensor (HC-SR04). An Arduino reads the distance and sends a `SWITCH` message over Bluetooth (HC-05). A small Python program on the laptop listens on the serial Bluetooth port and simulates a tab-switching hotkey when `SWITCH` is received.

---

## Features
- Wireless: Arduino -> HC-05 -> Laptop (Bluetooth serial)
- Simple debounced proximity trigger (distance < 5 cm)
- Python receiver simulates keypress to change browser/app tab

---

## Hardware
- Arduino UNO (or compatible)
- HC-SR04 ultrasonic sensor
- HC-05 Bluetooth module (or similar serial-BT module)
- Jumper wires
- Breadboard (optional)
- 1 × 2kΩ & 1 × 1kΩ resistors (voltage divider for HC-05 RX)

### Wiring (summary)
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

## Arduino code
Upload `arduino/ultrasonic_bluetooth.ino` from the `arduino/` folder using Arduino IDE.

**Behavior**: measures distance using HC-SR04. When distance &lt; 5 cm, sends `SWITCH` over Bluetooth and over Serial (for debugging). Includes a small debounce delay.

---

## PC Receiver (Python)

**Requirements**:
- Python 3.8+
- `pyserial` (read Bluetooth serial)
- `pyautogui` (simulate keypresses)

See `pc/requirements.txt`.

**How it works**:
1. Pair your HC-05 with your laptop over Bluetooth (OS-specific). Note the serial COM port name (Windows) or `/dev/tty.*` (macOS) or `/dev/rfcomm*` (Linux).
2. Update `PORT` in `pc/tab_switcher.py` or pass it via CLI when running.
3. Run `python tab_switcher.py --port COM5` (replace COM5 with your port).
4. Bring your browser (or target app) into focus; when you wave your hand within 5 cm, the script will simulate the platform-appropriate hotkey to move to the next tab.

**Important**: The script simulates keypresses. The target application must be focused for the shortcut to apply.

---

## Platform notes & shortcuts
- **Windows / Linux**: `ctrl + tab` (next tab)
- **macOS**: `command + option + right` (next tab in many browsers)

These are the keys used by the script. You can change them in `tab_switcher.py`.

---

## Pairing HC-05 (quick)
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

---

## How to push to GitHub (simple)
```bash
git init
git add .
git commit -m "Initial commit: Ultrasonic Bluetooth tab switcher"
# create remote repo on GitHub and then:
git remote add origin <your-repo-url>
git branch -M main
git push -u origin main