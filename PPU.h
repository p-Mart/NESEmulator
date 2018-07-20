#ifndef PPU_H
#define PPU_H

#define PPU_MEMORY_SIZE 65536
#define PPU_SPR_RAM_SIZE 256

// PPU Memory Map Addresses
#define PATTERN_TABLE_START 0x0000
#define PATTERN_TABLE_END 0x1FFF

#define NAME_TABLE_START 0x2000
#define NAME_TABLE_END 0x3EFF

// Memory-Mapped IO 
#define PPU_CTRL 0x2000
#define PPU_MASK 0x2001
#define PPU_STATUS 0x2002
#define OAM_ADDR 0x2003
#define OAM_DATA 0x2004
#define PPU_SCROLL 0x2005
#define PPU_ADDR 0x2006
#define PPU_DATA 0x2007

#define PALETTES_START 0x3F00
#define PALETTES_END 0x3FFF

// Technically the PPU can only address 16KB
// so everything after 0x4000 is mirroring of the first
// 16 KB of memory
#define MIRRORS_START 0x4000
#define MIRRORS_END 0xFFFF


#include <cstdint>

class PPU{
   
    // Define as singleton to ensure only one PPU gets created
    static PPU *instance;

    // Internal 16-bit addressable memory
    uint8_t memory[PPU_MEMORY_SIZE];
    uint8_t spr_ram[PPU_SPR_RAM_SIZE];

    PPU(){};

   public:
        static PPU* getInstance();
        uint8_t *read(uint16_t*);
        uint8_t *read(uint8_t*);

        void write(uint16_t*, uint8_t*);
        void write(uint8_t*, uint8_t*);

        void memDump();
};

#endif