#include "CPU.h"
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
    
    uint16_t hardware_address = *address;
    // RAM Mirroring
    if(hardware_address >= INTERNAL_MEMORY_START && hardware_address <= INTERNAL_MEMORY_END){
        // Bytes 0x0000 - 0x07FF are mirrored to address 0x1FFF
        hardware_address = (hardware_address % (INTERNAL_MEMORY_END - INTERNAL_MEMORY_START + 1));
    }

    // PPU I/O reads
    if(hardware_address >= PPU_START && hardware_address <= PPU_MIRRORS_END){
        // Emulate mirrored addresses by performing a modulus
        hardware_address = (hardware_address % 8) + PPU_START;


        #ifndef NDEBUG
            if(hardware_address == 0x2000){
                std::cout << "PPU CTRL read detected" << std::endl;
            }
            if(hardware_address == 0x2001){
                std::cout << "PPU MASK read detected" << std::endl;
            }
            if(hardware_address == 0x2002){
                std::cout << "PPU STATUS read detected" << std::endl;
            }
            if(hardware_address == 0x2003){
                std::cout << "OAM ADDR read detected" << std::endl;
            }
            if(hardware_address == 0x2004){
                std::cout << "OAM DATA read detected" << std::endl;
            }
            if(hardware_address == 0x2005){
                std::cout << "PPU SCROLL read detected" << std::endl;
            }
            if(hardware_address == 0x2006){
                std::cout << "PPU ADDR read detected" << std::endl;
            }
            if(hardware_address == 0x2007){
                std::cout << "PPU DATA read detected" << std::endl;
            }

        #endif

        if(hardware_address == PPU_STATUS){
            // NMI_occurred bit needs to be set off on read here
            PPU::getInstance()->statusRead();
        }
    }

    #ifndef NDEBUG
    if(hardware_address == DMC_FREQ){
        std::cout << "DMC FREQ Write read detected" << std::endl;
    }
    if(hardware_address == JOY2){
        std::cout << "JOY2 / APU FRAME CONTROL read detected" << std::endl;
    }
    if(hardware_address == OAMDMA){
        std::cout << "OAM DMA read detected" << std::endl;
    }
    if(hardware_address == JOY1){
        std::cout << "JOY1 read detected" <<std::endl;
    }
    #endif


    return &memory[hardware_address];
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

    uint16_t hardware_address = *address;
    // Detect CHR-ROM Bank switching via Memory Mapper

    // RAM Mirroring
    if(hardware_address >= INTERNAL_MEMORY_START && hardware_address <= INTERNAL_MEMORY_END){
        // Bytes 0x0000 - 0x07FF are mirrored to address 0x1FFF
        hardware_address = (hardware_address % (INTERNAL_MEMORY_END - INTERNAL_MEMORY_START + 1));
        memory[hardware_address] = *value;
        return;
    }

    // PPU I/O writes
    if(hardware_address >= PPU_START && hardware_address <= PPU_MIRRORS_END){

        // Emulate mirrored addresses by performing a modulus
        hardware_address = (hardware_address % 8) + PPU_START;

        #ifndef NDEBUG
            if(hardware_address == 0x2000){
                std::cout << "PPU CTRL write detected with value: " << +(*value) << std::endl;
            }
            if(hardware_address == 0x2001){
                std::cout << "PPU MASK write detected with value: " << +(*value) << std::endl;
            }
            if(hardware_address == 0x2002){
                std::cout << "PPU STATUS write detected with value: " << +(*value) << std::endl;
            }
            if(hardware_address == 0x2003){
                std::cout << "OAM ADDR write detected with value: " << +(*value) << std::endl;
            }
            if(hardware_address == 0x2004){
                std::cout << "OAM DATA write detected with value: " << +(*value) << std::endl;
            }
            if(hardware_address == 0x2005){
                std::cout << "PPU SCROLL write detected with value: " << +(*value) << std::endl;
            }
            if(hardware_address == 0x2006){
                std::cout << "PPU ADDR write detected with value: " << +(*value) << std::endl;
            }
            if(hardware_address == 0x2007){
                std::cout << "PPU DATA write detected with value: " << +(*value) << std::endl;
            }
            if(hardware_address == 0x4014){
                std::cout << "OAM DMA write detected with value: " << +(*value) << std::endl;
            }
        #endif

            // Redirect write into PPU
            if (hardware_address == PPU_ADDR){
                PPU::getInstance()->addrPort(value);
                return;
            }
            if (hardware_address == PPU_DATA){
                PPU::getInstance()->dataPort(value);
                return;
            }
    }

    // APU I/O Writes
    #ifndef NDEBUG
    if(hardware_address == DMC_FREQ){
        std::cout << "DMC FREQ Write detected with value: " << +(*value) << std::endl;
    }
    if(hardware_address == JOY2){
        std::cout << "JOY2 / APU FRAME CONTROL Write detected with value: " << +(*value) << std::endl;
    }
    if(hardware_address == OAMDMA){
        std::cout << "OAM DMA write detected with value: " << +(*value) << std::endl;
    }
    if(hardware_address == JOY1){
        std::cout << "JOY1 write detected with value: " << +(*value) << std::endl;
    }
    #endif

    // Detect Bank-switching for memory mappers
    // Right now just assuming UNROM mapper for donkey kong
    if (CPU::getInstance()->mapper == 3){
        if((hardware_address >= 0x8000) && (hardware_address <= 0xFFFF)){
            uint8_t bank_number = *value & 0x03;
            loadCHRROM(CPU::getInstance()->program, bank_number);
        }
    }

    memory[hardware_address] = *value;
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

