#include "instructions.h"
#include "CPU.h"
#include "MMU.h"

#include <string>
#include <bitset>
#include <iostream>

using namespace std;

CPU* cpu = CPU::getInstance();
MMU* mmu = MMU::getInstance();

/*
// Add with Carry
void ADC(){

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

void AND(){

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
*/



// ADC - Add with Carry

// Base behavior for ADC instruction
void ADC(uint8_t *value){
    uint8_t prior_A = cpu->A;
    cpu->A = cpu->A + *value + cpu->getStatusBit(C);

    // Set processor status
    // Negative flag
    if ((cpu->A & 0x80) == 0x80){
        cpu->setStatusBit(N);
    }else{
        cpu->clearStatusBit(N);
    }
    
    // Zero flag
    if (cpu->A == 0){
        cpu->setStatusBit(Z);
    }else{
        cpu->clearStatusBit(Z);
    }

    // Overflow flag
    // http://www.righto.com/2012/12/the-6502-overflow-flag-explained.html
    if (((prior_A ^ cpu->A) & (*value ^ cpu->A) & 0x80) != 0x00){
        cpu->setStatusBit(V);
    }else{
        cpu->clearStatusBit(V);
    }

    // Carry flag
    if ((prior_A + *value + cpu->getStatusBit(C)) > 0xff){
        cpu->setStatusBit(C);
    }else{
        cpu->clearStatusBit(C);
    }
}
void ADC_I(){
    uint8_t *value;
    value = cpu->nextByte();

    ADC(value);
}
void ADC_Z(){
    uint8_t address;
    uint8_t *value;
    address = *cpu->nextByte();
    value = mmu->read(&address);

    ADC(value);
}
void ADC_ZX(){
    uint8_t address = 0;
    uint8_t *value;
    address += *cpu->nextByte();
    address += cpu->X;
    value = mmu->read(&address);

    ADC(value);
}
void ADC_A(){

    uint16_t address = 0;
    uint8_t *value;

    address += *cpu->nextByte();
    address += *cpu->nextByte() * 16;
    value = mmu->read(&address);

    ADC(value);
}
void ADC_AX(){
    uint16_t address = 0;
    uint8_t *value;
    address += *cpu->nextByte();
    address += *cpu->nextByte() * 16;
    address += cpu->X;
    value = mmu->read(&address);

    ADC(value);
}
void ADC_AY(){
    uint16_t address = 0;
    uint8_t *value;
    address += *cpu->nextByte();
    address += *cpu->nextByte() * 16;
    address += cpu->Y;
    value = mmu->read(&address);

    ADC(value);
}
void ADC_IX(){
    uint8_t indirect_address = 0;
    uint16_t target_address = 0;
    uint8_t *value;

    indirect_address += *cpu->nextByte();
    indirect_address += cpu->X;
    target_address += *mmu->read(&indirect_address);
    indirect_address++;
    target_address += *mmu->read(&indirect_address) * 16;
    value = mmu->read(&target_address);

    ADC(value);
}
void ADC_IY(){
    uint8_t indirect_address = 0;
    uint16_t target_address = 0;
    uint8_t *value;

    indirect_address += *cpu->nextByte();
    target_address += *mmu->read(&indirect_address);
    indirect_address++;
    target_address += *mmu->read(&indirect_address) * 16;
    target_address += cpu->Y;
    value = mmu->read(&target_address);

    ADC(value);
}

// AND - Logical AND
void AND(){}
void AND_I(){}
void AND_Z(){}
void AND_ZX(){}
void AND_A(){}
void AND_AX(){}
void AND_AY(){}
void AND_IX(){}
void AND_IY(){}

// ASL - Arithmetic Shift Left
void ASL(){

    uint8_t prior_A = cpu->A;
    cpu->A = cpu->A << 1;

    // Set processor status
    // Negative flag
    if ((cpu->A & 0x80) == 0x80){
        cpu->setStatusBit(N);
    }else{
        cpu->clearStatusBit(N);
    }
    
    // Zero flag
    if (cpu->A == 0){
        cpu->setStatusBit(Z);
    }else{
        cpu->clearStatusBit(Z);
    }

    // Carry flag
    if ((prior_A & 0x80) == 0x80){
        cpu->setStatusBit(C);
    }else{
        cpu->clearStatusBit(C);
    }
}

