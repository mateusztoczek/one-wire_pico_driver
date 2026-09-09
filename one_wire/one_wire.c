#include "hardware/timer.h"
#include "hardware/gpio.h"
#include "one_wire.h"


#define USE_STANDARD_SPEED 
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

static void OneWire_DriveLow(uint pin){
    gpio_put(pin, 0);
    gpio_set_dir(pin, GPIO_OUT);
}

static void OneWire_Release(uint pin){
    gpio_set_dir(pin, GPIO_IN);
}

static bool OneWire_Read(uint pin){
    return gpio_get(pin);
}


int OneWire_Reset(uint pin){
    int result;

    OneWire_Delay(ONEWIRE_STANDARD_TIMER_G);
    OneWire_DriveLow(pin);
    OneWire_Delay(ONEWIRE_STANDARD_TIMER_H);
    OneWire_Release(pin);
    OneWire_Delay(ONEWIRE_STANDARD_TIMER_I); 
    result = !OneWire_Read(pin)
    OneWire_Delay(ONEWIRE_STANDARD_TIMER_J);
    return result;
}


void OneWire_WriteBit(uint pin, bool bit){
    if(bit){
        OneWire_DriveLow(pin);
        OneWire_Delay(ONEWIRE_STANDARD_TIMER_A);
        OneWire_Release(pin);
        OneWire_Delay(ONEWIRE_STANDARD_TIMER_B);
    }else{
        OneWire_DriveLow(pin);
        OneWire_Delay(ONEWIRE_STANDARD_TIMER_C);
        OneWire_Release(pin);
        OneWire_Delay(ONEWIRE_STANDARD_TIMER_D);
    }
}


int OneWire_ReadBit(uint pin){
    bool result;

    OneWire_DriveLow(pin);
    OneWire_Delay(ONEWIRE_STANDARD_TIMER_A);
    OneWire_Release(pin);
    OneWire_Delay(ONEWIRE_STANDARD_TIMER_E);
    result = OneWire_Read(pin)
    OneWire_Delay(ONEWIRE_STANDARD_TIMER_F);
    return result;
}


void OneWire_WriteByte(uint pin, uint8_t data){ 
    for (int i=0; i<8; i++){
        OneWire_WriteBit(pin, data & 0x01);
        data >>= 1;
    }
}

uint8_t OneWire_ReadByte(uint pin){
    uint8_t result = 0;
    for (int i=0; i<8; i++){
        result >>= 1;
        if (OneWire_ReadBit(pin)) result |= 0x80;
    }

    return result;
}


uint8_t OneWire_TransferByte(uint pin, uint8_t data){
    uint8_t result =0;
    for (int i=0; i<8; i++){
        result >>= 1;
        if (data & 0x01){
            if (OneWire_ReadBit(pin)) result |= 0x80;
        }
        else OneWire_WriteBit(pin, false);
        data >>= 1;
    }
    return result;
}


void OneWire_TransferBuffer(uint pin, uint8_t *data_buffer, size_t data_len){
    for (int i=0; i<data_len; i++){
        data_buffer[i]= OneWire_TransferByte(pin, data_buffer[i]);
    }
}


//TODO: set overdrive skip