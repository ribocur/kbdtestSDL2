#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_ttf.h>

int main()
{
#define _COLOR(RED, GREEN, BLUE)    { RED, GREEN, BLUE, 0xFF }
const SDL_Color COLOR_WHITE         = _COLOR(0xFF, 0xFF, 0xFF);
	
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Window* win = SDL_CreateWindow("ZiobroKeyboardTester v2", 100, 100, 620, 387, SDL_WINDOW_SHOWN);
	if (win == NULL) {
		fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == NULL) {
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(win);
		SDL_Quit();
		return EXIT_FAILURE;
	}

TTF_Init();
// Opening a font
TTF_Font* Sans = TTF_OpenFont("Sans.ttf",24 );

// Surface that is needed to text render
SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans,"Hello" ,COLOR_WHITE );

// Converting text into texture
SDL_Texture* Message= SDL_CreateTextureFromSurface(ren,surfaceMessage );

// Create rect for text
SDL_Rect Message_rect;
SDL_RenderCopy(ren,Message ,NULL ,&Message_rect );

	for (int i = 0; i < 20; i++) {
		SDL_RenderClear(ren);
		SDL_RenderPresent(ren);
		SDL_Delay(100);
	}

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return EXIT_SUCCESS;
}
