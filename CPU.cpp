#include "CPU.h"
#include "instructions.h"
#include "calltable.h"

#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <cstdint>
#include <stdexcept>

using namespace std;

CPU* CPU::instance = 0;

CPU::CPU(){
    // Initialize Registers / PC
    uint16_t PC = 0x0000;
    uint8_t A = 0x00;
    uint8_t X = 0x00;
    uint8_t Y = 0x00;
    uint8_t P = 0x00;
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
            result = P & 0x80;
            break;
        case V:  
            result = P & 0x40;
            break;
        case B:
            result = P & 0x10;
            break;
        case D: 
            result = P & 0x08;
            break;
        case I:
            result = P & 0x04;
            break;
        case Z:
            result = P & 0x02;
            break;
        case C:
            result = P & 0x01;
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
    PC = 0x0000;
}

void CPU::printRegisters(void){
    string status = bitset<8>(P).to_string();
    printf("A: %d X: %d Y: %d ", A, X, Y);
    cout << "P: " << status << "\n";
}

void CPU::runProgram(){
    uint8_t byte = program[PC];
    while(byte != 0x00){
        byte = program[PC];

        printf("\nOpcode: %d\n", byte);

        // Attempt to run byte as opcode
        opcodes[byte](); 

        printRegisters();

        // Increment PC by 1
        PC = PC + 1;
    }
}

// Retrieves the next byte while 
// simultaneously incrementing the
// Program Counter
uint8_t *CPU::nextByte(){
    uint8_t *returned_bytes;
    PC = PC + 1; // Increment PC by 1
    returned_bytes = &program[PC];

    return returned_bytes;
}

void CPU::loadProgram(string filename){
    // Read .nes program
    ifstream test_file;
    string raw_program;
    test_file.open(filename);

    // Extract program into a single string
    char c;
    while(test_file >> c){
        raw_program += c;
    }

    test_file.close();

    // Every 2 characters forms a byte
    int size = raw_program.length() / 2;
    for(int i = 0; i < raw_program.length(); i+=2){
        string byte = "";
        byte += raw_program[i];
        byte += raw_program[i+1];
        
        // Convert hex byte string into int representation
        // and store in contiguous blocks of memory
        program[i / 2] = strtoul(byte.c_str(), NULL, 16);
    }
}

