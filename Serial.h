#pragma once
#include <windows.h>
#include <stdio.h>
#include <cstdint>


void requestMCU(HANDLE hSerial);
bool readMCU(HANDLE hSerial);
uint16_t checkCrc(uint8_t* data_blk_ptr, uint16_t data_blk_size);
HANDLE initSerial(WCHAR* string);


int getLength(char str[]);