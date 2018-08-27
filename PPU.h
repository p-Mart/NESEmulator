#ifndef PPU_H
#define PPU_H

#define PPU_MEMORY_SIZE 65536
#define PPU_SPR_RAM_SIZE 256

// PPU Memory Map Addresses
#define PATTERN_TABLE_START 0x0000
#define PATTERN_TABLE_0_START 0x0000
#define PATTERN_TABLE_1_START 0x1000
#define PATTERN_TABLE_END 0x1FFF

#define NAME_TABLE_START 0x2000
#define NAME_TABLE_0_START 0x2000
#define NAME_TABLE_1_START 0x2400
#define NAME_TABLE_2_START 0x2800
#define NAME_TABLE_3_START 0x2C00
#define NAME_TABLE_MIRROR_START 0x3000
#define NAME_TABLE_END 0x3EFF

#define ATTRIBUTE_TABLE_0_START 0x23C0
#define ATTRIBUTE_TABLE_1_START 0x27C0
#define ATTRIBUTE_TABLE_2_START 0x2BC0
#define ATTRIBUTE_TABLE_3_START 0x2FC0

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
#define IMAGE_PALETTE_START 0x3F00
#define SPRITE_PALETTE_START 0x3F10
#define PALETTES_END 0x3FFF

// Technically the PPU can only address 16KB
// so everything after 0x4000 is mirroring of the first
// 16 KB of memory
#define MIRRORS_START 0x4000
#define MIRRORS_END 0xFFFF

// Resolution to draw at
#define RESOLUTION_WIDTH_NTSC 256
#define RESOLUTION_HEIGHT_NTSC 224
#define RESOLUTION_HEIGHT_PAL 240

// Misc. Drawing info
#define NUM_TILES 960

#include <SDL2/SDL.h>
#include <cstdint>

class PPU{
   
    // Define as singleton to ensure only one PPU gets created
    static PPU *instance;

    SDL_Window *win;
    SDL_Renderer *ren;

    // Internal 16-bit addressable memory
    uint8_t memory[PPU_MEMORY_SIZE];
    uint8_t spr_ram[PPU_SPR_RAM_SIZE];
    uint16_t vram_address;

    // Control port
    uint8_t *ppu_ctrl;

    // Mask port
    uint8_t *ppu_mask;

    // Status port
    uint8_t *ppu_status;

    // Latches
    bool address_latch;

    // System color palette
    const SDL_Color system_palette[64] = {
        {0x75,0x75,0x75,0xFF},
        {0x27,0x1B,0x8F,0xFF},
        {0x00,0x00,0xab,0xFF},
        {0x47,0x00,0x9f,0xFF},
        {0x8f,0x00,0x77,0xFF},
        {0xab,0x00,0x13,0xFF},
        {0xa7,0x00,0x00,0xFF},
        {0x7f,0x0b,0x00,0xFF},
        {0x43,0x2f,0x00,0xFF},
        {0x00,0x47,0x00,0xFF},
        {0x00,0x51,0x00,0xFF},
        {0x00,0x3f,0x17,0xFF},
        {0x1b,0x3f,0x5f,0xFF},
        {0x00,0x00,0x00,0xFF},
        {0x00,0x00,0x00,0xFF},
        {0x00,0x00,0x00,0xFF},
        {0xbc,0xbc,0xbc,0xFF},
        {0x00,0x73,0xef,0xFF},
        {0x23,0x3b,0xef,0xFF},
        {0x83,0x00,0xf3,0xFF},
        {0xbf,0x00,0xbf,0xFF},
        {0xe7,0x00,0x5b,0xFF},
        {0xdb,0x2b,0x00,0xFF},
        {0xcb,0x4f,0x0f,0xFF},
        {0x8b,0x73,0x00,0xFF},
        {0x00,0x97,0x00,0xFF},
        {0x00,0xab,0x00,0xFF},
        {0x00,0x93,0x3b,0xFF},
        {0x00,0x83,0x8b,0xFF},
        {0x00,0x00,0x00,0xFF},
        {0x00,0x00,0x00,0xFF},
        {0x00,0x00,0x00,0xFF},
        {0xff,0xff,0xff,0xFF},
        {0x3f,0xbf,0xff,0xFF},
        {0x5f,0x97,0xff,0xFF},
        {0xa7,0x8b,0xfd,0xFF},
        {0xf7,0x7b,0xff,0xFF},
        {0xff,0x77,0xb7,0xFF},
        {0xff,0x77,0x63,0xFF},
        {0xff,0x9b,0x3b,0xFF},
        {0xf3,0xbf,0x3f,0xFF},
        {0x83,0xd3,0x13,0xFF},
        {0x4f,0xdf,0x4b,0xFF},
        {0x58,0xf8,0x98,0xFF},
        {0x00,0xeb,0xdb,0xFF},
        {0x00,0x00,0x00,0xFF},
        {0x00,0x00,0x00,0xFF},
        {0x00,0x00,0x00,0xFF},
        {0xff,0xff,0xff,0xFF},
        {0xab,0xe7,0xff,0xFF},
        {0xc7,0xd7,0xff,0xFF},
        {0xd7,0xcb,0xff,0xFF},
        {0xff,0xc7,0xff,0xFF},
        {0xff,0xc7,0xdb,0xFF},
        {0xff,0xbf,0xb3,0xFF},
        {0xff,0xdb,0xab,0xFF},
        {0xff,0xe7,0xa3,0xFF},
        {0xe3,0xff,0xa3,0xFF},
        {0xab,0xf3,0xbf,0xFF},
        {0xb3,0xff,0xcf,0xFF},
        {0x9f,0xff,0xf3,0xFF},
        {0x00,0x00,0x00,0xFF},
        {0x00,0x00,0x00,0xFF},
        {0x00,0x00,0x00,0xFF}
    };

    PPU();
    ~PPU();

   public:
        static PPU* getInstance();
        uint8_t *read(uint16_t*);
        uint8_t *read(uint8_t*);
        void write(uint16_t*, uint8_t*);
        void write(uint8_t*, uint8_t*);
        void addrPort(uint8_t*);
        void dataPort(uint8_t*);

        void renderFrame();
        void renderTile(uint16_t);

        uint8_t getPaletteHigh(uint16_t);
        uint8_t getPaletteLow(uint8_t,uint8_t,uint8_t);
        uint16_t getCurrentNameTable();
        uint16_t getCurrentAttributeTable();
        uint16_t getCurrentPatternTable();

        SDL_Color getPixelColor(uint8_t, uint8_t);
        void memDump();
};

#endif