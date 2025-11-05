import argparse
import sys
import time
import platform

try:
    import serial
except Exception as e:
    print("pyserial is required. Install with: pip install pyserial")
    raise

try:
    import pyautogui
except Exception as e:
    print("pyautogui is required. Install with: pip install pyautogui")
    raise

# you can change the port or pass it via --port
PORT = None
BAUD = 9600

parser = argparse.ArgumentParser()
parser.add_argument("--port", help="Serial port for HC-05 (e.g. COM5 or /dev/tty.HC-05-DevB)")
parser.add_argument("--baud", type=int, default=BAUD, help="Baud rate (default 9600)")
parser.add_argument("--debug", action="store_true")
args = parser.parse_args()

if args.port:
    PORT = args.port
BAUD = args.baud

if not PORT:
    print("error: serial port not provided, use --port COM5 or set PORT in the file")
    sys.exit(1)

# hotkey function
plat = platform.system().lower()
if 'darwin' in plat or 'mac' in plat:
    def next_tab():
        pyautogui.hotkey('command', 'option', 'right')
elif 'windows' in plat or 'linux' in plat:
    def next_tab():
        pyautogui.hotkey('ctrl', 'tab')
else:
    def next_tab():
        pyautogui.hotkey('ctrl', 'tab')


def main():
    print(f"Opening serial port {PORT} at {BAUD} baud...")
    try:
        ser = serial.Serial(PORT, BAUD, timeout=1)
    except Exception as e:
        print("Failed to open serial port:" , e)
        sys.exit(1)

    print("Listening for 'SWITCH' from Arduino. Focus the target window now.")

    try:
        while True:
            try:
                if ser.in_waiting:
                    line = ser.readline().decode(errors='ignore').strip()
                    if args.debug and line:
                        print('RX:', line)
                    if line == 'SWITCH':
                        print('Trigger received — switching tab')
                        time.sleep(0.05)
                        next_tab()
                else:
                    time.sleep(0.05)
            except KeyboardInterrupt:
                print('Exiting (Ctrl+C)')
                break
            except Exception as e:
                print('Serial read error:', e)
                time.sleep(0.5)
    finally:
        ser.close()

if __name__ == '__main__':
    main()