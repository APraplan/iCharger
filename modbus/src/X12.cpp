#include <stdio.h>
#include <cstdint>
#include "modbus.hpp"

int main(){

    // uint8_t pIn[15] = {0x80, 0x00, 0x00, 0x05, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xaa};
    uint8_t pIn[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xaa, 0x00, 0x00};
    MasterWrite(0x8000, 5, pIn);

}