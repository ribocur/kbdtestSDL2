#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
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
	if (!win) {
		fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}
	
	SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!ren) {
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(win);
		SDL_Quit();
		return EXIT_FAILURE;
	}

// Grab keyboard 
SDL_SetWindowKeyboardGrab(win,SDL_TRUE);

TTF_Init();
// Opening a font
TTF_Font* Sans = TTF_OpenFont("ttf/Hack-Regular.ttf",24 );
	if (!Sans) {
		fprintf(stderr, "TTF_OpenFont Error %s\n", TTF_GetError());
		SDL_DestroyWindow(win);
		SDL_Quit();
		return EXIT_FAILURE;
	}
// Surface that is needed to text render
SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans,"Hello" ,COLOR_WHITE );
  if (!surfaceMessage) {
  	fprintf(stderr,"TTFRenderSolid Error: %s\n", SDL_GetError());
  	SDL_DestroyWindow(win);
  	SDL_Quit();
  	return EXIT_FAILURE;
  }
// Converting text into texture
SDL_Texture* Message= SDL_CreateTextureFromSurface(ren,surfaceMessage );

	if(!Message) {
		fprintf(stderr,"CreateTextureFromSurface Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(win);
		SDL_Quit();
	}
// Surface message is not needed here
SDL_FreeSurface(surfaceMessage);

SDL_Rect* dest = {0,0,20,50};

SDL_RenderClear(ren);
SDL_RenderCopy(ren,Message,NULL ,dest );
SDL_RenderPresent(ren);

// Set to 1 to close window
int close = 0;
while (!close)
{
	// Get events
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				close=1;
				break;
			case SDL_KEYDOWN:
				printf( SDL_GetKeyName(event.key.keysym.sym), "%s\n\n");
				fflush(stdout);
				break;
		}
	}

SDL_RenderClear(ren);
SDL_RenderCopy(ren,Message ,NULL ,dest );
SDL_Delay(1000/60);
}

// Closing
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	TTF_Quit();
	SDL_Quit();

	return EXIT_SUCCESS;
}
