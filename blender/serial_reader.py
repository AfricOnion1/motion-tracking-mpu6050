import bpy
import serial
import threading
import time

SERIAL_PORT = "COM3"
BAUD_RATE = 115200

roll = 0.0
pitch = 0.0
yaw = 0.0
running = True

def read_serial_data():
    global roll, pitch, yaw, running
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print(f"Connected to {SERIAL_PORT}")
        while running:
            if ser.in_waiting > 0:
                line = ser.readline().decode("utf-8").strip()
                try:
                    data = line.split(",")
                    roll = float(data[0])
                    pitch = float(data[1])
                    yaw = float(data[2])
                except (ValueError, IndexError):
                    pass
    except serial.SerialException as e:
        print(f"Serial error: {e}")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()

serial_thread = threading.Thread(target=read_serial_data)
serial_thread.start()

def update_rotation():
    global roll, pitch, yaw
    obj = bpy.data.objects.get("Empty")
    if obj:
        obj.rotation_euler = (
            pitch * (3.14159265 / 180.0),
            roll * (3.14159265 / 180.0) * -1,
            yaw * (3.14159265 / 180.0)
        )

def update_timer():
    if running:
        update_rotation()
        return 0.1
    return None

bpy.app.timers.register(update_timer)
