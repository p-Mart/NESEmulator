#include "instructions.h"
#include "CPU.h"

#include <string>
#include <bitset>
#include <iostream>

using namespace std;

// Add with Carry
uint8_t ADC(uint8_t M){

    uint8_t prior_A = A;
    A = A + M + getStatusBit(C);

    // Set processor status
    
    // Negative flag
    if ((A & 0x80) == 0x80){
        setStatusBit(N);
    }else{
        clearStatusBit(N);
    }
    
    // Zero flag
    if (A == 0){
        setStatusBit(Z);
    }else{
        clearStatusBit(Z);
    }

    // Overflow flag
    // http://www.righto.com/2012/12/the-6502-overflow-flag-explained.html
    if (((prior_A ^ A) & (M ^ A) & 0x80) != 0x00){
        setStatusBit(V);
    }else{
        clearStatusBit(V);
    }

    // Carry flag
    if ((prior_A + M + getStatusBit(C)) > 0xff){
        setStatusBit(C);
    }else{
        clearStatusBit(C);
    }

    return A;
}

uint8_t AND(uint8_t M){

    uint8_t prior_A = A;
    A = A & M;

    // Set processor status
    
    // Negative flag
    if ((A & 0x80) == 0x80){
        setStatusBit(N);
    }else{
        clearStatusBit(N);
    }
    
    // Zero flag
    if (A == 0){
        setStatusBit(Z);
    }else{
        clearStatusBit(Z);
    }

    return A;
}

uint8_t ASL(uint8_t M){

    uint8_t prior_A = A;
    A = A << 1;

    // Set processor status
    
    // Negative flag
    if ((A & 0x80) == 0x80){
        setStatusBit(N);
    }else{
        clearStatusBit(N);
    }
    
    // Zero flag
    if (A == 0){
        setStatusBit(Z);
    }else{
        clearStatusBit(Z);
    }

    // Carry flag
    if ((prior_A & 0x80) == 0x80){
        setStatusBit(C);
    }else{
        clearStatusBit(C);
    }

    return A;
}