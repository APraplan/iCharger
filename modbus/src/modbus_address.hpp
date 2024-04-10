#ifndef MODBUS_ADDRESSES_H_
#define MODBUS_ADDRESSES_H_

#include <cstdint>

#define VENDOR_ID 0x0483
#define PRODUCT_ID 0x5751

#define ORDER_STOP  0x00
#define ORDER_RUN  0x01
#define ORDER_MODIFY  0x02
#define ORDER_WRITE_SYS  3
#define ORDER_WRITE_MEM_HEAD  0x04
#define ORDER_WRITE_MEM  0x05
#define ORDER_TRANS_LOG_ON  0x06
#define ORDER_TRANS_LOG_OFF  0x07
#define ORDER_MSGBOX_YES  0x08
#define ORDER_MSGBOX_NO  0x09

#define X12_DEVICE_READ_ONLY 0x0000
struct X12_device_read_only{
    uint16_t device_ID = X12_DEVICE_READ_ONLY + 0x0000;
    uint16_t device_SN = X12_DEVICE_READ_ONLY + 0x0001;
    uint16_t software_version = X12_DEVICE_READ_ONLY + 0x0007;
    uint16_t hardware_version = X12_DEVICE_READ_ONLY + 0x0008;
    uint16_t system_length = X12_DEVICE_READ_ONLY + 0x0009;
    uint16_t memory_length = X12_DEVICE_READ_ONLY + 0x000A;
    uint16_t status_word = X12_DEVICE_READ_ONLY + 0x000B;
};

X12_device_read_only device_read_only;

#define X12_INPUT_READ_ONLY 0x0100
struct X12_input_read_only{
    uint16_t timestamp = X12_INPUT_READ_ONLY + 0x0000;
    uint16_t current_output_power = X12_INPUT_READ_ONLY + 0x0002;
    uint16_t current_output_current = X12_INPUT_READ_ONLY + 0x0004;
    uint16_t current_input_voltage = X12_INPUT_READ_ONLY + 0x0005;
    uint16_t current_output_voltage = X12_INPUT_READ_ONLY + 0x0006;
    uint16_t current_output_capacity = X12_INPUT_READ_ONLY + 0x0007;
    uint16_t current_internal_temperature = X12_INPUT_READ_ONLY + 0x0009;
    uint16_t current_external_temperature = X12_INPUT_READ_ONLY + 0x000A;
    uint16_t cell_0_15_voltage = X12_INPUT_READ_ONLY + 0x000B;
    uint16_t cell_0_15_balance_status = X12_INPUT_READ_ONLY + 0x001B;
    uint16_t cell_0_15_internal_resistance = X12_INPUT_READ_ONLY + 0x0023;
    uint16_t cell_total_internal_resistance = X12_INPUT_READ_ONLY + 0x0033;
    uint16_t line_internal_resistance = X12_INPUT_READ_ONLY + 0x0034;
    uint16_t cycles_count = X12_INPUT_READ_ONLY + 0x0035;
    uint16_t control_status = X12_INPUT_READ_ONLY + 0x0036;
    uint16_t run_status = X12_INPUT_READ_ONLY + 0x0037;
    uint16_t run_error = X12_INPUT_READ_ONLY + 0x0038;
    uint16_t dialog_bos = X12_INPUT_READ_ONLY + 0x0039;
    uint16_t cell_0_15_capacity = X12_INPUT_READ_ONLY + 0x003A;
};

X12_input_read_only input_read_only;

#define X12_CONTROL_REGISTER 0x8000
struct X12_control_register{
    uint16_t select_operation = X12_CONTROL_REGISTER + 0x0000;
    uint16_t select_memory = X12_CONTROL_REGISTER + 0x0001;
    uint16_t select_channel = X12_CONTROL_REGISTER + 0x0002;
    uint16_t order_lock = X12_CONTROL_REGISTER + 0x0003;
    uint16_t order = X12_CONTROL_REGISTER + 0x0004;
    uint16_t limit_current = X12_CONTROL_REGISTER + 0x0005;
    uint16_t limit_voltage = X12_CONTROL_REGISTER + 0x0006;
};

X12_control_register control_register;

#define X12_SYSTEM_STORAGE_AREA 0x8400
struct X12_system_storage_area{
    uint16_t system_parametes = X12_SYSTEM_STORAGE_AREA + 0x0000;

};

#define X12_INDEX_STORAGE_AREA 0x8800


#define X12_MEMORY_STORAGE_AREA 0x8C00
#define MEM_NAME_LEN 37
struct X12_memory_storage_area{
    uint16_t memory_head = X12_MEMORY_STORAGE_AREA + 0x0000;
    uint16_t memory = X12_MEMORY_STORAGE_AREA + 0x0001;
};

// struct _system{
//     uint16_t tempUnit;
//     uint16_t tempStop;
//     uint16_t tempFansOn;
//     uint16_t tempReduce;
//     uint16_t fansOffDelay;
//     uint16_t lcdContrast;
//     uint16_t lightValue;
//     uint16_t beepType[4];
//     uint16_t beepEnable[4];
//     uint16_t beepVOL[4];
//     uint16_t selectLanguage;
//     uint16_t selectAdj;
//     uint16_t ver;

// };


#endif