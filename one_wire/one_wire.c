#include "hardware/timer.h"
#include "hardware/gpio.h"
#include "one_wire.h"


#define ONEWIRE_STANDARD_TIMER_A 6
#define ONEWIRE_STANDARD_TIMER_B 64
#define ONEWIRE_STANDARD_TIMER_C 60
#define ONEWIRE_STANDARD_TIMER_D 10
#define ONEWIRE_STANDARD_TIMER_E 9
#define ONEWIRE_STANDARD_TIMER_F 55
#define ONEWIRE_STANDARD_TIMER_G 0
#define ONEWIRE_STANDARD_TIMER_H 480
#define ONEWIRE_STANDARD_TIMER_I 70
#define ONEWIRE_STANDARD_TIMER_J 410


static void OneWire_Delay(uint32_t us){
    busy_wait_us(us);
}

static void OneWire_DriveLow(OneWire *bus){
    gpio_put(bus->pin, 0);
    gpio_set_dir(bus->pin, GPIO_OUT);
}

static void OneWire_Release(OneWire *bus){
    gpio_set_dir(bus->pin, GPIO_IN);
}

static bool OneWire_Read(OneWire *bus){
    return gpio_get(bus->pin);
}


bool OneWire_Reset(OneWire *bus){
    bool result;

    OneWire_Delay(ONEWIRE_STANDARD_TIMER_G);
    OneWire_DriveLow(bus);
    OneWire_Delay(ONEWIRE_STANDARD_TIMER_H);
    OneWire_Release(bus);
    OneWire_Delay(ONEWIRE_STANDARD_TIMER_I); 
    result = !OneWire_Read(bus);
    OneWire_Delay(ONEWIRE_STANDARD_TIMER_J);
    return result;
}


void OneWire_WriteBit(OneWire *bus, bool bit){
    if(bit){
        OneWire_DriveLow(bus);
        OneWire_Delay(ONEWIRE_STANDARD_TIMER_A);
        OneWire_Release(bus);
        OneWire_Delay(ONEWIRE_STANDARD_TIMER_B);
    }else{
        OneWire_DriveLow(bus);
        OneWire_Delay(ONEWIRE_STANDARD_TIMER_C);
        OneWire_Release(bus);
        OneWire_Delay(ONEWIRE_STANDARD_TIMER_D);
    }
}


bool OneWire_ReadBit(OneWire *bus){
    bool result;

    OneWire_DriveLow(bus);
    OneWire_Delay(ONEWIRE_STANDARD_TIMER_A);
    OneWire_Release(bus);
    OneWire_Delay(ONEWIRE_STANDARD_TIMER_E);
    result = OneWire_Read(bus);
    OneWire_Delay(ONEWIRE_STANDARD_TIMER_F);
    return result;
}


void OneWire_WriteByte(OneWire *bus, uint8_t data){ 
    for (int i=0; i<8; i++){
        OneWire_WriteBit(bus, data & 0x01);
        data >>= 1;
    }
}

uint8_t OneWire_ReadByte(OneWire *bus){
    uint8_t result = 0;
    for (int i=0; i<8; i++){
        result >>= 1;
        if (OneWire_ReadBit(bus)) result |= 0x80;
    }

    return result;
}


uint8_t OneWire_TransferByte(OneWire *bus, uint8_t data){
    uint8_t result =0;
    for (int i=0; i<8; i++){
        result >>= 1;
        if (data & 0x01){
            if (OneWire_ReadBit(bus)) result |= 0x80;
        }
        else OneWire_WriteBit(bus, false);
        data >>= 1;
    }
    return result;
}


void OneWire_TransferBuffer(OneWire *bus, uint8_t *data_buffer, size_t data_len){
    for (int i=0; i<data_len; i++){
        data_buffer[i]= OneWire_TransferByte(bus, data_buffer[i]);
    }
}