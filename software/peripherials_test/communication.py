
def test(ser):
    print("-------------------- TESTING CONNECTION -----------------")
    ser.flush()
    ser.write('test\n'.encode())
    if ser.read(4) == b'1234':
        print("TEST OK")
        print("TEST END")
        return True
    else:
        print("TEST FAILED")
        print("TEST END")
        return False

