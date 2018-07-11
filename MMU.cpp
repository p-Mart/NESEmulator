#include "MMU.h"
MMU* MMU::instance = 0;

MMU* MMU::getInstance(){
    if (instance == 0){
        instance = new MMU();
    }

    return instance;
}

uint8_t *MMU::read(uint16_t address){
    return memory[address];
}

void MMU::write(uint16_t address, uint8_t *value){
    memory[address] = value;
}