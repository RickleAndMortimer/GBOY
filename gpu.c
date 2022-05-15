#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <stdio.h>

#define FPS 60
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 720
#define PIXEL_WIDTH WINDOW_WIDTH / 160
#define PIXEL_HEIGHT WINDOW_HEIGHT / 144

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

SDL_Color* colors[] = {&DARKEST_GREEN, &DARK_GREEN, &LIGHT_GREEN, &LIGHTEST_GREEN};
    
typedef struct 
{
   Uint8 pixels[16];
} Tile;

// Uint8 tiles[0x97FF-0x800];

Tile tiles[128];

void drawPixel(SDL_Color* color, Uint32 x, Uint32 y, Uint32 width, Uint32 height, SDL_Renderer* renderer) {
    SDL_Rect rect = {x, y, width, height};
    SDL_SetRenderDrawColor(renderer, color->r, color->b, color->g, color->a);
    SDL_RenderFillRect(renderer, &rect);
}

void drawTile(Tile* tile, Uint32 x, Uint32 y, SDL_Renderer* renderer) {
    for (size_t k = 0; k < 16; k+=2) {
	Uint8 byte1 = tile->pixels[k];
        Uint8 byte2 = tile->pixels[k+1];
	for (size_t j = 0; j < 8; j++) {
	    printf("%d ", ((byte2 >> j & 1) << 1) | (byte1 >> j & 1));
	    drawPixel(colors[((byte2 >> j & 1) << 1) | (byte1 >> j & 1)], (7 - j)*PIXEL_WIDTH + x, (k >> 1)*PIXEL_HEIGHT + y, PIXEL_WIDTH, PIXEL_HEIGHT, renderer);
	}
	printf("\n");
    }
}

	
int main(int argc, char* argv[]) {
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;
	
    srand(time(NULL));
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_RenderClear(renderer);

    Tile j = { {0x3C, 0x7E, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x5E, 0x7E, 0x0A, 0x7C, 0x56, 0x38, 0x7C} };



    drawTile(&j, 10, 10, renderer);
    Tile one = { {0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D} };
    Tile two = { { 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99 } };
    Tile three = { {0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E } };
    Tile four = {{0xFF, 0x00, 0x7E, 0xFF, 0x85, 0x81, 0x89, 0x83, 0x93, 0x85, 0xA5, 0x8B, 0xC9, 0x97, 0x7E, 0xFF}};
    Tile five = {{ 0x7C, 0x7C, 0x00, 0xC6, 0xC6, 0x00, 0x00, 0xFE, 0xC6, 0xC6, 0x00, 0xC6, 0xC6, 0x00, 0x00, 0x00 }}; 


    drawTile(&four, 40, 80, renderer);

    drawTile(&five, 120, 120, renderer);

    while (1) {
	//Random color gen
	#if 0
	for (int y = 0; y < WINDOW_HEIGHT; y += 5) { 
	    for (int x = 0; x < WINDOW_WIDTH; x += 5) {	
		drawPixel(colors[rand() % 4], x, y, PIXEL_WIDTH, PIXEL_HEIGHT, renderer);
	    }
        }
	#endif 
	//Tile coloring

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
