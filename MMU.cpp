#include "MMU.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

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

// Utility function to dump contents of memory
// output file is MMU_Dump.txt
void MMU::memDump(){
    std::string dumpfile_name = "MMU_Dump.txt";
    std::ofstream dumpfile;
    std::stringstream sstream;
    dumpfile.open(dumpfile_name);

    int bytes_per_line = 16;
    // Print out 16 bytes per line
    for(int i = 0; i < MEMORY_SIZE; i+=bytes_per_line){
        sstream.str("");
        sstream << std::setfill('0') << std::setw(4) << std::hex << i;
        sstream << ":\t";

        for(int j = 0; j < bytes_per_line; j++){
            sstream << std::setfill('0') << std::setw(2) << std::hex << +memory[i+j];
            sstream << " ";
        }
        dumpfile << sstream.str() << std::endl;
    }

    dumpfile.close();
}