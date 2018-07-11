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
void ADC(uint16_t);
void ADC_I(uint8_t);
void ADC_Z(uint8_t);
void ADC_ZX(uint8_t);
void ADC_A(uint16_t);
void ADC_AX(uint16_t);
void ADC_AY(uint16_t);
void ADC_IX(uint8_t);
void ADC_IY(uint8_t);

// AND - Logical AND
void AND(uint16_t);
void AND_I(uint8_t);
void AND_Z(uint8_t);
void AND_ZX(uint8_t);
void AND_A(uint16_t);
void AND_AX(uint16_t);
void AND_AY(uint16_t);
void AND_IX(uint8_t);
void AND_IY(uint8_t);

// ASL - Arithmetic Shift Left
void ASL();
void ASL_Z(uint8_t);
void ASL_ZX(uint8_t);
void ASL_A(uint16_t);
void ASL_AX(uint16_t);

// BCC - Branch if Carry Clear
void BCC(uint8_t);

// BCS - Branch if Carry Set
void BCS(uint8_t);

// BEQ - Branch if Equal
void BEQ(uint8_t);

// BIT - Bit Test
void BIT(uint16_t);
void BIT_Z(uint8_t);
void BIT_A(uint16_t);

// BMI - Branch if Minus
void BMI(uint8_t);

// BNE - Branch if Not Equal
void BNE(uint8_t);

// BPL - Branch if Positive
void BPL(uint8_t);

// BRK - Force Interrupt
void BRK();

// BVC - Branch if Overflow Clear
void BVC(uint8_t);

// BVS - Branch if Overflow Set
void BVS(uint8_t);

// CLC - Clear Carry Flag
void CLC();

// CLD - Clear Decimal Mode
void CLD();

// CLI - Clear Interrupt Disable
void CLI();

// CLV - Clear Overflow Flag
void CLV();

// CMP - Compare
void CMP(uint16_t);
void CMP_I(uint8_t);
void CMP_Z(uint8_t);
void CMP_ZX(uint8_t);
void CMP_A(uint16_t);
void CMP_AX(uint16_t);
void CMP_AY(uint16_t);
void CMP_IX(uint8_t);
void CMP_IY(uint8_t);

// CPX - Compare X Register
void CPX(uint16_t);
void CPX_I(uint8_t);
void CPX_Z(uint8_t);
void CPX_A(uint16_t);

// CPY - Compare Y Register
void CPY(uint16_t);
void CPY_I(uint8_t);
void CPY_Z(uint8_t);
void CPY_A(uint16_t);

// DEC - Decrement Memory
void DEC(uint16_t);
void DEC_Z(uint8_t);
void DEC_ZX(uint8_t);
void DEC_A(uint16_t);
void DEC_AX(uint16_t);

// DEX - Decrement X Register
void DEX();

// DEX - Decrement Y Register
void DEY();

// EOR - Exclusive OR
void EOR(uint16_t);
void EOR_I(uint8_t);
void EOR_Z(uint8_t);
void EOR_ZX(uint8_t);
void EOR_A(uint16_t);
void EOR_AX(uint16_t);
void EOR_AY(uint16_t);
void EOR_IX(uint8_t);
void EOR_IY(uint8_t);

// INC - Increment Memory
void INC(uint16_t);
void INC_Z(uint8_t);
void INC_ZX(uint8_t);
void INC_A(uint16_t);
void INC_AX(uint16_t);

// INX - Increment X Register
void INX();

// INY - Increment Y Register
void INY();

// JMP - Jump
void JMP(uint16_t);
void JMP_A(uint16_t);
void JMP_I(uint16_t); // NOTE: This is for INDIRECT addressing (only JMP does this)

// JSR - Jump to Subroutine
void JSR(uint16_t); // Absolute addressing only

// LDA - Load Accumulator
void LDA(uint16_t);
void LDA_I(uint8_t);
void LDA_Z(uint8_t);
void LDA_ZX(uint8_t);
void LDA_A(uint16_t);
void LDA_AX(uint16_t);
void LDA_AY(uint16_t);
void LDA_IX(uint8_t);
void LDA_IY(uint8_t);

// LDX - Load X Register
void LDX(uint16_t);
void LDX_I(uint8_t);
void LDX_Z(uint8_t);
void LDX_ZY(uint8_t); 
void LDX_A(uint16_t);
void LDX_AY(uint16_t);

// LDY - Load Y Register
void LDY(uint16_t);
void LDY_I(uint8_t);
void LDY_Z(uint8_t);
void LDY_ZX(uint8_t); 
void LDY_A(uint16_t);
void LDY_AX(uint16_t);

// LSR - Logical Shift Right
void LSR();
void LSR_Z(uint8_t);
void LSR_ZX(uint8_t);
void LSR_A(uint16_t);
void LSR_AX(uint16_t);

// NOP - No Operation
void NOP();

// ORA - Logical Inclusive OR
void ORA(uint16_t);
void ORA_I(uint8_t);
void ORA_Z(uint8_t);
void ORA_ZX(uint8_t);
void ORA_A(uint16_t);
void ORA_AX(uint16_t);
void ORA_AY(uint16_t);
void ORA_IX(uint8_t);
void ORA_IY(uint8_t);

// PHA - Push Accumulator
void PHA();

// PHP - Push Processor Status
void PHP();

// PLP - Pull Processor Status
void PLP();

// ROL - Rotate Left
void ROL();
void ROL_Z(uint8_t);
void ROL_ZX(uint8_t);
void ROL_A(uint16_t);
void ROL_AX(uint16_t);

// ROR - Rotate Right
void ROR();
void ROR_Z(uint8_t);
void ROR_ZX(uint8_t);
void ROR_A(uint16_t);
void ROR_AX(uint16_t);

// RTI - Return from Interrupt
void RTI();

// RTS - Return from Subroutine
void RTS();

// SBC - Subtract with Carry
void SBC(uint16_t);
void SBC_I(uint8_t);
void SBC_Z(uint8_t);
void SBC_ZX(uint8_t);
void SBC_A(uint16_t);
void SBC_AX(uint16_t);
void SBC_AY(uint16_t);
void SBC_IX(uint8_t);
void SBC_IY(uint8_t);

// SEC - Set Carry Flag
void SEC();

// SED - Set Decimal Flag
void SED();

// SEI - Set Interrupt Disable
void SEI();

// STA - Store Accumulator
void STA(uint16_t);
void STA_Z(uint8_t);
void STA_ZX(uint8_t);
void STA_A(uint16_t);
void STA_AX(uint16_t);
void STA_AY(uint16_t);
void STA_IX(uint8_t);
void STA_IY(uint8_t);

// STX - Store X Register
void STX(uint16_t);
void STX_Z(uint8_t);
void STX_ZY(uint8_t);
void STX_A(uint16_t);

// STY - Store Y Register
void STY(uint16_t);
void STY_Z(uint8_t);
void STY_ZX(uint8_t);
void STY_A(uint16_t);

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