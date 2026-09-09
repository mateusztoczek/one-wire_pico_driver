#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "pico/types.h"


int OneWire_Reset(uint pin);
void OneWire_WriteBit(uint pin, bool bit);
int OneWire_ReadBit(uint pin);
void OneWire_WriteByte(uint pin, uint8_t data);
uint8_t OneWire_ReadByte(uint pin);
uint8_t OneWire_TransferByte(uint pin, uint8_t data);
void OneWire_TransferBuffer(uint pin, uint8_t *data_buffer, size_t data_len);