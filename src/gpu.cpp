#include "gpu.h"

// SDL info
#if 0
COLORS:
    Darkest Green: 15, 56, 15
    Dark Green: 48, 98, 48
    Light Green: 139, 172, 15
    Lightest Green: 155, 188, 172
#endif 

SDL_Color DARKEST_GREEN = {0, 15, 56, 15};
SDL_Color DARK_GREEN = {0, 48, 98, 48};
SDL_Color LIGHT_GREEN = {0, 15, 172, 139};
SDL_Color LIGHTEST_GREEN = {0, 172, 188, 155};
SDL_Color* colors[] = { &DARKEST_GREEN, &DARK_GREEN, &LIGHT_GREEN, &LIGHTEST_GREEN };

static SDL_Renderer* gpu_renderer;

// Tile Maps
TileMap background_tilemap;
TileMap window_tilemap;
// Uint8 tiles[0x97FF-0x800];
// tiles 0-127, 128-255 are used usually
Tile tiles[384];
OAM_Entry oam_table[40];

//// GPU Registers:

// Scrolling Registers
// https://gbdev.io/pandocs/Scrolling.html
Uint8* scx, scy;
Uint8* ly, lyc;
Uint8* wy;

// Palette Registers
// https://gbdev.io/pandocs/Palettes.html
Uint8* bgp;
Uint8* obp0, obp1;

/*  
    LCD Control Register
    https://gbdev.io/pandocs/LCDC.html
    
    Bit 7	LCD and PPU enable	0=Off, 1=On
    Bit 6	Window tile map area	0=9800-9BFF, 1=9C00-9FFF
    Bit 5	Window enable	0=Off, 1=On
    Bit 4	BG and Window tile data area	0=8800-97FF, 1=8000-8FFF
    Bit 3	BG tile map area	0=9800-9BFF, 1=9C00-9FFF
    Bit 2	OBJ size	0=8x8, 1=8x16
    Bit 1	OBJ enable	0=Off, 1=On
    Bit 0	BG and Window enable/priority	0=Off, 1=On
*/
Uint8 LCD_control = 0;

/*
    LCD Status Register
    https://gbdev.io/pandocs/STAT.html

    Bit 6 - LYC=LY STAT Interrupt source         (1=Enable) (Read/Write)
    Bit 5 - Mode 2 OAM STAT Interrupt source     (1=Enable) (Read/Write)
    Bit 4 - Mode 1 VBlank STAT Interrupt source  (1=Enable) (Read/Write)
    Bit 3 - Mode 0 HBlank STAT Interrupt source  (1=Enable) (Read/Write)
    Bit 2 - LYC=LY Flag                          (0=Different, 1=Equal) (Read Only)
    Bit 1-0 - Mode Flag                          (Mode 0-3, see below) (Read Only)
          0: HBlank
          1: VBlank
          2: Searching OAM
          3: Transferring Data to LCD Controller
*/
Uint8 LCD_status = 0;

void initRenderer(SDL_Renderer* renderer) 
{    
    gpu_renderer = renderer;
}

void drawPixel(SDL_Color* color, int x, int y, int width, int height) 
{
    SDL_Rect rect = {x, y, width, height};
    SDL_SetRenderDrawColor(gpu_renderer, color->r, color->b, color->g, color->a);
    SDL_RenderFillRect(gpu_renderer, &rect);
}

void drawTile(Tile* tile, Uint32 x, Uint32 y) 
{
    for (size_t i = 0; i < 16; i+=2) 
    {
        Uint8 byte1 = tile->pixels[i];
        Uint8 byte2 = tile->pixels[i+1];
        for (size_t j = 0; j < 8; j++) 
        {
            drawPixel(colors[((byte2 >> j & 1) << 1) | (byte1 >> j & 1)], 
                      x + (7 - j) * PIXEL_WIDTH, 
                      y + (i >> 1) * PIXEL_HEIGHT, 
                      PIXEL_WIDTH, 
                      PIXEL_HEIGHT);
        }
    }
}

void drawTileMap(TileMap* tilemap) 
{
    for (size_t i = 0; i < TILEMAP_SIZE; i++)
    {
        drawTile(&tiles[tilemap->tile_indexes[i]], (i % 32) * TILE_WIDTH, (i / 32) * TILE_HEIGHT);
    }
}

void drawSprite(OAM_Entry* entry) 
{
    drawTile(&tiles[entry->tile_index], (entry->x % 32) * TILE_WIDTH - 8, (entry->y / 32) * TILE_HEIGHT - 16);
}
