#include <stdio.h>
#include <cstdint>
#include "modbus.hpp"

#include <stdio.h>
#include <cstdint>
#include "modbus.hpp"
#include "modbus_address.hpp"

#include <string>


float formatVoltage(uint8_t* buffer, int index) {
    float value = ((buffer[index+1] << 8) | buffer[index]) / 1000.0;
    return value;
}

int main(int argc, char* argv[]) {

    eMBErrorCode ret;

    if (argc != 2) {
        printf("Not enough input arguments\n");
        printf("Supported arguments: start, stop, read\n");
        return 1;
    }

    std::string command(argv[1]);
    
    if(command == "start"){
        uint8_t pIn[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xaa, 0x00, ORDER_RUN};
        ret = MasterWrite(control_register.select_operation, 5, pIn);
        
    }
    else if(command == "stop"){
        uint8_t pIn[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xaa, 0x00, ORDER_STOP};
        ret = MasterWrite(control_register.select_operation, 5, pIn);
    }
    else if(command == "read"){
            uint8_t pOut[64] = {0};
        ret = MasterRead(input_read_only.cell_0_15_voltage, 12, pOut);

        for (int i = 0; i < 24; i+=2) {
        float value = formatVoltage(pOut, i);
        printf("Voltage cell %.1i : %.3f\n",i/2+1 , value);
    }
        
    }
    else{
        printf("Invalid input argument\n");
        printf("Supported arguments: Start, Stop, Read\n");
        return 1;
    }

    if(ret != MB_EOK){
        printf("Error: %d\n", ret);
    }
    else{
        printf("Success\n");
    }

    return 0;
}