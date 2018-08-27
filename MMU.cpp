#include "MMU.h"
#include "PPU.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <assert.h>

MMU* MMU::instance = 0;

MMU* MMU::getInstance(){
    if (instance == 0){
        instance = new MMU();
    }

    return instance;
}

// Read from anywhere in memory (4B addresses)
uint8_t *MMU::read(uint16_t *address){

    // RAM Mirroring
    if(*address >= INTERNAL_MEMORY_START && *address <= INTERNAL_MEMORY_END){
        // Bytes 0x0000 - 0x07FF are mirrored to address 0x1FFF
        *address = (*address % (INTERNAL_MEMORY_END - INTERNAL_MEMORY_START + 1));
    }

    // PPU I/O reads
    if(*address >= PPU_START && *address <= PPU_MIRRORS_END){
        // Emulate mirrored addresses by performing a modulus
        *address = (*address % 8) + PPU_START;


        #ifndef NDEBUG
        if(*address == 0x2000){
            std::cout << "PPU CTRL read detected" << std::endl;
        }
        if(*address == 0x2001){
            std::cout << "PPU MASK read detected" << std::endl;
        }
        if(*address == 0x2002){
            std::cout << "PPU STATUS read detected" << std::endl;
        }
        if(*address == 0x2003){
            std::cout << "OAM ADDR read detected" << std::endl;
        }
        if(*address == 0x2004){
            std::cout << "OAM DATA read detected" << std::endl;
        }
        if(*address == 0x2005){
            std::cout << "PPU SCROLL read detected" << std::endl;
        }
        if(*address == 0x2006){
            std::cout << "PPU ADDR read detected" << std::endl;
        }
        if(*address == 0x2007){
            std::cout << "PPU DATA read detected" << std::endl;
        }
        if(*address == 0x4014){
            std::cout << "OAM DMA read detected" << std::endl;
        }
    #endif

    }

    return &memory[*address];
}

// Read at Zero-page location
uint8_t *MMU::read(uint8_t *address){
    return &memory[*address];
}

// Pop value from stack
uint8_t *MMU::readStack(uint8_t *stack_pointer){
    uint16_t address = 0;
    address += *stack_pointer;
    address += STACK_START;
    return read(&address);
}

// Full Memory access (4B addresses)
void MMU::write(uint16_t *address, uint8_t *value){

    // Detect CHR-ROM Bank switching via Memory Mapper

    // RAM Mirroring
    if(*address >= INTERNAL_MEMORY_START && *address <= INTERNAL_MEMORY_END){
        // Bytes 0x0000 - 0x07FF are mirrored to address 0x1FFF
        *address = (*address % (INTERNAL_MEMORY_END - INTERNAL_MEMORY_START + 1));
        memory[*address] = *value;
        return;
    }

    // PPU I/O writes
    if(*address >= PPU_START && *address <= PPU_MIRRORS_END){

        // Emulate mirrored addresses by performing a modulus
        *address = (*address % 8) + PPU_START;

        #ifndef NDEBUG
            std::cout << "PPU write modified address: " << *address << std::endl;
            if(*address == 0x2000){
                std::cout << "PPU CTRL write detected with value: " << +(*value) << std::endl;
            }
            if(*address == 0x2001){
                std::cout << "PPU MASK write detected with value: " << +(*value) << std::endl;
            }
            if(*address == 0x2002){
                std::cout << "PPU STATUS write detected with value: " << +(*value) << std::endl;
            }
            if(*address == 0x2003){
                std::cout << "OAM ADDR write detected with value: " << +(*value) << std::endl;
            }
            if(*address == 0x2004){
                std::cout << "OAM DATA write detected with value: " << +(*value) << std::endl;
            }
            if(*address == 0x2005){
                std::cout << "PPU SCROLL write detected with value: " << +(*value) << std::endl;
            }
            if(*address == 0x2006){
                std::cout << "PPU ADDR write detected with value: " << +(*value) << std::endl;
            }
            if(*address == 0x2007){
                std::cout << "PPU DATA write detected with value: " << +(*value) << std::endl;
            }
            if(*address == 0x4014){
                std::cout << "OAM DMA write detected with value: " << +(*value) << std::endl;
            }
        #endif

            // Override MMU write into CPU memory
            if (*address == PPU_ADDR){
                PPU::getInstance()->addrPort(value);
                return;
            }
            if (*address == PPU_DATA){
                PPU::getInstance()->dataPort(value);
                return;
            }
            

        //PPU::getInstance()->write(address, value);
    }

    memory[*address] = *value;
}

// Zero-page writes (2B addresses)
void MMU::write(uint8_t *address, uint8_t *value){
    memory[*address] = *value;
}

// Push value into stack
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