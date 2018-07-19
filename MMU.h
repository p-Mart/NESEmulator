#ifndef MMU_H
#define MMU_H

#define MEMORY_SIZE 65536

// Memory Map Addresses
// RAM
#define INTERNAL_MEMORY_START 0x0000
#define INTERNAL_MEMORY_END 0x07FF
#define ZERO_PAGE_START 0x0000
#define ZERO_PAGE_END 0x00FF
#define STACK_START 0x0100
#define STACK_END 0x01FF
#define RAM_START 0x0200
#define RAM_END 0x07FF

// I/O Register mapping
#define IO_REGISTERS_START 0x2000
#define IO_REGISTERS_END 0x401F
#define PPU_REGISTERS_START 0x2000
#define PPU_REGISTERS_END 0x2007

// Cartridge Space
#define CARTRIDGE_START 0x4020
#define CARTRIDGE_END 0xFFFF
#define INTERRUPTS_START 0xFFFA
#define INTERRUPTS_END 0xFFFF

#include <cstdint>

class MMU{
   
    // Define as singleton to ensure only one MMU gets created
    static MMU *instance;

    // Internal 16-bit addressable memory
    uint8_t memory[MEMORY_SIZE];

    MMU(){};

   public:
        static MMU* getInstance();
        uint8_t *read(uint16_t*);
        uint8_t *read(uint8_t*);

        void write(uint16_t*, uint8_t*);
        void write(uint8_t*, uint8_t*);
        void writeStack(uint8_t*, uint8_t*);
        uint8_t *readStack(uint8_t*);
};

#endif