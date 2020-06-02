def get_value(ser):
    ser.write('bt?\n'.encode())

    ret = ser.read_until('\n').decode()

    while ret[0] != 't':
        ret = ret[1:]

    ret = ret[2:]
    return float(ret)


def test(ser):
    battery_value = get_value(ser)
    print("-------------------- TESTING BATTERY --------------------")

    print("Battery is " + str(battery_value) + "V")
    if battery_value < 2.0:
        print("Battery level is too low or battery is not connected")

    return True
