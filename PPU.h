#ifndef PPU_H
#define PPU_H

#define PPU_MEMORY_SIZE 65536

// PPU Memory Map Addresses
#define PATTERN_TABLE_START 0x0000
#define PATTERN_TABLE_END 0x1FFF

#define NAME_TABLE_START 0x2000
#define NAME_TABLE_END 0x3EFF

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