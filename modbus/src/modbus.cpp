#include "modbus.hpp"
#include <iostream>
#include <stdio.h>
#include <cstdint>

#include <wchar.h>
#include "../../hidapi/hidapi/hidapi.h"
// #include "hidapi.h"

using namespace std;

#define READ_REG_COUNT_MAX		((HID_PACK_MAX-4)/2) //30
#define WRITE_REG_COUNT_MAX		((HID_PACK_MAX-8)/2) //28
#define TIME_OUT	500

//  gcc -fPIC -shared -o hidapi.so windows/hid.c -Ihidapi
//  g++ -fPIC -shared -o CModbus.so modbus.cpp -Ihidapi/hidapi -Lhidapi\build\src\windows -Ihidapi


// Need to implement the same thing with the hid api lib
#include <iomanip>
void DisplayArrayInHex(const uint8_t* array, size_t length) {
    printf("Array in hex: [");
    for (size_t i = 0; i < length; ++i) {
        // Print each byte in hex format
        printf("%02x", static_cast<int>(array[i]));
        if (i < length - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}


class hid_dev {
private:
    hid_device *handle;

public:
    hid_dev() {
        // Initialize the hidapi library
		printf("Hid init\n");
        hid_init();

        // Open the device using the VID, PID, and optionally the Serial number.
        handle = hid_open(0x0483, 0x5751, NULL);
        if (!handle) {
            printf("Unable to open device\n");
            hid_exit();
        }
    }

    ~hid_dev() {
        // Close the HID device and cleanup
        hid_close(handle);
        hid_exit();
    }

    bool read(uint8_t* buffer, size_t length) {
        int res = hid_read(handle, buffer, length);
        return (res >= 0);
    }

    bool write(const uint8_t* buffer, size_t length) {
        int res = hid_write(handle, buffer, length);
        return (res >= 0);
    }
};

hid_dev device;

eMBErrorCode MasterRead(uint8_t ReadType,uint32_t RegStart,uint32_t RegCount,uint8_t *pOut)
{
	eMBErrorCode ret;
	uint32_t i;
	uint8_t InBuf[16];
	uint8_t FunCode;

	if(ReadType==0)
		FunCode = MB_FUNC_READ_HOLDING_REGISTER;
	else
		FunCode = MB_FUNC_READ_INPUT_REGISTER;

	for(i=0;i<RegCount/READ_REG_COUNT_MAX;i++)
	{
		InBuf[0] = (uint8_t)(RegStart >> 8);
		InBuf[1] = (uint8_t)(RegStart & 0xff);
		InBuf[2] = 0;
		InBuf[3] = READ_REG_COUNT_MAX;
		ret = MasterModBus(FunCode,InBuf,pOut,TIME_OUT);
		if(ret != MB_EOK)return ret;
		RegStart += READ_REG_COUNT_MAX;
		pOut += (2*READ_REG_COUNT_MAX);
	}

	if(RegCount%READ_REG_COUNT_MAX)
	{
		InBuf[0] = (uint8_t)(RegStart >> 8);
		InBuf[1] = (uint8_t)(RegStart & 0xff);
		InBuf[2] = 0;
		InBuf[3] = (uint8_t) RegCount%READ_REG_COUNT_MAX;	
		ret = MasterModBus(FunCode,InBuf,pOut,TIME_OUT);
		if(ret != MB_EOK)return ret;
	}
	return ret;
}

eMBErrorCode MasterWrite(uint32_t RegStart,uint32_t RegCount,uint8_t *pIn)
{
	eMBErrorCode ret;
	uint32_t i,j;
	uint8_t InBuf[80];

	for(i=0;i<RegCount/WRITE_REG_COUNT_MAX;i++)
	{
		InBuf[0] = (uint8_t)(RegStart >> 8);
		InBuf[1] = (uint8_t)(RegStart & 0xff);
		InBuf[2] = 0;
		InBuf[3] = WRITE_REG_COUNT_MAX;
		InBuf[4] = 2*WRITE_REG_COUNT_MAX;

		for(j=0;j<InBuf[4];j=j+2)
		{
			InBuf[5+j] = pIn[j+1];
			InBuf[5+j+1] = pIn[j];
		}
		ret = MasterModBus(MB_FUNC_WRITE_MULTIPLE_REGISTERS,InBuf,NULL,TIME_OUT);
		if(ret != MB_EOK)return ret;
		RegStart += WRITE_REG_COUNT_MAX;
		pIn += (2*WRITE_REG_COUNT_MAX);
	}

	if(RegCount%WRITE_REG_COUNT_MAX)
	{
		InBuf[0] = (uint8_t)(RegStart >> 8);
		InBuf[1] = (uint8_t)(RegStart & 0xff);
		InBuf[2] = 0;
		InBuf[3] = (uint8_t) RegCount%WRITE_REG_COUNT_MAX;
		InBuf[4] = 2*InBuf[3];

		// printf("MasterWrite\n");
		// DisplayArrayInHex(InBuf, HID_PACK_MAX+1);

		for(j=0;j<InBuf[4];j=j+2)
		{
			InBuf[5+j] = pIn[j];
			InBuf[5+j+1] = pIn[j+1];
		}
		ret = MasterModBus(MB_FUNC_WRITE_MULTIPLE_REGISTERS,InBuf,NULL,TIME_OUT);
		if(ret != MB_EOK)return ret;
	}
	return ret;
}

eMBErrorCode MasterModBus(uint8_t FunCode,uint8_t *pIn,uint8_t *pOut,uint32_t ms)
{
	int i;
	uint8_t HidBuf[HID_PACK_MAX+1];

	HidBuf[HID_PACK_CH] = REPORT_ID;
	HidBuf[HID_PACK_TYPE] = MB_HID_PROTOCOL_ID;
	HidBuf[HID_PACK_MODBUS] = FunCode;

	// printf("MasterModBus\n");
	// DisplayArrayInHex(HidBuf, HID_PACK_MAX+1);

	switch(FunCode)
	{
	case MB_FUNC_READ_INPUT_REGISTER:	//Modbus function 0x04 Read Input Registers
		if(0)
			return MB_ENOREG;
		HidBuf[HID_PACK_LEN] = 7;
		break;
	case MB_FUNC_READ_HOLDING_REGISTER:  //Modbus function 0x03 Read Holding Registers
		if(0)
			return MB_ENOREG;
		HidBuf[HID_PACK_LEN] = 7;
		break;
	case MB_FUNC_WRITE_MULTIPLE_REGISTERS:	//Modbus function 0x10 Write Multiple Registers
		if(0)
			return MB_ENOREG;
		HidBuf[HID_PACK_LEN] = 7+(pIn[4]+1);
		if(HidBuf[HID_PACK_LEN] > HID_PACK_MAX)return MB_ELEN;
		break;
	default:
		return MB_EILLFUNCTION;
	}
	//copy from pD
	for(i=0;i<HidBuf[HID_PACK_LEN]-3;i++)
		HidBuf[HID_PACK_MODBUS+1+i]=pIn[i];

	//trans
	// if(JsHID.Write(HidBuf,HID_PACK_MAX+1)==FALSE)return MB_EIO;
	printf("MasterModBus write\n");
    DisplayArrayInHex(HidBuf, HID_PACK_MAX+1);
    if (!device.write(HidBuf, HID_PACK_MAX+1)) return MB_EIO;
	HidBuf[HID_PACK_CH]=REPORT_ID;
    if (!device.read(HidBuf, HID_PACK_MAX+1)) return MB_ETIMEDOUT;
    DisplayArrayInHex(HidBuf, HID_PACK_MAX+1);
	// if(JsHID.Read(HidBuf,HID_PACK_MAX+1,ms)==FALSE)return MB_ETIMEDOUT;
	if(HidBuf[HID_PACK_LEN] > HID_PACK_MAX)return MB_ELEN;
	

	if(HidBuf[HID_PACK_MODBUS] == FunCode)
	{
		switch(FunCode)
		{
		case MB_FUNC_READ_INPUT_REGISTER:	//Modbus function 0x04 Read Input Registers
		case MB_FUNC_READ_HOLDING_REGISTER:  //Modbus function 0x03 Read Holding Registers
			if((HidBuf[HID_PACK_LEN] != HidBuf[HID_PACK_MODBUS+1]+4) || (HidBuf[HID_PACK_LEN] & 0x01))return MB_ELEN;
			//copy to pOut
			for(i=0;i<HidBuf[HID_PACK_MODBUS+1];i=i+2)
			{
				pOut[i] = HidBuf[HID_PACK_MODBUS+2+i+1]; 
				pOut[i+1] = HidBuf[HID_PACK_MODBUS+2+i]; 
			}
			break;
		case MB_FUNC_WRITE_MULTIPLE_REGISTERS:	//Modbus function 0x10 Write Multiple Registers
			HidBuf[HID_PACK_LEN] = 0+5+(HidBuf[HID_PACK_MODBUS+5]*2+1);	
			break;
		}
	}
	else if(HidBuf[HID_PACK_MODBUS] == (FunCode | 0x80))
		return (eMBErrorCode)HidBuf[HID_PACK_MODBUS+1];
	else
		return MB_ERETURN;

	return MB_EOK;
}