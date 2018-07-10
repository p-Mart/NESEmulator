#ifndef CPU_H
#define CPU_H

#include <cstdint>

// 16-bit program counter
extern uint16_t PC;

/* 8-bit registers. They are:
A - Accumulator register
X - Index Register X
Y - Index Register Y
P - Processor Status */
extern uint8_t A;
extern uint8_t X;
extern uint8_t Y;
extern uint8_t P;

// P bit references
// [N, V, [ ], B, D, I, Z, C]
const short int N = 0;
const short int V = 1;
const short int B = 2;
const short int D = 3;
const short int I = 4;
const short int Z = 5;
const short int C = 6;

void initializeRegisters();

uint8_t getStatusBit(uint8_t bit);
void setStatusBit(uint8_t bit);
void clearStatusBit(uint8_t bit);

#endif