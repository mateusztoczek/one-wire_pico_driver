#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "pico/types.h"

typedef struct {
    uint pin;
} OneWire;


int OneWire_Reset(OneWire *bus);
void OneWire_WriteBit(OneWire *bus, bool bit);
int OneWire_ReadBit(OneWire *bus);
void OneWire_WriteByte(OneWire *bus, uint8_t data);
uint8_t OneWire_ReadByte(OneWire *bus);
uint8_t OneWire_TransferByte(OneWire *bus, uint8_t data);
void OneWire_TransferBuffer(OneWire *bus, uint8_t *data_buffer, size_t data_len);