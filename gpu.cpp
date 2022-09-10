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

Uint8 scx = 0, scy = 0;

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
    for (size_t k = 0; k < 16; k+=2) 
    {
        Uint8 byte1 = tile->pixels[k];
        Uint8 byte2 = tile->pixels[k+1];
        for (size_t j = 0; j < 8; j++) 
        {
            drawPixel(colors[((byte2 >> j & 1) << 1) | (byte1 >> j & 1)], 
                      x + (7 - j) * PIXEL_WIDTH, 
                      y + (k >> 1) * PIXEL_HEIGHT, 
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
