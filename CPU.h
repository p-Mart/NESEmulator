#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <string>
// P bit references
// [N, V, [ ], B, D, I, Z, C]
const short int N = 0;
const short int V = 1;
const short int B = 2;
const short int D = 3;
const short int I = 4;
const short int Z = 5;
const short int C = 6;



class CPU{

    static CPU *instance;
    CPU();

public:

    uint8_t program[2048];

    // 16-bit program counter
    uint16_t PC;

    /* 8-bit registers. They are:
    A - Accumulator register
    X - Index Register X
    Y - Index Register Y
    P - Processor Status */
    uint8_t A;
    uint8_t X;
    uint8_t Y;
    uint8_t P;

    static CPU* getInstance();
    void clearRegisters();
    uint8_t getStatusBit(uint8_t);
    uint8_t *nextByte();
    void setStatusBit(uint8_t);
    void clearStatusBit(uint8_t);
    void printRegisters();
    void runProgram();
    void loadProgram(std::string);
};



#endif