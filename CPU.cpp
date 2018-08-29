#include "CPU.h"
#include "MMU.h"
#include "PPU.h"
#include "instructions.h"
#include "calltable.h"

#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <cstdint>
#include <stdexcept>
#include <assert.h>
#include <ctime>

using namespace std;

CPU* CPU::instance = 0;

CPU::CPU(){
    // Initialize Registers / PC / SP
    PC = 0x0000;
    SP = 0xFF;
    A = 0x00;
    X = 0x00;
    Y = 0x00;
    P = 0x00;

    mapper = 0;
    // Initialize PPU (move to NES class?)
    PPU::getInstance();
}

CPU* CPU::getInstance(){

    if (instance == 0){
        instance = new CPU();
    }

    return instance;
}

uint8_t CPU::getStatusBit(uint8_t bit){
    uint8_t result;

    switch (bit){
        case N: 
            result = (P & 0x80) >> 7;
            break;
        case V:  
            result = (P & 0x40) >> 6;
            break;
        case B:
            result = (P & 0x10) >> 4;
            break;
        case D: 
            result = (P & 0x08) >> 3;
            break;
        case I:
            result = (P & 0x04) >> 2;
            break;
        case Z:
            result = (P & 0x02) >> 1;
            break;
        case C:
            result = (P & 0x01);
            break;
    }

    return result;
}

void CPU::setStatusBit(uint8_t bit){
    switch (bit){
        case N: 
            P = P | 0x80;
            break;
        case V:  
            P = P | 0x40;
            break;
        case B:
            P = P | 0x10;
            break;
        case D: 
            P = P | 0x08;
            break;
        case I:
            P = P | 0x04;
            break;
        case Z:
            P = P | 0x02;
            break;
        case C:
            P = P | 0x01;
            break;
    }
}

void CPU::clearStatusBit(uint8_t bit){
    switch (bit){
        case N:
            P = P & 0x7F;
            break;
        case V:  
            P = P & 0xBF;
            break;
        case B:
            P = P & 0xEF;
            break;
        case D: 
            P = P & 0xF7;
            break;
        case I:
            P = P & 0xFB;
            break;
        case Z:
            P = P & 0xFD;
            break;
        case C:
            P = P & 0xFE;
            break;
    }
}

void CPU::clearRegisters(void){
    A = X = Y = P = 0x00;
    SP = 0xFF;
    PC = 0x0000;
}

void CPU::printRegisters(void){
    string status = bitset<8>(P).to_string();
    printf("A: %d X: %d Y: %d ", A, X, Y);
    std::cout << "P: " << status << "\n";
    printf("PC: %04X SP: %04X\n", PC, SP);
}

void CPU::runProgram(){
    uint8_t byte = *MMU::getInstance()->read(&PC);
    int cycles = 0;
    while(byte != 0x00){
        byte = *MMU::getInstance()->read(&PC);
        
        if((cycles > 300000) && (cycles < 400000)){
            printf("\nOpcode: %02X\n", byte);
            printf("Cycle: %d\n", cycles);
        }
        // Attempt to run byte as opcode
        opcodes[byte](); 

        if((cycles > 300000) && (cycles < 400000)){
            printRegisters();
        }

        // Increment PC by 1
        PC = PC + 1;
        cycles += 1;

        // PPU tick
        // 3 CPU Clocks = 1 PPU Clock
        for (int i = 0; i < 3; i++){
            PPU::getInstance()->renderFrame(); 
        }
    }

    #ifndef NDEBUG
        MMU::getInstance()->memDump();
        PPU::getInstance()->memDump();
    #endif
}

// Retrieves the next byte while 
// simultaneously incrementing the
// Program Counter
uint8_t *CPU::nextByte(){
    uint8_t *returned_byte;
    PC = PC + 1; // Increment PC by 1
    returned_byte = MMU::getInstance()->read(&PC);

    return returned_byte;
}

