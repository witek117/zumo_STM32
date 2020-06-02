import serial
import WS2812
import communication
import mcp9700
import battery

print("ZUMO test program")
print("COM PORT: ")
# com_port = input()
# com_port = "COM" + str(com_port)
com_port = "COM" + str(3)
print("Connecting to " + com_port)


ser = serial.Serial('COM3', 230400, timeout=0.1)

communication.test(ser)

mcp9700.test(ser)

battery.test(ser)

WS2812.test(ser)
