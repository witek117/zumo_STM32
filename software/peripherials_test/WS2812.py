
import time


def test(ser):
    print("-------------------- TESTING WS2812B --------------------")
    print("1. ALL OFF")
    ser.write('ws 0 0 0 0\nws 1 0 0 0\n'.encode())
    time.sleep(1)
    print("2. ALL ON")
    ser.write('ws 0 100 100 100\nws 1 100 100 100\n'.encode())
    time.sleep(1)
    print("3. GREEN")
    ser.write('ws 0 255 0 0\nws 1 255 0 0\n'.encode())
    time.sleep(1)
    print("4. RED")
    ser.write('ws 0 0 255 0\nws 1 0 255 0\n'.encode())
    time.sleep(1)
    print("5. BLUE")
    ser.write('ws 0 0 0 255\nws 1 0 0 255\n'.encode())
    time.sleep(1)
    print("6. ALL OFF")
    ser.write('ws 0 0 0 0\nws 1 0 0 0\n'.encode())
    print("TEST END")
    return True

