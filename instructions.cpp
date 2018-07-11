#include "instructions.h"
#include "CPU.h"

#include <string>
#include <bitset>
#include <iostream>

using namespace std;

// Add with Carry
void ADC(uint8_t value){

    uint8_t prior_A = A;
    A = A + value + getStatusBit(C);

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
    if (((prior_A ^ A) & (value ^ A) & 0x80) != 0x00){
        setStatusBit(V);
    }else{
        clearStatusBit(V);
    }

    // Carry flag
    if ((prior_A + value + getStatusBit(C)) > 0xff){
        setStatusBit(C);
    }else{
        clearStatusBit(C);
    }
}

void AND(uint8_t value){

    uint8_t prior_A = A;
    A = A & value;

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
}

void ASL(){

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
}

void NOP(){
    return; // Need to add cycle burn
}