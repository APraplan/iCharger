VENDOR_ID = 0x0483
PRODUCT_ID = 0x5751

MB_HID_PROTOCOL_ID = 0x30

HID_PACK_MAX = 64
HID_PACK_CH = 0

HID_PACK_LEN = 1
HID_PACK_TYPE = 2
HID_PACK_MODBUS = 3
REPORT_ID = 0

READ_REG_COUNT_MAX = ((HID_PACK_MAX-4)/2) #//30
WRITE_REG_COUNT_MAX = ((HID_PACK_MAX-8)/2) #//28
TIME_OUT = 500

READ_INPUT = 1 # 0x0000—0x00f - 0x0100—0x01ff 
READ_HOLDING = 0 # 0x8800—0xbfff

class MBErrorCode:
    MB_EOK = 0x00                        # No error 
    MB_EX_ILLEGAL_FUNCTION = 0x01
    MB_EX_ILLEGAL_DATA_ADDRESS = 0x02
    MB_EX_ILLEGAL_DATA_VALUE = 0x03
    MB_EX_SLAVE_DEVICE_FAILURE = 0x04
    MB_EX_ACKNOWLEDGE = 0x05
    MB_EX_SLAVE_BUSY = 0x06
    MB_EX_MEMORY_PARITY_ERROR = 0x08
    MB_EX_GATEWAY_PATH_FAILED = 0x0A
    MB_EX_GATEWAY_TGT_FAILED = 0x0B
    MB_ENOREG = 0x80                   # illegal register address.
    MB_EILLFUNCTION = 0x81			   # illegal function code.
    MB_EIO = 0x82                      # I/O error.
    MB_ERETURN = 0x83			 	   # protocol stack in illegal state.
    MB_ELEN = 0x84					   # pack len larg error.
    MB_ETIMEDOUT = 0x85                # timeout error occurred.
    
class MBFunctions: # iCharge X only implements the Modbus 0x03, 0x04, 0x10 function code
    # MB_FUNC_NONE = 0
    # MB_FUNC_READ_COILS = 1
    # MB_FUNC_READ_DISCRETE_INPUTS = 2
    # MB_FUNC_WRITE_SINGLE_COIL = 5
    # MB_FUNC_WRITE_MULTIPLE_COILS = 15
    MB_FUNC_READ_HOLDING_REGISTER = 3
    MB_FUNC_READ_INPUT_REGISTER = 4
    # MB_FUNC_WRITE_REGISTER = 6
    MB_FUNC_WRITE_MULTIPLE_REGISTERS = 16
    # MB_FUNC_READWRITE_MULTIPLE_REGISTERS = 23
    # MB_FUNC_DIAG_READ_EXCEPTION = 7
    # MB_FUNC_DIAG_DIAGNOSTIC = 8
    # MB_FUNC_DIAG_GET_COM_EVENT_CNT = 11
    # MB_FUNC_DIAG_GET_COM_EVENT_LOG = 12
    # MB_FUNC_OTHER_REPORT_SLAVEID = 17
    # MB_FUNC_ERROR = 128
    
# Input read only
INPUT_READ_ONLY = 0x0100

# Address offset 
TIMESTAMP_H = 0x00
TIMESTAMP_L = 0x01
OUTPUT_POWER_H = 0x02
OUTPUT_POWER_L = 0x03
OUTPUT_CURRENT = 0x04
INPUT_VOLTAGE = 0x05
OUTPUT_VOLTAGE = 0x06
OUTPUT_CAPACITY_H = 0x07
OUTPUT_CAPACITY_L = 0x08
INTERNAL_TEMP = 0x09
EXTERNAL_TEMP = 0x0A
CELL_VOLTAGE = 0x0B
CELL_BALANCE = 0x1B
CELL_INTERNAL_RESISTANCE = 0x23
CELL_TOTAL_INTERNAL_RESISTANCE = 0x33
LINE_INTERNAL_RESISTANCE = 0x34
CYCLE_COUNT = 0x35
CONTROL_STATUS = 0x36
RUN_STATUS = 0x37
RUN_ERROR = 0x38
DIALOG_ID = 0x39
CELL_CAPACITY = 0x3A

# Control register
CONTROL_REGISTERS = 0x8000
   
# Address offset
SELECT_RUN_OPERATION = 0x00
SELECT_MEMORY = 0x01
SELECT_CHANNEL = 0x02
ORDER_LOCK = 0x03
ORDER = 0x04
LIMIT_CURRENT = 0x05
LIMIT_VOLTAGE = 0x06

class Operation:
    CHARGE = 0
    STORAGE = 1
    DISCHARGE = 2
    CYCLE = 3
    ONLY_BALANCE = 4
    POWER_SUPPLY = 5
    
class Select_memory:
    PROGRAM_0 = 0
    PROGRAM_1 = 1
    PROGRAM_2 = 2
    PROGRAM_3 = 3
    PROGRAM_4 = 4
    PROGRAM_5 = 5
    PROGRAM_6 = 6
    PROGRAM_7 = 7
    PROGRAM_8 = 8
    PROGRAM_9 = 9
    PROGRAM_10 = 10
    PROGRAM_11 = 11
    PROGRAM_12 = 12
    PROGRAM_13 = 13
    PROGRAM_14 = 14
    PROGRAM_15 = 15
    PROGRAM_16 = 16
    PROGRAM_17 = 17
    PROGRAM_18 = 18
    PROGRAM_19 = 19
    PROGRAM_20 = 20
    PROGRAM_21 = 21
    PROGRAM_22 = 22
    PROGRAM_23 = 23
    PROGRAM_24 = 24
    PROGRAM_25 = 25
    PROGRAM_26 = 26
    PROGRAM_27 = 27
    PROGRAM_28 = 28
    PROGRAM_29 = 29
    PROGRAM_30 = 30
    PROGRAM_31 = 31
    
class Select_channel:
    CHANNEL_1 = 0
    
class Order_lock:
    UNLOCK = 0x55aa
    LOCK = 0x0000
    
class Order:
    STOP = 0 # Stop running the selected channel charge/discharge procedures
    RUN = 1 # Run the selected channel & MEMORY & PROGRAM
    MODIFY = 2 # Modify the limited current and voltage parameters when running
    WRITE_SYS = 3 # Save System data in the RAM to flash memory
    WRITE_MEM_HEAD = 4 # Save MemHead data in the RAM to the flash memory
    WRITE_MEM = 5 # Save Memory data in the RAM to the flash
    TRANS_LOG_ON = 6 # Open Log transmission, this data is used for "Logview" software
    TRANS_LOG_OFF = 7 # Close Log transmission
    MSGBOX_YES = 8 # Dialog box to respond to <YES>
    MSGBOX_NO = 9 # Dialog box to respond to <NO>
    
    