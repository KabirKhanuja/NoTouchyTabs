import argparse, sys, time, platform
try:
    import serial
except Exception:
    print("pyserial is required, install via pip install pyserial")
    raise
try:
    import pyautogui
except Exception:
    print("pyautogui is required, install via pip install pyautogui")
    raise

parser = argparse.ArgumentParser()
parser.add_argument("--port", required=True, help="Serial port)")
parser.add_argument("--baud", type=int, default=9600)
parser.add_argument("--debug", action="store_true")
parser.add_argument("--action", choices=["next_tab", "app_switch", "custom"], default="next_tab",
                    help="Action to perform on SWITCH message (default: next_tab)")
parser.add_argument("--keys", default="", help="For --action custom: comma/plus-separated keys, eg ctrl+shift+tab or command,option,right")
args = parser.parse_args()

plat = platform.system().lower()

def do_next_tab():
    # next browser tab
    if 'darwin' in plat or 'mac' in plat:
        # works on Chrome/Firefox on macOS
        pyautogui.hotkey('command', 'option', 'right')
    else:
        pyautogui.hotkey('ctrl', 'tab')

def do_app_switch():
    # app switcher 
    if 'darwin' in plat or 'mac' in plat:
        pyautogui.hotkey('command', 'tab')
    else:
        pyautogui.hotkey('alt', 'tab')

def do_custom(keys_string):
    """
    Accepts strings like:
      "ctrl+shift+tab"
      "command,option,right"
      "ctrl+alt+right"
    Separators: + or , or space
    """
    if not keys_string:
        print("No custom keys provided.")
        return
    # normalize
    # split by + or , or whitespace
    import re
    keys = [k.strip() for k in re.split(r'[+,\\s]+', keys_string) if k.strip()]
    if not keys:
        print("No keys parsed from --keys.")
        return
    pyautogui.hotkey(*keys)

# map action to function
def perform_action(action, keys=""):
    if action == "next_tab":
        do_next_tab()
    elif action == "app_switch":
        do_app_switch()
    elif action == "custom":
        do_custom(keys)
    else:
        do_next_tab()

def main():
    print(f"Opening serial port {args.port} at {args.baud} baud...")
    try:
        ser = serial.Serial(args.port, args.baud, timeout=1)
    except Exception as e:
        print("Failed to open serial port:", e)
        sys.exit(1)

    print("Listening for 'SWITCH' from Arduino. Focus the target window now.")
    if args.action == "custom":
        print("Custom keys:", args.keys)

    try:
        while True:
            try:
                if ser.in_waiting:
                    line = ser.readline().decode(errors='ignore').strip()
                    if args.debug and line:
                        print("RX:", line)
                    if line == "SWITCH":
                        print("Trigger received — performing action:", args.action)
                        time.sleep(0.05)
                        perform_action(args.action, args.keys)
                else:
                    time.sleep(0.05)
            except KeyboardInterrupt:
                print("Exiting (Ctrl+C)")
                break
            except Exception as e:
                print("Serial read error:", e)
                time.sleep(0.5)
    finally:
        ser.close()

if __name__ == "__main__":
    main()