#include "instructions.h"
#include "CPU.h"
#include "MMU.h"

#include <string>
#include <bitset>
#include <iostream>
#include <functional>

using namespace std;

// Addressing mode wrappers

// Immediate addressing - take a 1 Byte constant
// at the next byte in the program and pass that
// to the calling instruction
void immediate(std::function<void (uint8_t*)> f){
    uint8_t* value = CPU::getInstance()->nextByte();
    f(value);
}

// Zero Page addressing - take a 1 Byte address
// and read from zero page memory (0x0000 - 0x00FF),
// then passing that value to the calling instruction
void zeropage(std::function<void (uint8_t*)> f){
    uint8_t address;
    uint8_t *value;
    address = *CPU::getInstance()->nextByte();
    value = MMU::getInstance()->read(&address);

    f(value);
}

// Zero Page, X - take a 1 Byte address, then add
// the current value of the X register to it. This value
// is used as an address into zero page memory, the
// read of which address is passed to the calling function
void zeropageX(std::function<void (uint8_t*)> f){
    uint8_t address = 0;
    uint8_t* value;
    address += *CPU::getInstance()->nextByte();
    address += CPU::getInstance()->X;
    value = MMU::getInstance()->read(&address);

    f(value);
}

void zeropageY(std::function<void (uint8_t*)> f){
    uint8_t address = 0;
    uint8_t* value;
    address += *CPU::getInstance()->nextByte();
    address += CPU::getInstance()->Y;
    value = MMU::getInstance()->read(&address);

    f(value);
}

// Absolute address - take a 2 Byte address
// and pass this to the calling function
void absolute(std::function<void (uint8_t*)> f){
    uint16_t address = 0;
    uint8_t* value;

    address += *CPU::getInstance()->nextByte();
    address += *CPU::getInstance()->nextByte() * 16 * 16;
    value = MMU::getInstance()->read(&address);

    f(value);
}

void absoluteX(std::function<void (uint8_t*)> f){
    uint16_t address = 0;
    uint8_t *value;
    address += *CPU::getInstance()->nextByte();
    address += *CPU::getInstance()->nextByte() * 16 * 16;
    address += CPU::getInstance()->X;
    value = MMU::getInstance()->read(&address);

    f(value);
}

void absoluteY(std::function<void (uint8_t*)> f){
    uint16_t address = 0;
    uint8_t *value;
    address += *CPU::getInstance()->nextByte();
    address += *CPU::getInstance()->nextByte() * 16 * 16;
    address += CPU::getInstance()->Y;
    value = MMU::getInstance()->read(&address);

    f(value);
}

void indirectX(std::function<void (uint8_t*)> f){
    uint8_t indirect_address = 0;
    uint16_t target_address = 0;
    uint8_t *value;

    indirect_address += *CPU::getInstance()->nextByte();
    indirect_address += CPU::getInstance()->X;
    target_address += *MMU::getInstance()->read(&indirect_address);
    indirect_address++;
    target_address += *MMU::getInstance()->read(&indirect_address) * 16 * 16;
    value = MMU::getInstance()->read(&target_address);

    f(value);
}

void indirectY(std::function<void (uint8_t*)> f){
    uint8_t indirect_address = 0;
    uint16_t target_address = 0;
    uint8_t *value;

    indirect_address += *CPU::getInstance()->nextByte();
    target_address += *MMU::getInstance()->read(&indirect_address);
    indirect_address++;
    target_address += *MMU::getInstance()->read(&indirect_address) * 16 * 16;
    target_address += CPU::getInstance()->Y;
    value = MMU::getInstance()->read(&target_address);

    f(value);
}

