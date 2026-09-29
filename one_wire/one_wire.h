#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "pico/types.h"
#include "hardware/pio.h"
#include "one_wire.pio.h"


typedef struct {
    PIO pio;
    uint pin;
    uint state_machine;
    uint offset;
} OneWire;

bool OneWire_Init(OneWire *bus, uint pin);
bool OneWire_Reset(OneWire *bus);
void OneWire_WriteBit(OneWire *bus, bool bit);
bool OneWire_ReadBit(OneWire *bus);
void OneWire_WriteByte(OneWire *bus, uint8_t data);
uint8_t OneWire_ReadByte(OneWire *bus);
uint8_t OneWire_TransferByte(OneWire *bus, uint8_t data);
void OneWire_TransferBuffer(OneWire *bus, uint8_t *data_buffer, size_t data_len);