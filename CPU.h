#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <string>
#include <vector>

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

    std::vector<uint8_t> program;

    // 16-bit program counter
    uint16_t PC;

    // Stack pointer, initialized at 0x01FF
    // Note that SP is only 1 Byte, and points
    // to the top of the 0x01XX page
    uint8_t SP;

    /* 8-bit registers. They are:
    A - Accumulator register
    X - Index Register X
    Y - Index Register Y
    P - Processor Status */
    uint8_t A;
    uint8_t X;
    uint8_t Y;
    uint8_t P;

    // Program info
    uint8_t n_prgrom_banks;
    uint8_t n_chrrom_banks;
    uint8_t control_one;
    uint8_t control_two;
    uint8_t n_ram_banks;
    int mapper;

    // Cycle related information
    unsigned int cycle_delay; // Delay counter to emulate cycle stalling
    uint8_t cycle_parity; // 0 = even, 1 = odd. arithmetic in mod 2

    static CPU* getInstance();
    void clearRegisters();
    uint8_t getStatusBit(uint8_t);
    int getMapper();
    uint8_t *nextByte();
    void setStatusBit(uint8_t);
    void clearStatusBit(uint8_t);
    void printRegisters();
    void runProgram();
    void loadProgram(std::string);

    void interruptNMI();
    void delay(unsigned int);

    void pushStack(uint8_t*);
    void pushStack(uint16_t*);

    uint8_t *popStack();
};

#endif