void CPU::loadProgram(string filename){

    // Read .nes program
    ifstream test_file;
    test_file.open(filename);

    // Extract program into a single string
    char c;
    int i = 0;
    while(test_file.get(c)){
        program.push_back(c);
    }
    test_file.close();

    //// Process 16 Byte header
    // For the .ines standard, the first 4 bytes
    // MUST be the following
    assert(program[0] == 0x4E);
    assert(program[1] == 0x45);
    assert(program[2] == 0x53);
    assert(program[3] == 0x1A);

    // Number of 16KB PRG-ROM banks
    n_prgrom_banks = program[4];

    // Number of 8KB CHR-ROM banks
    n_chrrom_banks = program[5];

    // First ROM Control Byte
    control_one = program[6];

    // Second ROM Control Byte
    control_two = program[7];

    // Number of 8KB RAM Banks
    n_ram_banks = program[8];

    // Bytes 9 - 15 should be 0x00

    // Get Memory Mapper
    mapper = control_one >> 4;
    mapper += (control_two >> 4) * 256;

    // Load PRG-ROM into memory
    MMU::getInstance()->loadPRGROM(program);
    /*
    uint16_t prg_start = 16; //PRG-ROM starts at byte 16
    uint16_t program_length = 16384 * n_prgrom_banks;

    // 1 PRG-ROM -> bank 1 is copied to bank 2
    if(n_prgrom_banks == 1){
        for(uint16_t i = 0; i < program_length; i++){
            uint16_t address = PRG_ROM1_START + i;
            MMU::getInstance()->write(&address, &program[prg_start + i]);
        }
        for(uint16_t i = 0; i < program_length; i++){
            uint16_t address = PRG_ROM2_START + i;
            MMU::getInstance()->write(&address, &program[prg_start + i]);
        }
    }

    if(n_prgrom_banks == 2){
        for(uint16_t i = 0; i < program_length; i++){
            uint16_t address = PRG_ROM1_START + i;
            MMU::getInstance()->write(&address, &program[prg_start + i]);
        }
    }
    */
    // Load CHR-ROM (Bank 1) into PPU memory
    /*
    uint16_t chr_start = prg_start + program_length;
    uint16_t chr_length = 8192;
    for(uint16_t i = 0; i < chr_length; i++){
        uint16_t address = PATTERN_TABLE_START + i;
        PPU::getInstance()->write(&address, &program[chr_start + i]);
    }
    */
    uint8_t bank_number = 0;
    MMU::getInstance()->loadCHRROM(program, bank_number);

    // Get Interrupt Vectors
    // RESET vector must be at 0xFFFC - 0xFFFD
    uint16_t reset_vector = 0;
    uint16_t reset_low = 0xFFFC;
    uint16_t reset_high = 0xFFFD;

    reset_vector += *MMU::getInstance()->read(&reset_low);
    reset_vector += *MMU::getInstance()->read(&reset_high) * 16 * 16;

    PC = reset_vector;

    #ifndef NDEBUG
        MMU::getInstance()->memDump();
        PPU::getInstance()->memDump();
    #endif
}

uint8_t *CPU::popStack(){
    SP = SP + 1;
    uint8_t *value = MMU::getInstance()->readStack(&SP);
}

void CPU::pushStack(uint8_t *value){
    MMU::getInstance()->writeStack(&SP, value);
    SP = SP - 1;
}
void CPU::pushStack(uint16_t *value){
    uint8_t lsb = 0;
    uint8_t msb = 0;
    lsb += (*value & 0x00FF);
    msb += ((*value & 0xFF00 ) >> 8);
    
    MMU::getInstance()->writeStack(&SP, &msb);
    SP = SP - 1;

    MMU::getInstance()->writeStack(&SP, &lsb);
    SP = SP - 1;
}

void CPU::interruptNMI(){

    #ifndef NDEBUG
        printf("NMI Interrupt has occurred\n");
    #endif

    // Push current PC and processor status onto stack, and set the I flag
    pushStack(&PC);
    pushStack(&P);
    SEI();

    uint16_t nmi_lo = 0xFFFA;
    uint16_t nmi_hi = 0xFFFB;

    // Load address of interrupt handling routine into PC
    PC = 0;
    PC += *MMU::getInstance()->read(&nmi_lo);
    PC += *MMU::getInstance()->read(&nmi_hi) *  256;
}