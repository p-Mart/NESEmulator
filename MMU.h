#ifndef MMU_H
#define MMU_H

#define MEMORY_SIZE 65536

// Memory Map Addresses
// RAM
#define INTERNAL_MEMORY_START 0x0000
#define INTERNAL_MEMORY_END 0x1FFF
#define ZERO_PAGE_START 0x0000
#define ZERO_PAGE_END 0x00FF
#define STACK_START 0x0100
#define STACK_END 0x01FF
#define RAM_START 0x0200
#define RAM_END 0x07FF

// I/O Register mapping
#define IO_REGISTERS_START 0x2000
#define IO_REGISTERS_END 0x401F

#define PPU_START 0x2000
#define PPU_END 0x2007
#define PPU_MIRRORS_START 0x2008
#define PPU_MIRRORS_END 0x3FFF

#define SQ1_VOL 0x4000
#define SQ1_SWEEP 0x4001
#define SQ1_LO 0x4002
#define SQ1_HI 0x4003
#define SQ2_VOL 0x4004
#define SQ2_SWEEP 0x4005
#define SQ2_LOW 0x4006
#define SQ2_HI 0x4007
#define TRI_LINEAR 0x4008
#define TRI_LO 0x400A
#define TRI_HI 0x400B
#define NOISE_VOL 0x400C
#define NOISE_LO 0x400E
#define NOISE_HI 0x400F
#define DMC_FREQ 0x4010
#define DMC_RAW 0x4011
#define DMC_START 0x4012
#define DMC_LEN 0x4013
#define OAMDMA 0x4014
#define SND_CHN 0x4015
#define JOY1 0x4016
#define JOY2 0x4017

// Cartridge Space
#define CARTRIDGE_START 0x4020
#define PRG_ROM1_START 0x8000
#define PRG_ROM1_END 0xBFFF
#define PRG_ROM2_START 0xC000
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
        uint8_t *readPort(uint16_t*);

        void write(uint16_t*, uint8_t*);
        void write(uint8_t*, uint8_t*);
        void writeStack(uint8_t*, uint8_t*);
        uint8_t *readStack(uint8_t*);

        void loadPRGROM(std::vector<uint8_t>&);
        void loadCHRROM(std::vector<uint8_t>&, uint8_t&);

        void memDump();
};

#endif