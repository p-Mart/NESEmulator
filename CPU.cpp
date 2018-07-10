#include "CPU.h"
#include "instructions.h"

#include <iostream>
#include <bitset>
#include <string>

using namespace std;

uint8_t getStatusBit(uint8_t bit){
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

void setStatusBit(uint8_t bit){
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

void clearStatusBit(uint8_t bit){
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

void initializeRegisters(void){
    A = X = Y = P = 0x00;
    PC = 0x0000;
}

void printRegisters(void){
    string status = bitset<8>(P).to_string();
    printf("A: %d X: %d Y: %d ", A, X, Y);
    cout << "P: " << status << "\n";
}


// 16-bit program counter
uint16_t PC = 0x0000;

/* 8-bit registers. They are:
A - Accumulator register
X - Index Register X
Y - Index Register Y
P - Processor Status */
uint8_t A = 0x00;
uint8_t X = 0x00;
uint8_t Y = 0x00;
uint8_t P = 0x00;

/*
int main(void){

    //initializeRegisters();
    printRegisters();

    uint8_t meme; 

    meme = 0xFF;
    ADC(meme);
    printRegisters();

    meme = 0x01;
    ADC(meme);
    printRegisters();

    return 0;
}
*/