// This is needed to link the ports of PPU to the "registers" in
// MMU
// If read() was used, which normally alerts the PPU of certain
// operations, it recursively can create PPU classes during 
// the PPU initialization. There's probably a nicer solution!
uint8_t *MMU::readPort(uint16_t *address){
     if(*address >= PPU_START && *address <= PPU_MIRRORS_END){
        return &memory[*address];
     }else{
        throw "readPort(): address is not from 0x2000 .. 0x2008";
     }
}

void MMU::loadPRGROM(std::vector<uint8_t> &program){
    // Load PRG-ROM into memory
    uint16_t prg_start = 16; //PRG-ROM starts at byte 16
    uint16_t program_length = 16384 * CPU::getInstance()->n_prgrom_banks;

    // 1 PRG-ROM -> bank 1 is copied to bank 2
    if(CPU::getInstance()->n_prgrom_banks == 1){
        for(uint16_t i = 0; i < program_length; i++){
            uint16_t address = PRG_ROM1_START + i;
            memory[address] = program[prg_start + i];
        }
        for(uint16_t i = 0; i < program_length; i++){
            uint16_t address = PRG_ROM2_START + i;
            memory[address] = program[prg_start + i];
        }
    }

    if(CPU::getInstance()->n_prgrom_banks == 2){
        for(uint16_t i = 0; i < program_length; i++){
            uint16_t address = PRG_ROM1_START + i;
            memory[address] = program[prg_start + i];
        }
    }
}

void MMU::loadCHRROM(std::vector<uint8_t> &program, uint8_t &bank_number){
    // Load CHR-ROM into PPU VRAM
    uint16_t chr_length = 8192;
    uint16_t header_offset = 16;
    uint16_t prg_offset = 16384 * CPU::getInstance()->n_prgrom_banks;
    uint16_t chr_bank_offset = (bank_number) * chr_length;
    uint16_t chr_start = header_offset + prg_offset + chr_bank_offset;
    for(uint16_t i = 0; i < chr_length; i++){
        uint16_t address = PATTERN_TABLE_START + i;
        PPU::getInstance()->write(&address, &program[chr_start + i]);
    }
}
