#include <time.h>
#include "gpu.h"

extern Tile tiles[];
extern TileMap background_tilemap;
extern TileMap window_tilemap;

int main(int argc, char* argv[]) {
    SDL_Event event;
    SDL_Renderer* renderer;
    SDL_Window* window;
	
    srand(time(NULL));
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_RenderClear(renderer);
    initRenderer(renderer);

    // Draw individual tiles
    // A
    tiles[0] = { { 0x3C, 0x7E, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x5E, 0x7E, 0x0A, 0x7C, 0x56, 0x38, 0x7C } }; 
    tiles[1] = { { 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D } };
    tiles[2] = { { 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99 } };
    tiles[3] = { { 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E } };
    tiles[4] = { { 0xFF, 0x00, 0x7E, 0xFF, 0x85, 0x81, 0x89, 0x83, 0x93, 0x85, 0xA5, 0x8B, 0xC9, 0x97, 0x7E, 0xFF } };
    tiles[5] = { { 0x7C, 0x7C, 0x00, 0xC6, 0xC6, 0x00, 0x00, 0xFE, 0xC6, 0xC6, 0x00, 0xC6, 0xC6, 0x00, 0x00, 0x00 } }; 

    /*
    drawTile(&tiles[0], 0, 0, renderer);
    drawTile(&tiles[4], 40, 0, renderer);
    drawTile(&tiles[5], 80, 0, renderer);
    */

    // Draw tile map
    background_tilemap.tile_indexes[0] = 0;
    background_tilemap.tile_indexes[1] = 1;
    background_tilemap.tile_indexes[2] = 3;
    background_tilemap.tile_indexes[3] = 4;
    background_tilemap.tile_indexes[4] = 2;
    background_tilemap.tile_indexes[5] = 5;

    drawTileMap(&background_tilemap);

    while (true) {
        //Random color gen
        #if 0
            for (int y = 0; y < WINDOW_HEIGHT; y += 5) 
            { 
                for (int x = 0; x < WINDOW_WIDTH; x += 5) 
                {	
                    drawPixel(colors[rand() % 4], x, y, PIXEL_WIDTH, PIXEL_HEIGHT, renderer);
                }
            }
        #endif 

        SDL_RenderPresent(renderer);
        SDL_Delay(1000/FPS);
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
