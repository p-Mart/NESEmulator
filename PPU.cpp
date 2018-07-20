#include "PPU.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

PPU* PPU::instance = 0;

PPU* PPU::getInstance(){
    if (instance == 0){
        instance = new PPU();
    }

    return instance;
}

uint8_t *PPU::read(uint16_t *address){
    return &memory[*address];
}

uint8_t *PPU::read(uint8_t *address){
    return &memory[*address];
}

void PPU::write(uint16_t *address, uint8_t *value){
    memory[*address] = *value;
}

void PPU::write(uint8_t *address, uint8_t *value){
    memory[*address] = *value;
}

void PPU::memDump(){
    std::string dumpfile_name = "PPU_Dump.txt";
    std::ofstream dumpfile;
    std::stringstream sstream;
    dumpfile.open(dumpfile_name);

    int bytes_per_line = 16;
    // Print out 16 bytes per line
    for(int i = 0; i < PPU_MEMORY_SIZE; i+=bytes_per_line){
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