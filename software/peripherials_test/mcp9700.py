def turn_on(ser):
    ser.write('t 1\n'.encode())


def turn_off(ser):
    ser.write('t 0\n'.encode())


def get_value(ser):
    ser.write('t?\n'.encode())
    ret = ser.read_until('\n').decode()

    while ret[0] != 't':
        ret = ret[1:]

    ret = ret[2:]
    return float(ret)


def test(ser):
    print("-------------------- TESTING MCP9700 --------------------")
    turn_off(ser)

    if get_value(ser) != 0.0:
        print("Wrong value 1")
        return False

    print("Off TEST OK")

    turn_on(ser)
    if get_value(ser) == 0.0:
        print("Wrong value 2")
        return False

    print("On TEST OK, temperature is " + str(get_value(ser)))
    print("TEST END")