void ASL_Z(){}
void ASL_ZX(){}
void ASL_A(){}
void ASL_AX(){}

void BRANCH(int8_t* offset){
    cpu->PC = cpu->PC + *offset;
}

// BCC - Branch if Carry Clear
void BCC(){

    int8_t offset = 0;
    offset += *cpu->nextByte();
    
    if(cpu->getStatusBit(C) == 0){
        BRANCH(&offset);
    }
}

// BCS - Branch if Carry Set
void BCS(){

    int8_t offset = 0;
    offset += *cpu->nextByte();
    
    if(cpu->getStatusBit(C) == 1){
        BRANCH(&offset);
    }
}

// BEQ - Branch if Equal
void BEQ(){

    int8_t offset = 0;
    offset += *cpu->nextByte();
    
    if(cpu->getStatusBit(Z) == 1){
        BRANCH(&offset);
    }
}

// BIT - Bit Test
void BIT(){}
void BIT_Z(){}
void BIT_A(){}

// BMI - Branch if Minus
void BMI(){

    int8_t offset = 0;
    offset += *cpu->nextByte();
    
    if(cpu->getStatusBit(N) == 1){
        BRANCH(&offset);
    }
}

// BNE - Branch if Not Equal
void BNE(){

    int8_t offset = 0;
    offset += *cpu->nextByte();
    
    if(cpu->getStatusBit(Z) == 0){
        BRANCH(&offset);
    }
}

// BPL - Branch if Positive
void BPL(){

    int8_t offset = 0;
    offset += *cpu->nextByte();
    
    if(cpu->getStatusBit(N) == 0){
        BRANCH(&offset);
    }
}

// BRK - Force Interrupt
void BRK(){}

// BVC - Branch if Overflow Clear
void BVC(){

    int8_t offset = 0;
    offset += *cpu->nextByte();
    
    if(cpu->getStatusBit(V) == 0){
        BRANCH(&offset);
    }
}

// BVS - Branch if Overflow Set
void BVS(){

    int8_t offset = 0;
    offset += *cpu->nextByte();
    
    if(cpu->getStatusBit(V) == 1){
        BRANCH(&offset);
    }
}

// CLC - Clear Carry Flag
void CLC(){
    cpu->clearStatusBit(C);
}

// CLD - Clear Decimal Mode
void CLD(){
    cpu->clearStatusBit(D);
}

// CLI - Clear Interrupt Disable
void CLI(){
    cpu->clearStatusBit(I);
}

// CLV - Clear Overflow Flag
void CLV(){
    cpu->clearStatusBit(V);
}

// CMP - Compare
void CMP(){}
void CMP_I(){}
void CMP_Z(){}
void CMP_ZX(){}
void CMP_A(){}
void CMP_AX(){}
void CMP_AY(){}
void CMP_IX(){}
void CMP_IY(){}

// CPX - Compare X Register
void CPX(){}
void CPX_I(){}
void CPX_Z(){}
void CPX_A(){}

// CPY - Compare Y Register
void CPY(){}
void CPY_I(){}
void CPY_Z(){}
void CPY_A(){}

// DEC - Decrement Memory
void DEC(){}
void DEC_Z(){}
void DEC_ZX(){}
void DEC_A(){}
void DEC_AX(){}

// DEX - Decrement X Register
void DEX(){}

// DEX - Decrement Y Register
void DEY(){}

// EOR - Exclusive OR
void EOR(){}
void EOR_I(){}
void EOR_Z(){}
void EOR_ZX(){}
void EOR_A(){}
void EOR_AX(){}
void EOR_AY(){}
void EOR_IX(){}
void EOR_IY(){}

// INC - Increment Memory
void INC(){}
void INC_Z(){}
void INC_ZX(){}
void INC_A(){}
void INC_AX(){}

// INX - Increment X Register
void INX(){}

// INY - Increment Y Register
void INY(){}

