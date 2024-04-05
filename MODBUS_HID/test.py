import Modbus

if __name__ == '__main__':
    device = Modbus.HIDDevice()
    device.open()

    Modbus.MasterModBus(device, Modbus.MBFunctions.MB_FUNC_READ_HOLDING_REGISTER, pIn, pOut)
    print(pOut)
    device.close()