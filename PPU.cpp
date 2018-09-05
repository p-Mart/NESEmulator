
#include "PPU.h"
#include "CPU.h"
#include "MMU.h"
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <assert.h>

PPU* PPU::instance = 0;

PPU::PPU(){
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "SDL_Init error: " << SDL_GetError() << std::endl;
        throw SDL_GetError();
    }

    // Create window
    win = SDL_CreateWindow(
        "NES Emulator", 100, 100, RESOLUTION_WIDTH_NTSC, RESOLUTION_HEIGHT_PAL, SDL_WINDOW_SHOWN);
    if (win == nullptr){
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        throw SDL_GetError();
    }

    // Initialize Renderer
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr){
        SDL_DestroyWindow(win);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        throw SDL_GetError();
    }

    // Clear renderer and show window
    SDL_RenderClear(ren);
    SDL_RenderPresent(ren);

    // Point ppu_ctrl to the PPUCTRL register
    uint16_t addr = PPU_CTRL;
    ppu_ctrl = MMU::getInstance()->readPort(&addr);
    *ppu_ctrl = 0;

    // Point ppu_mask to the PPUMASK register
    addr = PPU_MASK;
    ppu_mask = MMU::getInstance()->readPort(&addr);
    *ppu_mask = 0;

    // Point ppu_status to the PPUSTATUS register
    addr = PPU_STATUS;
    ppu_status = MMU::getInstance()->readPort(&addr);
    *ppu_status = 0;

    // Set vram address to starting address of memory
    vram_address = 0;

    // Initialize latches, counters
    address_latch = 0;
    status_read = 0;
    cycle = 0;
    scanline = 0;
}

