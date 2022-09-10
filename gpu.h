#ifndef GPU_H
#define GPU_H

#include "SDL2/SDL.h"

#define FPS 60
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 720
#define PIXEL_WIDTH WINDOW_WIDTH / 160
#define PIXEL_HEIGHT WINDOW_HEIGHT / 144
#define TILE_WIDTH PIXEL_WIDTH * 8
#define TILE_HEIGHT PIXEL_HEIGHT * 8
#define TILEMAP_SIZE 32 * 32

struct Tile
{
    Uint8 pixels[16];
};

struct TileMap
{
    Uint8 tile_indexes[TILEMAP_SIZE];
};

struct OAM_Entry
{
    Uint8 y;
    Uint8 x;
    Uint8 tile_index;
    /*
     *   Bit7   BG and Window over OBJ (0=No, 1=BG and Window colors 1-3 over the OBJ)
     *   Bit6   Y flip          (0=Normal, 1=Vertically mirrored)
     *   Bit5   X flip          (0=Normal, 1=Horizontally mirrored)
     *   Bit4   Palette number  **Non CGB Mode Only** (0=OBP0, 1=OBP1)
     *   Bit3   Tile VRAM-Bank  **CGB Mode Only**     (0=Bank 0, 1=Bank 1)
     *   Bit2-0 Palette number  **CGB Mode Only**     (OBP0-7)
    */
    Uint8 flags;
};

void drawPixel(SDL_Color* color, int x, int y, int width, int height);
void drawTile(Tile* tile, Uint32 x, Uint32 y);
void drawTileMap(TileMap* tilemap);
void drawSprite(OAM_Entry* entry);

void initRenderer(SDL_Renderer* renderer);

#endif
