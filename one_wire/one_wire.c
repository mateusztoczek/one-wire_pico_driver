#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "one_wire.h"


bool OneWire_Init(OneWire *bus, uint pin){
    bus->pin= pin;
    bus->pio= pio0;
    int state_machine_resp = pio_claim_unused_sm(bus->pio, false);
    if (state_machine_resp <0) return false;
    bus->state_machine= (uint)state_machine_resp;
    if(!pio_can_add_program(bus->pio, &onewire_program)){
        pio_sm_unclaim(bus->pio, bus->state_machine);
        return false;
    }
    bus->offset = pio_add_program(bus->pio, &onewire_program);
    pio_sm_config sm_config=onewire_program_get_default_config(bus->offset);

    sm_config_set_in_pins(&sm_config, pin);
    sm_config_set_sideset_pins(&sm_config, pin);
    sm_config_set_in_shift(&sm_config,true,false, 32);
    sm_config_set_out_shift(&sm_config, true,false,32);
    float cycle_divider = (float)clock_get_hz(clk_sys)/1000000.0f;
    sm_config_set_clkdiv(&sm_config, cycle_divider);
    pio_gpio_init(bus->pio,pin);

    pio_sm_set_pins_with_mask(bus->pio, bus->state_machine, 0u, 1u << pin);
    pio_sm_set_pindirs_with_mask(bus->pio,bus->state_machine,0u, 1u << pin);
    pio_sm_init(bus->pio, bus->state_machine, bus->offset, &sm_config);
    pio_sm_set_enabled(bus->pio,bus->state_machine,true);

    return true;
}


bool OneWire_TransferBit(OneWire *bus, bool bit){
    uint32_t command= bus->offset +onewire_offset_transfer_bit;
    pio_sm_put_blocking(bus->pio, bus->state_machine, command);
    pio_sm_put_blocking( bus->pio, bus->state_machine, bit ? 1u:0u);
    uint32_t response= pio_sm_get_blocking( bus->pio, bus->state_machine);

    return (response >> 31) &1u;
}


bool OneWire_ReadBit(OneWire *bus){
    return OneWire_TransferBit(bus, true);
}


void OneWire_WriteBit(OneWire *bus, bool bit){
    OneWire_TransferBit(bus, bit);
}


bool OneWire_Reset(OneWire *bus){
    uint32_t command= bus->offset +onewire_offset_reset;
    pio_sm_put_blocking(bus->pio, bus->state_machine, command);
    uint32_t response= pio_sm_get_blocking( bus->pio, bus->state_machine);
    bool resp_state= (response >> 31) &1u; 
    
    return !resp_state;
}


uint8_t OneWire_TransferByte(OneWire *bus, uint8_t data){
    uint32_t command= bus->offset +onewire_offset_transfer_byte;
    pio_sm_put_blocking(bus->pio, bus->state_machine, command);
    pio_sm_put_blocking( bus->pio, bus->state_machine, data);
    uint32_t response= pio_sm_get_blocking( bus->pio, bus->state_machine);

    return (uint8_t)(response >> 24);
}


uint8_t OneWire_ReadByte(OneWire *bus){
    return OneWire_TransferByte(bus,0xFFu);
}


void OneWire_WriteByte(OneWire *bus, uint8_t data){ 
    OneWire_TransferByte(bus, data);
}


void OneWire_TransferBuffer(OneWire *bus, uint8_t *data_buffer, size_t data_len){
    for (size_t i=0; i<data_len; i++){
        data_buffer[i]= OneWire_TransferByte(bus, data_buffer[i]);
    }
}