PPU::~PPU(){
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

PPU* PPU::getInstance(){
    if (instance == 0){
        instance = new PPU();
    }

    return instance;
}

uint8_t *PPU::read(uint16_t *address){
    
    uint16_t hardware_address = *address; // Emulate mirroring
    // Bytes 0x0000 - 0x3FFF are mirrored from 0x4000 -> 0xFFFF
    if(hardware_address >= MIRRORS_START && hardware_address <= MIRRORS_END){
        hardware_address = hardware_address % MIRRORS_START;
    }
    // Bytes 0x2000 - 0x2EFF are mirrored from 0x3000 -> 0x3EFF
    if(hardware_address >= 0x3000 && hardware_address <= 0x3EFF){
        hardware_address = (hardware_address % (0x3EFF - 0x3000 + 1)) + 0x2000;
    }
    // Bytes 0x3F00 - 0x3F1F are mirrored from 0x3F20 - 0x3FFF
    if(hardware_address >= 0x3F20 && hardware_address <= 0x3FFF){
        hardware_address = (hardware_address % (0x3FFF - 0x3F20 + 1)) + 0x3F00;
    }

    return &memory[hardware_address];
}

uint8_t *PPU::read(uint8_t *address){
    return &memory[*address];
}

void PPU::write(uint16_t *address, uint8_t *value){

    uint16_t hardware_address = *address;
    // Bytes 0x0000 - 0x3FFF are mirrored from 0x4000 -> 0xFFFF
    if(hardware_address >= MIRRORS_START && hardware_address <= MIRRORS_END){
        hardware_address = hardware_address % MIRRORS_START;
    }
    // Bytes 0x2000 - 0x2EFF are mirrored from 0x3000 -> 0x3EFF
    if(hardware_address >= 0x3000 && hardware_address <= 0x3EFF){
        hardware_address = (hardware_address % (0x3EFF - 0x3000 + 1)) + 0x2000;
    }
    // Bytes 0x3F00 - 0x3F1F are mirrored from 0x3F20 - 0x3FFF
    if(hardware_address >= 0x3F20 && hardware_address <= 0x3FFF){
        hardware_address = (hardware_address % (0x3FFF - 0x3F20 + 1)) + 0x3F00;
    }

    memory[hardware_address] = *value;
}

void PPU::write(uint8_t *address, uint8_t *value){
    memory[*address] = *value;
}

void PPU::addrPort(uint8_t *value){
    // Address port needs 2 writes (hi then lo byte), at which point 
    // data is written on the PPU DATA line
    if (!address_latch){
        vram_address = 0;
        vram_address += (*value) * 256; // hi byte
        address_latch = true;
    }else{
        vram_address += *value; // lo byte
        address_latch = false;
    }
}

void PPU::dataPort(uint8_t *value){
    // A write on the data port will increment the current vram address
    // by 1 or 32, which depends on bit 2 of the PPUMASK register. The
    // value written on the port is stored prior to incrementing
    bool vram_increment_flag = ((*ppu_ctrl & 0x04) == 0x04);
    uint16_t increment_amount = (vram_increment_flag == 0) ? 1 : 32;
    write(&vram_address, value);
    vram_address += increment_amount;
}


void PPU::memDump(){
    std::string dumpfile_name = "PPU_Dump.txt";
    std::ofstream dumpfile;
    std::stringstream sstream;
    dumpfile.open(dumpfile_name);

    int bytes_per_line = 16;
    // Print out 16 bytes per line
    for(int i = 0; i < PPU_MEMORY_SIZE; i+=bytes_per_line){
        sstream.str("");
        sstream << std::setfill('0') << std::setw(4) << std::hex << i;
        sstream << ":\t";

        for(int j = 0; j < bytes_per_line; j++){
            sstream << std::setfill('0') << std::setw(2) << std::hex << +memory[i+j];
            sstream << " ";
        }
        dumpfile << sstream.str() << std::endl;
    }

    dumpfile.close();
}

void PPU::tick(){


    // Perform one PPU Cycle
    if(scanline < SCANLINES_PER_FRAME){

        if (cycle < SCANLINE_CYCLES){
            // Each scanline of rendering takes 341 cycles

            cycle += 1;
        }else{
            cycle = 0;
            scanline += 1;
        }
        
    }else{
        scanline = 0;
    }
}

// Render a frame
void PPU::renderFrame(){

    if(status_read){
        *ppu_status = *ppu_status & 0x7F;
        status_read = false;
    }

    // PPU Timing Emulation
    if(scanline == 0){
        // Pre-render scanlines
        // Disable v-blank occurring flag
        if(cycle == 1) *ppu_status = *ppu_status & 0x1F;
    }
    else if ((scanline > 0) && (scanline < 241)){
        // Visible scanlines
        if ((cycle > 0) && (cycle < 257)){
            // Render each pixel on the current scanline
            //renderPixel(cycle);
        }
        else if ((cycle >= 257) && (cycle < 321)){
            // Tile data for sprites on next scanline fetched
        }
        else if ((cycle >= 321) && (cycle < 337)){
            // Fetch first two tiles for next scanline
        }
        else if ((cycle >= 337) && (cycle < 341)){
            // Used in only one memory mapper (MMC5)
        }
    }
    else if (scanline == 241 && cycle == 0){
        // Idle scanline
        // Cheating here - rendering everything all at once
        SDL_RenderClear(ren);
        for(uint16_t tile = 0; tile < NUM_TILES; tile++){
            renderTile(tile);
        }
        SDL_RenderPresent(ren);
    }
    else if ((scanline >= 242) && (scanline < 263)){
        // Vertical blanking scanlines
        if ((cycle == 1) && (scanline == 242)){
             // V-Blank
            *ppu_status = *ppu_status | 0x80;
            // Interrupt if bit 7 is turned on in PPU CTRL
            if ((*ppu_ctrl & 0x80) == 0x80){
                CPU::getInstance()->interruptNMI();
            }
        }
    }

    // Increment cycle / scanline counter
    if(cycle < SCANLINE_CYCLES){
        cycle++;
    }else{
        scanline++;
        cycle = 0;
    }
    if(scanline == 263) scanline = 0;

    /*    
    if(cycle < SCANLINE_CYCLES){
        cycle++;
    }else{
        cycle = 0;

        SDL_RenderClear(ren);
        for(uint16_t tile = 0; tile < NUM_TILES; tile++){
            renderTile(tile);
        }
        SDL_RenderPresent(ren);
        
        // V-Blank
        *ppu_status = *ppu_status | 0x80;
        // Interrupt if bit 7 is turned on in PPU CTRL
        if ((*ppu_ctrl & 0x80) == 0x80){
            CPU::getInstance()->interruptNMI();
        }
    }
    */
}

// Get the upper 2 bits of palette based
// on the attribute table and tile number
uint8_t PPU::getPaletteHigh(uint16_t tile){
    if (tile < 0 || tile >= NUM_TILES){
        throw "tile # is out of range [0, 959]";
    }

    // Calculate which byte (0-63) the current
    // tile belongs to
    uint8_t byte_x = (tile % 32) / 4;
    uint8_t byte_y = (tile / 32) / 4;
    uint16_t byte_addr = byte_x + 8 * byte_y;

    // Get the byte's address in the current
    // attribute table and read it
    uint16_t attribute_addr = getCurrentAttributeTable();
    byte_addr += attribute_addr;
    uint8_t byte = *read(&byte_addr);
    
    // Now, calculate which square (0 - 3) within
    // the block this tile belongs to
    uint8_t square_x = (tile % 4) / 2; // low bit
    uint8_t square_y = 2 * (((tile / 32) / 2) % 2); // high bit (multiply by 2)
    uint8_t square = square_x + square_y;

    uint8_t palette_high;
    //Byte format is 33221100
    switch(square){
        case 0x00:
            palette_high = byte & 0x03;
            break;
        case 0x01:
            palette_high = (byte & 0x0C) >> 2;
            break;
        case 0x02:
            palette_high = (byte & 0x30) >> 4;
            break;
        case 0x03:
            palette_high = (byte & 0xC0) >> 6;
            break;
        default:
            throw "square is not in [0, 3]";
            break;
    }

    return palette_high;
}

// Calculate the lower 2 bits of palette 
// based on the desired pixel in the
// CHR art.
// Takes in the row # in the CHR art and the
// pixel in that row.
uint8_t PPU::getPaletteLow(uint8_t chr_row_low, uint8_t chr_row_high, uint8_t pixel){
    uint8_t low_bit = ((0x80 >> pixel) & chr_row_low) >> (7 - pixel);
    uint8_t high_bit = ((0x80 >> pixel) & chr_row_high) >> (7 - pixel);
    uint8_t palette_low = (high_bit << 1) | low_bit;
    return palette_low;
}

SDL_Color PPU::getPixelColor(uint8_t palette_low, uint8_t palette_high){
    uint16_t palette_addr = (palette_high << 2) + palette_low;
    palette_addr += IMAGE_PALETTE_START;

    uint8_t palette = *read(&palette_addr);
    SDL_Color color = system_palette[palette];

    return color;
}

// Return the starting address of the name table
// that's currently in use
uint16_t PPU::getCurrentNameTable(){
    uint16_t name_table_addr;
    uint8_t name_table_bits = *ppu_ctrl & 0x03;
    switch (name_table_bits){
        case 0x00:
            name_table_addr = NAME_TABLE_0_START;
            break;
        case 0x01:
            name_table_addr = NAME_TABLE_1_START;
            break;
        case 0x02:
            name_table_addr = NAME_TABLE_2_START;
            break;
        case 0x03:
            name_table_addr = NAME_TABLE_3_START;
            break;
        default:
            throw "name_table_bits is not in [0, 3]";
            break;
    }

    return name_table_addr;
}

// Return the starting address of the attribute table
// that's currently in use
uint16_t PPU::getCurrentAttributeTable(){
    uint16_t attribute_table_addr;
    uint8_t name_table_bits = *ppu_ctrl;
    name_table_bits = name_table_bits & 0x03;
    // The attribute table is associated with the
    // name table currently in use
    switch (name_table_bits){
        case 0x00:
            attribute_table_addr = ATTRIBUTE_TABLE_0_START;
            break;
        case 0x01:
            attribute_table_addr = ATTRIBUTE_TABLE_1_START;
            break;
        case 0x02:
            attribute_table_addr = ATTRIBUTE_TABLE_2_START;
            break;
        case 0x03:
            attribute_table_addr = ATTRIBUTE_TABLE_3_START;
            break;
        default:
            throw "name_table_bits is not in [0, 3]";
            break;
    }

    return attribute_table_addr;
}

// Return the starting address of the pattern table
// that's currently in use
uint16_t PPU::getCurrentPatternTable(){
    uint16_t pattern_table_addr;
    uint8_t pattern_table_bits = (*ppu_ctrl & 0x08) >> 3;
    switch (pattern_table_bits){
        case 0x00:
            pattern_table_addr = PATTERN_TABLE_0_START;
            break;
        case 0x01:
            pattern_table_addr = PATTERN_TABLE_1_START;
            break;
        default:
            throw "pattern_table_bits is not in [0, 1]";
            break;
    }

    return pattern_table_addr;
}

void PPU::renderPixel(uint16_t pixel){
    // Render a pixel on the current scanline

}

void PPU::renderTile(uint16_t tile){
    // Get starting address of name table, based on which
    // name table is being used
    uint16_t name_table_addr = getCurrentNameTable();

    // Add the tile variable to get the proper entry in the table
    name_table_addr += tile;

    // Read to get the tile #
    uint8_t tile_number = *read(&name_table_addr);

    // Calculate the starting address of the tile # in the
    // pattern table
    uint16_t pattern_table_addr = getCurrentPatternTable();
    pattern_table_addr = pattern_table_addr + (16 * tile_number);

    // Get the high bits for palette
    uint8_t palette_high = getPaletteHigh(tile);

    // Loop over each row of pixels
    for(uint8_t row_number = 0; row_number < 8; row_number++){
        // Get the i'th row of pixels, low part
        uint8_t chr_row_low = *read(&pattern_table_addr);

        // Get the i'th row of pixels, hi part (should always be 8 bytes
        // added to the current address)
        uint16_t offset_addr = pattern_table_addr + 0x08;
        uint8_t chr_row_high = *read(&offset_addr);

        // Draw each pixel in that row, from left to right
        for(uint8_t pixel = 0; pixel < 8; pixel++){
            // Get the low bits for color
            uint8_t palette_low = getPaletteLow(chr_row_low, chr_row_high, pixel);
            
            // The high bits are from palette_high
            SDL_Color pixel_color = getPixelColor(palette_low, palette_high);

            // Figure out the x and y position of this pixel
            int x = (tile % 32) * 8 + pixel;
            int y = (tile / 32) * 8 + row_number;

            // Draw pixel
            SDL_SetRenderDrawColor(ren, pixel_color.r, pixel_color.g, pixel_color.b, 0xff);
            SDL_RenderDrawPoint(ren, x, y);
        }

        // Move to next row
        pattern_table_addr += 1;
    }
}

void PPU::statusRead(){
    // Mark NMI_occurred flag to 0 on next cycle
    status_read = true;
}