import ctypes

CModbus = ctypes.CDLL(r"C:\Users\APrap\Documents\Rigi\Software\iCharger\modbus\CModbus.so")

CModbus.MasterWrite(0x0000, 0x0001, b"\x00\x01\x00\x02")

CModbus.MasterRead(0x0000, 0x0001)


# display = CModbus.display
# display.argtypes = [ctypes.c_char_p, ctypes.c_int]
# display.restype = ctypes.c_int

# display(b"stephane", 18)
