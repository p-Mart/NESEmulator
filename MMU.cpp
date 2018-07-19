#include "MMU.h"
MMU* MMU::instance = 0;

MMU* MMU::getInstance(){
    if (instance == 0){
        instance = new MMU();
    }

    return instance;
}

uint8_t *MMU::read(uint16_t *address){
    return &memory[*address];
}

uint8_t *MMU::read(uint8_t *address){
    return &memory[*address];
}

uint8_t *MMU::readStack(uint8_t *stack_pointer){
    uint16_t address = 0;
    address += *stack_pointer;
    address += STACK_START;

    return read(&address);
}

void MMU::write(uint16_t *address, uint8_t *value){
    memory[*address] = *value;
}

void MMU::write(uint8_t *address, uint8_t *value){
    memory[*address] = *value;
}

void MMU::writeStack(uint8_t *stack_pointer, uint8_t *value){
    uint16_t address = 0;
    address += *stack_pointer;
    address += STACK_START;

    write(&address, value);
}