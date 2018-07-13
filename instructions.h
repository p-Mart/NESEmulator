#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <cstdint>

// Naming convention for functions:
//      (OPCODE)_(ADDRESSINGMODE)
// Where the addressing mode can be - 
//      I - Immediate
//      Z - Zero page
//      ZX - Zero page, X
//      A - Absolute
//      AX - Absolute, X
//      AY - Absolute, Y
//      IX - Indirect, X
//      IY - Indirect, Y
// If the addressing mode portion is left off, 
// then it is (likely?) just acting on the registers,
// or it is the base behavior for the function

// ADC - Add with Carry
void ADC(uint8_t*);
void ADC_I();
void ADC_Z();
void ADC_ZX();
void ADC_A();
void ADC_AX();
void ADC_AY();
void ADC_IX();
void ADC_IY();

// AND - Logical AND
void AND();
void AND_I();
void AND_Z();
void AND_ZX();
void AND_A();
void AND_AX();
void AND_AY();
void AND_IX();
void AND_IY();

// ASL - Arithmetic Shift Left
void ASL();
void ASL_Z();
void ASL_ZX();
void ASL_A();
void ASL_AX();

// BCC - Branch if Carry Clear
void BCC();

// BCS - Branch if Carry Set
void BCS();

// BEQ - Branch if Equal
void BEQ();

// BIT - Bit Test
void BIT();
void BIT_Z();
void BIT_A();

// BMI - Branch if Minus
void BMI();

// BNE - Branch if Not Equal
void BNE();

// BPL - Branch if Positive
void BPL();

// BRK - Force Interrupt
void BRK();

// BVC - Branch if Overflow Clear
void BVC();

// BVS - Branch if Overflow Set
void BVS();

// CLC - Clear Carry Flag
void CLC();

// CLD - Clear Decimal Mode
void CLD();

// CLI - Clear Interrupt Disable
void CLI();

// CLV - Clear Overflow Flag
void CLV();

// CMP - Compare
void CMP();
void CMP_I();
void CMP_Z();
void CMP_ZX();
void CMP_A();
void CMP_AX();
void CMP_AY();
void CMP_IX();
void CMP_IY();

// CPX - Compare X Register
void CPX();
void CPX_I();
void CPX_Z();
void CPX_A();

// CPY - Compare Y Register
void CPY();
void CPY_I();
void CPY_Z();
void CPY_A();

// DEC - Decrement Memory
void DEC();
void DEC_Z();
void DEC_ZX();
void DEC_A();
void DEC_AX();

// DEX - Decrement X Register
void DEX();

// DEX - Decrement Y Register
void DEY();

// EOR - Exclusive OR
void EOR();
void EOR_I();
void EOR_Z();
void EOR_ZX();
void EOR_A();
void EOR_AX();
void EOR_AY();
void EOR_IX();
void EOR_IY();

// INC - Increment Memory
void INC();
void INC_Z();
void INC_ZX();
void INC_A();
void INC_AX();

// INX - Increment X Register
void INX();

// INY - Increment Y Register
void INY();

// JMP - Jump
void JMP();
void JMP_A();
void JMP_I(); // NOTE: This is for INDIRECT addressing (only JMP does this)

// JSR - Jump to Subroutine
void JSR(); // Absolute addressing only

// LDA - Load Accumulator
void LDA();
void LDA_I();
void LDA_Z();
void LDA_ZX();
void LDA_A();
void LDA_AX();
void LDA_AY();
void LDA_IX();
void LDA_IY();

// LDX - Load X Register
void LDX();
void LDX_I();
void LDX_Z();
void LDX_ZY(); 
void LDX_A();
void LDX_AY();

// LDY - Load Y Register
void LDY();
void LDY_I();
void LDY_Z();
void LDY_ZX(); 
void LDY_A();
void LDY_AX();

// LSR - Logical Shift Right
void LSR();
void LSR_Z();
void LSR_ZX();
void LSR_A();
void LSR_AX();

// NOP - No Operation
void NOP();

// ORA - Logical Inclusive OR
void ORA();
void ORA_I();
void ORA_Z();
void ORA_ZX();
void ORA_A();
void ORA_AX();
void ORA_AY();
void ORA_IX();
void ORA_IY();

// PHA - Push Accumulator
void PHA();

// PHP - Push Processor Status
void PHP();

// PLA - Pull Accumulator
void PLA();

// PLP - Pull Processor Status
void PLP();

// ROL - Rotate Left
void ROL();
void ROL_Z();
void ROL_ZX();
void ROL_A();
void ROL_AX();

// ROR - Rotate Right
void ROR();
void ROR_Z();
void ROR_ZX();
void ROR_A();
void ROR_AX();

// RTI - Return from Interrupt
void RTI();

// RTS - Return from Subroutine
void RTS();

// SBC - Subtract with Carry
void SBC();
void SBC_I();
void SBC_Z();
void SBC_ZX();
void SBC_A();
void SBC_AX();
void SBC_AY();
void SBC_IX();
void SBC_IY();

// SEC - Set Carry Flag
void SEC();

// SED - Set Decimal Flag
void SED();

// SEI - Set Interrupt Disable
void SEI();

// STA - Store Accumulator
void STA(uint16_t*);
void STA_Z();
void STA_ZX();
void STA_A();
void STA_AX();
void STA_AY();
void STA_IX();
void STA_IY();

// STX - Store X Register
void STX();
void STX_Z();
void STX_ZY();
void STX_A();

// STY - Store Y Register
void STY();
void STY_Z();
void STY_ZX();
void STY_A();

// TAX - Transfer Accumulator to X
void TAX();

// TAY - Transfer Accumulator to Y
void TAY();

// TSX - Transfer Stack Pointer to X
void TSX();

// TXA - Transfer X to Accumulator
void TXA();

// TXS - Transfer X to Stack Pointer
void TXS();

// TYA - Transfer Y to Accumulator
void TYA();

#endif