// JMP - Jump
void JMP(){}
void JMP_A(){}
void JMP_I(){} // NOTE: This is for INDIRECT addressing (only JMP does this)

// JSR - Jump to Subroutine
void JSR(){} // Absolute addressing only

// LDA - Load Accumulator
void LDA(uint8_t *value){
    cpu->A = *value;
}
void LDA_I(){
    uint8_t *value = cpu->nextByte();
    LDA(value);
}
void LDA_Z(){
    uint8_t *address = cpu->nextByte();
    uint8_t *value = mmu->read(address);
    LDA(value);
}
void LDA_ZX(){}
void LDA_A(){

    uint16_t address = 0;
    address += *cpu->nextByte();
    address += *cpu->nextByte() * 16;

    uint8_t *value = mmu->read(&address);
    LDA(value);
}
void LDA_AX(){}
void LDA_AY(){}
void LDA_IX(){}
void LDA_IY(){}

// LDX - Load X Register
void LDX(){}
void LDX_I(){}
void LDX_Z(){}
void LDX_ZY(){} 
void LDX_A(){}
void LDX_AY(){}

// LDY - Load Y Register
void LDY(){}
void LDY_I(){}
void LDY_Z(){}
void LDY_ZX(){} 
void LDY_A(){}
void LDY_AX(){}

// LSR - Logical Shift Right
void LSR(){}
void LSR_Z(){}
void LSR_ZX(){}
void LSR_A(){}
void LSR_AX(){}

// NOP - No Operation
void NOP(){}

// ORA - Logical Inclusive OR
void ORA(){}
void ORA_I(){}
void ORA_Z(){}
void ORA_ZX(){}
void ORA_A(){}
void ORA_AX(){}
void ORA_AY(){}
void ORA_IX(){}
void ORA_IY(){}

// PHA - Push Accumulator
void PHA(){}

// PHP - Push Processor Status
void PHP(){}

// PLA - Pull Accumulator
void PLA(){}

// PLP - Pull Processor Status
void PLP(){}

// ROL - Rotate Left
void ROL(){}
void ROL_Z(){}
void ROL_ZX(){}
void ROL_A(){}
void ROL_AX(){}

// ROR - Rotate Right
void ROR(){}
void ROR_Z(){}
void ROR_ZX(){}
void ROR_A(){}
void ROR_AX(){}

// RTI - Return from Interrupt
void RTI(){}

// RTS - Return from Subroutine
void RTS(){}

// SBC - Subtract with Carry
void SBC(){}
void SBC_I(){}
void SBC_Z(){}
void SBC_ZX(){}
void SBC_A(){}
void SBC_AX(){}
void SBC_AY(){}
void SBC_IX(){}
void SBC_IY(){}

// SEC - Set Carry Flag
void SEC(){
    cpu->setStatusBit(C);
}

// SED - Set Decimal Flag
void SED(){
    cpu->setStatusBit(D);
}

// SEI - Set Interrupt Disable
void SEI(){
    cpu->setStatusBit(I);
}

// STA - Store Accumulator
void STA(uint16_t *address){
    mmu->write(address, &cpu->A);
}

void STA_Z(){
    uint16_t *value;
    *value += *cpu->nextByte();
    STA(value);
}

void STA_ZX(){}
void STA_A(){
    uint16_t value = 0;
    value += *cpu->nextByte();
    value += *cpu->nextByte() * 16;
    STA(&value);
}

void STA_AX(){}
void STA_AY(){}
void STA_IX(){}
void STA_IY(){}

// STX - Store X Register
void STX(){}
void STX_Z(){}
void STX_ZY(){}
void STX_A(){}

// STY - Store Y Register
void STY(){}
void STY_Z(){}
void STY_ZX(){}
void STY_A(){}

// TAX - Transfer Accumulator to X
void TAX(){}

// TAY - Transfer Accumulator to Y
void TAY(){}

// TSX - Transfer Stack Pointer to X
void TSX(){}

// TXA - Transfer X to Accumulator
void TXA(){}

// TXS - Transfer X to Stack Pointer
void TXS(){}

// TYA - Transfer Y to Accumulator
void TYA(){}