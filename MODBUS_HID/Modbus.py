import hid
import struct
import time
from Modbus_def import *

def print_bytearray(bytearray):
    hex_str = ' '.join(f'{byte:02x}' for byte in bytearray)
    print(hex_str)

def format_command(command):
    bytearray_command = bytearray()
    for U16 in command:
        bytearray_command.extend([U16 >> 8, U16 & 0xFF])
        # print_bytearray(bytearray_command)
    return bytearray_command

def find_device(vendor_id, product_id):
    for device_dict in hid.enumerate():
        if device_dict['vendor_id'] == vendor_id and device_dict['product_id'] == product_id:
            return device_dict
    return None

def MasterModBus(device, FunCode, pIn, pOut):
    
    print(type(pIn))

    HidBuf = bytearray(HID_PACK_MAX + 1)
    HidBuf[HID_PACK_CH] = REPORT_ID
    HidBuf[HID_PACK_TYPE] = MB_HID_PROTOCOL_ID
    HidBuf[HID_PACK_MODBUS] = FunCode
    
    # Set packet length based on function code
    if FunCode == MBFunctions.MB_FUNC_READ_INPUT_REGISTER or FunCode == MBFunctions.MB_FUNC_READ_HOLDING_REGISTER:
        HidBuf[HID_PACK_LEN] = 7
    elif FunCode == MBFunctions.MB_FUNC_WRITE_MULTIPLE_REGISTERS:
        HidBuf[HID_PACK_LEN] = 7 + (pIn[4] + 1)
        if HidBuf[3] > HID_PACK_MAX:
            return MBErrorCode.MB_ELEN
    else:
        return MBErrorCode.MB_EILLFUNCTION
    
    # print("Write: ", HidBuf)

    # Copy input data to HidBuf
    HidBuf[HID_PACK_MODBUS+1:HID_PACK_MODBUS+1+len(pIn)] = pIn[:]
    HidBuf = HidBuf[:HID_PACK_MAX + 1]

    # Write HID packet
    print("Write: ")
    print_bytearray(HidBuf)
    if not device.write(HidBuf):
        return MBErrorCode.MB_EIO

    # Read response
    time.sleep(TIME_OUT / 1000.0)  # Simple delay for demonstration, consider adjusting for your application
    HidBuf = device.read(HID_PACK_MAX + 1)
    #add ID 
    HidBuf.insert(0, 0x0)
    if not HidBuf:
        return MBErrorCode.MB_ETIMEDOUT
    
    print("Read: ")
    print_bytearray(HidBuf)

    # Validate response length
    if HidBuf[HID_PACK_LEN] > HID_PACK_MAX:
        return MBErrorCode.MB_ELEN
    
    if HidBuf[HID_PACK_MODBUS] == FunCode:
        if HidBuf[HID_PACK_MODBUS] == MBFunctions.MB_FUNC_READ_INPUT_REGISTER or MBFunctions.MB_FUNC_READ_HOLDING_REGISTER: # Modbus function 0x04 Read Input Registers
            if((HidBuf[HID_PACK_LEN] != HidBuf[HID_PACK_MODBUS+1]+4) or (HidBuf[HID_PACK_LEN] & 0x01)):                     # Modbus function 0x03 Read Holding Registers
                return MBErrorCode.MB_ELEN
            else:
                for i in range(0, HidBuf[HID_PACK_MODBUS+1], 2):
                    pOut[i] = HidBuf[HID_PACK_MODBUS + 2 + i + 1]
                    pOut[i + 1] = HidBuf[HID_PACK_MODBUS + 2 + i]
            
        elif HidBuf[HID_PACK_MODBUS] == MBFunctions.MB_FUNC_WRITE_MULTIPLE_REGISTERS:                                       # Modbus function 0x10 Write Multiple Registers
            HidBuf[HID_PACK_LEN] = 0+5+(HidBuf[HID_PACK_MODBUS+5]*2+1)	

    elif HidBuf[HID_PACK_MODBUS] == (FunCode | 0x80):
        return HidBuf[HID_PACK_MODBUS+1] # Return exception code
    else:
        return MBErrorCode.MB_ERETURN
    
    return MBErrorCode.MB_EOK

def MasterRead(ReadType, RegStart, RegCount):
   
    pOut = bytearray()
    FunCode = MBFunctions.MB_FUNC_READ_HOLDING_REGISTER if ReadType == 0 else MBFunctions.MB_FUNC_READ_INPUT_REGISTER
    
    total_bytes = RegCount * 2  # Assuming each register is 2 bytes
    read_bytes = 0

    while RegCount > 0:
        current_read_count = min(RegCount, READ_REG_COUNT_MAX)
        RegCount -= current_read_count
        
        InBuf = bytearray([
            (RegStart >> 8) & 0xFF,  # High byte of RegStart
            RegStart & 0xFF,         # Low byte of RegStart
            0x00,                     # High byte of the register count (always 0 since count is <= READ_REG_COUNT_MAX)
            current_read_count       # Low byte of the register count
        ])
        
        ret = MasterModBus(device, FunCode, InBuf, pOut)
        if ret != MBErrorCode.MB_EOK:
            return ret, pOut[:total_bytes]
        
        # Assuming MasterModBus appends the read data to a global `pOut`
        # You might need to adjust this part depending on how MasterModBus is implemented
        read_bytes += current_read_count * 2
        RegStart += current_read_count

    # At this point, `pOut` should contain the data read
    # Depending on your implementation, you might need to return or process `pOut` further
    return MBErrorCode.MB_EOK, pOut[:total_bytes]

def MasterWrite(RegStart, RegCount, pIn):

    pOut = bytearray()
    # total_bytes_to_write = RegCount * 2  # Assuming 2 bytes per register
    written_bytes = 0  # Counter for the bytes written

    while RegCount > 0:
        current_write_count = min(RegCount, WRITE_REG_COUNT_MAX)
        RegCount -= current_write_count

        # Preparing InBuf with the register start address and the count of registers to write
        InBuf = bytearray([
            (RegStart >> 8) & 0xFF,  # High byte of RegStart
            RegStart & 0xFF,         # Low byte of RegStart
            0x00,                     # High byte of the count (always 0 since count is <= WRITE_REG_COUNT_MAX)
            current_write_count,      # Low byte of the count
            2 * current_write_count   # Number of bytes to write
        ])

        # Add the data to be written, swapping bytes
        for i in range(0, 2 * (current_write_count), 2):
            # print("index", written_bytes + i)
            InBuf.extend([pIn[written_bytes + i], pIn[written_bytes + i + 1]])
            
        # Send the write command
        ret = MasterModBus(device, MBFunctions.MB_FUNC_WRITE_MULTIPLE_REGISTERS, InBuf, pOut)
        if ret != MBErrorCode.MB_EOK:
            return ret

        written_bytes += 2 * current_write_count
        RegStart += current_write_count

    return MBErrorCode.MB_EOK

if __name__ == "__main__":
    device_info = find_device(VENDOR_ID, PRODUCT_ID)
    
    if device_info:
        print("Device found. Opening...")
        device = hid.device()
        device.open(VENDOR_ID, PRODUCT_ID)
        print("Device opened.")

        print("Writing registers...")
        command = [Operation.ONLY_BALANCE, Select_memory.PROGRAM_0, Select_channel.CHANNEL_1, Order_lock.UNLOCK, Order.STOP]
        ret = MasterWrite(CONTROL_REGISTERS, len(command)//2, command)
        print("Writing return code: ", ret)
    
        device.close()
    else:
        print("Device not found.")
        
    print("Goodbye!")