// ADC - Add with Carry
// Base behavior for ADC instruction
void ADC(uint8_t *value){
    uint8_t prior_A = CPU::getInstance()->A;
    CPU::getInstance()->A = CPU::getInstance()->A + *value + CPU::getInstance()->getStatusBit(C);
    
    // Set processor status
    // Negative flag
    if ((CPU::getInstance()->A & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
    
    // Zero flag
    if (CPU::getInstance()->A == 0){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }

    // Overflow flag
    // http://www.righto.com/2012/12/the-6502-overflow-flag-explained.html
    if (((prior_A ^ CPU::getInstance()->A) & (*value ^ CPU::getInstance()->A) & 0x80) != 0x00){
        CPU::getInstance()->setStatusBit(V);
    }else{
        CPU::getInstance()->clearStatusBit(V);
    }

    // Carry flag
    if ((prior_A + *value + CPU::getInstance()->getStatusBit(C)) > 0xff){
        CPU::getInstance()->setStatusBit(C);
    }else{
        CPU::getInstance()->clearStatusBit(C);
    }
}
void ADC_I(){
    immediate(ADC);
}
void ADC_Z(){
    zeropage(ADC);
}
void ADC_ZX(){
    zeropageX(ADC);
}
void ADC_A(){
    absolute(ADC);
}
void ADC_AX(){
    absoluteX(ADC);
}
void ADC_AY(){
    absoluteY(ADC);
}
void ADC_IX(){
    indirectX(ADC);
}
void ADC_IY(){
    indirectY(ADC);
}

// AND - Logical AND
void AND(uint8_t *value){
    CPU::getInstance()->A = CPU::getInstance()->A & *value;
    
    // Set processor status
    // Negative flag
    if ((CPU::getInstance()->A & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
    
    // Zero flag
    if (CPU::getInstance()->A == 0){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }
}
void AND_I(){
    immediate(AND);
}
void AND_Z(){
    zeropage(AND);
}
void AND_ZX(){
    zeropageX(AND);
}
void AND_A(){
    absolute(AND);
}
void AND_AX(){
    absoluteX(AND);
}
void AND_AY(){
    absoluteY(AND);
}
void AND_IX(){
    indirectX(AND);
}
void AND_IY(){
    indirectY(AND);
}

// ASL - Arithmetic Shift Left
void ASL_BASE(uint8_t* value){
    uint8_t prior_value = *value;
    *value <<= 1;
    
    // Set processor status
    // Negative flag
    if ((*value & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
    
    // Zero flag
    if (CPU::getInstance()->A == 0){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }

    // Carry flag
    if ((prior_value & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(C);
    }else{
        CPU::getInstance()->clearStatusBit(C);
    }
}
void ASL(){
    ASL_BASE(&(CPU::getInstance()->A));
}
void ASL_Z(){
    zeropage(ASL_BASE);
}
void ASL_ZX(){
    zeropageX(ASL_BASE);
}
void ASL_A(){
    absolute(ASL_BASE);
}
void ASL_AX(){
    absoluteX(ASL_BASE);
}

// Base branch functionality (relative addressing)
void BRANCH(int8_t* offset){
    CPU::getInstance()->PC = CPU::getInstance()->PC + *offset;
}

// BCC - Branch if Carry Clear
void BCC(){

    int8_t offset = 0;
    offset += *CPU::getInstance()->nextByte();
    
    if(CPU::getInstance()->getStatusBit(C) == 0){
        BRANCH(&offset);
    }
}

// BCS - Branch if Carry Set
void BCS(){

    int8_t offset = 0;
    offset += *CPU::getInstance()->nextByte();
    
    if(CPU::getInstance()->getStatusBit(C) == 1){
        BRANCH(&offset);
    }
}

// BEQ - Branch if Equal
void BEQ(){

    int8_t offset = 0;
    offset += *CPU::getInstance()->nextByte();
    
    if(CPU::getInstance()->getStatusBit(Z) == 1){
        BRANCH(&offset);
    }
}

// BIT - Bit Test
void BIT(uint8_t* value){
    // AND the A register with a value in memory
    // but dont store the result -- just set the zero
    // flag.
    uint8_t result = *value & CPU::getInstance()->A;

    if (result == 0x00){
        CPU::getInstance()->setStatusBit(Z);
    }

    // Bit 7 and 6 of P are set to the contents
    // of the memory value.
    if ((*value & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }

    if ((*value & 0x40) == 0x40){
        CPU::getInstance()->setStatusBit(V);
    }else{
        CPU::getInstance()->clearStatusBit(V);
    }
}
void BIT_Z(){
    zeropage(BIT);
}
void BIT_A(){
    absolute(BIT);
}

// BMI - Branch if Minus
void BMI(){

    int8_t offset = 0;
    offset += *CPU::getInstance()->nextByte();
    
    if(CPU::getInstance()->getStatusBit(N) == 1){
        BRANCH(&offset);
    }
}

// BNE - Branch if Not Equal
void BNE(){

    int8_t offset = 0;
    offset += *CPU::getInstance()->nextByte();
    
    if(CPU::getInstance()->getStatusBit(Z) == 0){
        BRANCH(&offset);
    }
}

// BPL - Branch if Positive
void BPL(){
    int meme = 0;
    int8_t offset = 0;
    offset += *CPU::getInstance()->nextByte();
    meme += offset;
    std::cout << meme << std::endl;
    
    if(CPU::getInstance()->getStatusBit(N) == 0){
        BRANCH(&offset);
    }
}

// BRK - Force Interrupt
void BRK(){
    // Need to push PC and P onto stack
    // Need to load the IRQ interrupt vector (0xFFFE - 0XFFFF)
    //      into PC

    // 7 cycle burn
    CPU::getInstance()->setStatusBit(B);
}

// BVC - Branch if Overflow Clear
void BVC(){

    int8_t offset = 0;
    offset += *CPU::getInstance()->nextByte();
    
    if(CPU::getInstance()->getStatusBit(V) == 0){
        BRANCH(&offset);
    }
}

// BVS - Branch if Overflow Set
void BVS(){

    int8_t offset = 0;
    offset += *CPU::getInstance()->nextByte();
    
    if(CPU::getInstance()->getStatusBit(V) == 1){
        BRANCH(&offset);
    }
}

// CLC - Clear Carry Flag
void CLC(){
    CPU::getInstance()->clearStatusBit(C);
}

// CLD - Clear Decimal Mode
void CLD(){
    CPU::getInstance()->clearStatusBit(D);
}

// CLI - Clear Interrupt Disable
void CLI(){
    CPU::getInstance()->clearStatusBit(I);
}

// CLV - Clear Overflow Flag
void CLV(){
    CPU::getInstance()->clearStatusBit(V);
}

// CMP - Compare
void CMP(uint8_t* value){
    uint8_t result = (CPU::getInstance()->A == *value);

    if (result == 0x00){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }

    if (CPU::getInstance()->A >= *value){
        CPU::getInstance()->setStatusBit(C);
    }else{
        CPU::getInstance()->clearStatusBit(C);
    }

    if ((result & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
}
void CMP_I(){
    immediate(CMP);
}
void CMP_Z(){
    zeropage(CMP);
}
void CMP_ZX(){
    zeropageX(CMP);
}
void CMP_A(){
    absolute(CMP);
}
void CMP_AX(){
    absoluteX(CMP);
}
void CMP_AY(){
    absoluteY(CMP);
}
void CMP_IX(){
    indirectX(CMP);
}
void CMP_IY(){
    indirectY(CMP);
}

// CPX - Compare X Register
void CPX(uint8_t* value){
    
    uint8_t result = (CPU::getInstance()->X == *value);

    if (result == 0x00){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }

    if (CPU::getInstance()->X >= *value){
        CPU::getInstance()->setStatusBit(C);
    }else{
        CPU::getInstance()->clearStatusBit(C);
    }

    if ((result & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
}
void CPX_I(){
    immediate(CPX);
}
void CPX_Z(){
    zeropage(CPX);
}
void CPX_A(){
    absolute(CPX);
}

// CPY - Compare Y Register
void CPY(uint8_t* value){
    uint8_t result = (CPU::getInstance()->Y == *value);

    if (result == 0x00){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }

    if (CPU::getInstance()->Y >= *value){
        CPU::getInstance()->setStatusBit(C);
    }else{
        CPU::getInstance()->clearStatusBit(C);
    }

    if ((result & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
}
void CPY_I(){
    immediate(CPY);
}
void CPY_Z(){
    zeropage(CPY);
}
void CPY_A(){
    absolute(CPY);
}

// DEC - Decrement Memory
void DEC(uint8_t* value){
    uint8_t result;
    *value = *value - 1;
    result = *value;

    if (result == 0x00){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }

    if ((result & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
}
void DEC_Z(){
    zeropage(DEC);
}
void DEC_ZX(){
    zeropageX(DEC);
}
void DEC_A(){
    absolute(DEC);
}
void DEC_AX(){
    absoluteX(DEC);
}

// DEX - Decrement X Register
void DEX(){
    CPU::getInstance()->X = CPU::getInstance()->X - 1;

    if (CPU::getInstance()->X == 0x00){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }

    if ((CPU::getInstance()->X & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
}

// DEX - Decrement Y Register
void DEY(){
    CPU::getInstance()->Y = CPU::getInstance()->Y - 1;

    if (CPU::getInstance()->Y == 0x00){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }

    if ((CPU::getInstance()->Y & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
}

// EOR - Exclusive OR
void EOR(uint8_t *value){
    CPU::getInstance()->A = CPU::getInstance()->A ^ *value;
    
    // Set processor status
    // Negative flag
    if ((CPU::getInstance()->A & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
    
    // Zero flag
    if (CPU::getInstance()->A == 0){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }
}
void EOR_I(){
    immediate(EOR);
}
void EOR_Z(){
    zeropage(EOR);
}
void EOR_ZX(){
    zeropageX(EOR);
}
void EOR_A(){
    absolute(EOR);
}
void EOR_AX(){
    absoluteX(EOR);
}
void EOR_AY(){
    absoluteY(EOR);
}
void EOR_IX(){
    indirectX(EOR);
}
void EOR_IY(){
    indirectY(EOR);
}

// INC - Increment Memory
void INC(uint8_t* value){
    uint8_t result;
    *value = *value + 1;
    result = *value;

    if (result == 0x00){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }

    if ((result & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
}
void INC_Z(){
    zeropage(INC);
}
void INC_ZX(){
    zeropageX(INC);
}
void INC_A(){
    absolute(INC);
}
void INC_AX(){
    absoluteX(INC);
}

// INX - Increment X Register
void INX(){
    CPU::getInstance()->X = CPU::getInstance()->X + 1;

    if (CPU::getInstance()->X == 0x00){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }

    if ((CPU::getInstance()->X & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
}

// INY - Increment Y Register
void INY(){
    CPU::getInstance()->Y = CPU::getInstance()->Y + 1;

    if (CPU::getInstance()->Y == 0x00){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }

    if ((CPU::getInstance()->Y & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
}

// JMP - Jump
void JMP(){

}
void JMP_A(){
    uint16_t address = 0;
    address += *CPU::getInstance()->nextByte();
    address += *CPU::getInstance()->nextByte() * 16 * 16;

    CPU::getInstance()->PC = address;
}
// This is indirect addressing, not immediate
void JMP_I(){
    uint16_t indirect_address = 0;
    uint16_t target_address = 0;

    indirect_address += *CPU::getInstance()->nextByte();
    indirect_address += *CPU::getInstance()->nextByte() * 16 * 16;

    target_address += *MMU::getInstance()->read(&indirect_address);
    indirect_address += 1;
    target_address += *MMU::getInstance()->read(&indirect_address) * 16 * 16;

    CPU::getInstance()->PC = target_address;
}

// JSR - Jump to Subroutine
void JSR(){
    // Push current address - 1 on to stack as a return point
    uint16_t return_point = CPU::getInstance()->PC - 1;
    CPU::getInstance()->pushStack(&return_point);

    // Set PC to target address
    JMP_A();
}

// LDA - Load Accumulator
void LDA(uint8_t *value){
    CPU::getInstance()->A = *value;

    if (CPU::getInstance()->A == 0x00){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }

    if ((CPU::getInstance()->A & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
}
void LDA_I(){
    immediate(LDA);
}
void LDA_Z(){
    zeropage(LDA);
}
void LDA_ZX(){
    zeropageX(LDA);
}
void LDA_A(){
    absolute(LDA);
}
void LDA_AX(){
    absoluteX(LDA);
}
void LDA_AY(){
    absoluteY(LDA);
}
void LDA_IX(){
    indirectX(LDA);
}
void LDA_IY(){
    indirectY(LDA);
}

// LDX - Load X Register
void LDX(uint8_t* value){
    CPU::getInstance()->X = *value;

    if (CPU::getInstance()->X == 0x00){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }

    if ((CPU::getInstance()->X & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
}
void LDX_I(){
    immediate(LDX);
}
void LDX_Z(){
    zeropage(LDX);
}
void LDX_ZY(){
    zeropageY(LDX);
} 
void LDX_A(){
    absolute(LDX);
}
void LDX_AY(){
    absoluteY(LDX);
}

// LDY - Load Y Register
void LDY(uint8_t *value){
    CPU::getInstance()->Y = *value;

    if (CPU::getInstance()->Y == 0x00){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }

    if ((CPU::getInstance()->Y & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
}
void LDY_I(){
    immediate(LDY);
}
void LDY_Z(){
    zeropage(LDY);
}
void LDY_ZX(){
    zeropageX(LDY);
} 
void LDY_A(){
    absolute(LDY);
}
void LDY_AX(){
    absoluteX(LDY);
}

// LSR - Logical Shift Right
void LSR_BASE(uint8_t *value){
    uint8_t prior_value = *value;
    *value >>= 1;
    
    // Set processor status
    // Negative flag
    if ((*value & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
    
    // Zero flag
    if (*value == 0){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }

    // Carry flag
    if ((prior_value & 0x01) == 0x01){
        CPU::getInstance()->setStatusBit(C);
    }else{
        CPU::getInstance()->clearStatusBit(C);
    }
}

void LSR(){
    LSR_BASE(&(CPU::getInstance()->A));
}
void LSR_Z(){
    zeropage(LSR_BASE);
}
void LSR_ZX(){
    zeropageX(LSR_BASE);
}
void LSR_A(){
    absolute(LSR_BASE);
}
void LSR_AX(){
    absoluteX(LSR_BASE);
}

// NOP - No Operation
void NOP(){}

// ORA - Logical Inclusive OR
void ORA(uint8_t *value){
    CPU::getInstance()->A = CPU::getInstance()->A | *value;
    
    // Set processor status
    // Negative flag
    if ((CPU::getInstance()->A & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
    
    // Zero flag
    if (CPU::getInstance()->A == 0){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }
}
void ORA_I(){
    immediate(ORA);
}
void ORA_Z(){
    zeropage(ORA);
}
void ORA_ZX(){
    zeropageX(ORA);
}
void ORA_A(){
    absolute(ORA);
}
void ORA_AX(){
    absoluteX(ORA);
}
void ORA_AY(){
    absoluteY(ORA);
}
void ORA_IX(){
    indirectX(ORA);
}
void ORA_IY(){
    indirectY(ORA);
}

// PHA - Push Accumulator
void PHA(){
    CPU::getInstance()->pushStack(&(CPU::getInstance()->A));
}

// PHP - Push Processor Status
void PHP(){
    CPU::getInstance()->pushStack(&(CPU::getInstance()->PC));
}

// PLA - Pull Accumulator
void PLA(){
    CPU::getInstance()->A = *CPU::getInstance()->popStack();

    if (CPU::getInstance()->A == 0x00){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }

    if ((CPU::getInstance()->A & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
}

// PLP - Pull Processor Status
void PLP(){
    CPU::getInstance()->P = *CPU::getInstance()->popStack();
}

// ROL - Rotate Left
void ROL_BASE(uint8_t *value){
    uint8_t prior_value=  *value;
    *value = (*value << 1) | (*value >> 7);

    // Set processor status
    // Negative flag
    if ((*value & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
    
    // Zero flag
    if (*value == 0){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }

    // Carry flag
    if ((prior_value & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(C);
    }else{
        CPU::getInstance()->clearStatusBit(C);
    }
}
void ROL(){
    ROL_BASE(&(CPU::getInstance()->A));
}
void ROL_Z(){
    zeropage(ROL_BASE);
}
void ROL_ZX(){
    zeropageX(ROL_BASE);
}
void ROL_A(){
    absolute(ROL_BASE);
}
void ROL_AX(){
    absoluteX(ROL_BASE);
}

// ROR - Rotate Right
void ROR_BASE(uint8_t *value){
    uint8_t prior_value = *value;
    *value = (*value >> 1) | (*value << 7);

    // Set processor status
    // Negative flag
    if ((*value & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
    
    // Zero flag
    if (*value == 0){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }

    // Carry flag
    if ((prior_value & 0x01) == 0x01){
        CPU::getInstance()->setStatusBit(C);
    }else{
        CPU::getInstance()->clearStatusBit(C);
    }
}
void ROR(){
    ROR_BASE(&(CPU::getInstance()->A));
}
void ROR_Z(){
    zeropage(ROR_BASE);
}
void ROR_ZX(){
    zeropageX(ROR_BASE);
}
void ROR_A(){
    absolute(ROR_BASE);
}
void ROR_AX(){
    absoluteX(ROR_BASE);
}

// RTI - Return from Interrupt
void RTI(){
    CPU::getInstance()->P = *CPU::getInstance()->popStack();
    CPU::getInstance()->PC = *CPU::getInstance()->popStack();
}

// RTS - Return from Subroutine
void RTS(){
    CPU::getInstance()->PC = *CPU::getInstance()->popStack();
}

// SBC - Subtract with Carry
void SBC(uint8_t *value){
    // TODO : verify status bits
    uint8_t prior_A = CPU::getInstance()->A;
    CPU::getInstance()->A = CPU::getInstance()->A - *value - (1 - CPU::getInstance()->getStatusBit(C));
    
    // Set processor status
    // Negative flag
    if ((CPU::getInstance()->A & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
    
    // Zero flag
    if (CPU::getInstance()->A == 0){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }

    // Overflow flag
    // http://www.righto.com/2012/12/the-6502-overflow-flag-explained.html
    if (((prior_A ^ CPU::getInstance()->A) & (*value ^ CPU::getInstance()->A) & 0x80) != 0x00){
        CPU::getInstance()->setStatusBit(V);
    }else{
        CPU::getInstance()->clearStatusBit(V);
    }

    // Carry flag
    if ((prior_A + *value + CPU::getInstance()->getStatusBit(C)) > 0xff){
        CPU::getInstance()->clearStatusBit(C);
    }else{
        CPU::getInstance()->setStatusBit(C);
    }
}
void SBC_I(){
    immediate(SBC);
}
void SBC_Z(){
    zeropage(SBC);
}
void SBC_ZX(){
    zeropageX(SBC);
}
void SBC_A(){
    absolute(SBC);
}
void SBC_AX(){
    absoluteX(SBC);
}
void SBC_AY(){
    absoluteY(SBC);
}
void SBC_IX(){
    indirectX(SBC);
}
void SBC_IY(){
    indirectY(SBC);
}

// SEC - Set Carry Flag
void SEC(){
    CPU::getInstance()->setStatusBit(C);
}

// SED - Set Decimal Flag
void SED(){
    CPU::getInstance()->setStatusBit(D);
}

// SEI - Set Interrupt Disable
void SEI(){
    CPU::getInstance()->setStatusBit(I);
}

// STA - Store Accumulator
void STA(uint8_t *value){
    *value = CPU::getInstance()->A;
}
void STA_Z(){
    zeropage(STA);
}
void STA_ZX(){
    zeropageX(STA);
}
void STA_A(){
    absolute(STA);
}
void STA_AX(){
    absoluteX(STA);
}
void STA_AY(){
    absoluteY(STA);
}
void STA_IX(){
    indirectX(STA);
}
void STA_IY(){
    indirectY(STA);
}

// STX - Store X Register
void STX(uint8_t *value){
    *value = CPU::getInstance()->X;
}
void STX_Z(){
    zeropage(STX);
}
void STX_ZY(){
    zeropageY(STX);
}
void STX_A(){
    absolute(STX);
}

// STY - Store Y Register
void STY(uint8_t *value){
    *value = CPU::getInstance()->Y;
}
void STY_Z(){
    zeropage(STY);
}
void STY_ZX(){
    zeropageY(STY);
}
void STY_A(){
    absolute(STY);
}

// TAX - Transfer Accumulator to X
void TAX(){
    CPU::getInstance()->X = CPU::getInstance()->A;

    if (CPU::getInstance()->X == 0x00){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }

    if ((CPU::getInstance()->X & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
}

// TAY - Transfer Accumulator to Y
void TAY(){
    CPU::getInstance()->Y = CPU::getInstance()->A;

    if (CPU::getInstance()->Y == 0x00){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }

    if ((CPU::getInstance()->Y & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
}

// TSX - Transfer Stack Pointer to X
void TSX(){
    CPU::getInstance()->X = CPU::getInstance()->SP;

    if (CPU::getInstance()->X == 0x00){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }

    if ((CPU::getInstance()->X & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
}

// TXA - Transfer X to Accumulator
void TXA(){
    CPU::getInstance()->A = CPU::getInstance()->X;

    if (CPU::getInstance()->A == 0x00){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }

    if ((CPU::getInstance()->A & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
}

// TXS - Transfer X to Stack Pointer
void TXS(){
    CPU::getInstance()->SP = CPU::getInstance()->X;
}

// TYA - Transfer Y to Accumulator
void TYA(){
    CPU::getInstance()->A = CPU::getInstance()->Y;

    if (CPU::getInstance()->A == 0x00){
        CPU::getInstance()->setStatusBit(Z);
    }else{
        CPU::getInstance()->clearStatusBit(Z);
    }

    if ((CPU::getInstance()->A & 0x80) == 0x80){
        CPU::getInstance()->setStatusBit(N);
    }else{
        CPU::getInstance()->clearStatusBit(N);
    }
}