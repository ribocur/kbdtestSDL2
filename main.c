#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_ttf.h>


#define _COLOR(RED, GREEN, BLUE)    { RED, GREEN, BLUE, 0xFF }
const SDL_Color COLOR_WHITE         = _COLOR(0xFF, 0xFF, 0xFF);

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define BOX_SIZE 50
#define BOX_PADDING 10

typedef struct
{
	SDL_Rect rect;
	char key;
	short int pressed;
	short int was_pressed;
	
} Box;

int main()
{
	
	
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Window* win = SDL_CreateWindow("ZiobroKeyboardTester v2", 100, 100, 1200, 500, SDL_WINDOW_SHOWN);
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

Box boxes[20];
// Generate key boxes()i
int x = BOX_PADDING, y = BOX_PADDING;
for (int i =0;i < 20; ++i)
{
	boxes[i].rect = (SDL_Rect){x,y, BOX_SIZE, BOX_SIZE};
	boxes[i].key = 'A' + i;
	boxes[i].pressed = 0;

	x += BOX_SIZE + BOX_PADDING;
	if ( x + BOX_SIZE + BOX_PADDING > SCREEN_WIDTH)
	{
		x = BOX_PADDING;
		y += BOX_SIZE + BOX_PADDING;
	}
}
// Set to 1 to close window
int close = 0;
while (!close)
{
	// Get events
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		for (int i=0;i< 10;i++)
		{
			int tapeta = i;
			Box *box = &boxes[i];

			// Draw Box
			SDL_SetRenderDrawColor(ren,255 ,255 ,255 ,255 );
			SDL_RenderFillRect(ren,&box -> rect );

			// Draw letter
			SDL_Color color = box -> pressed ? (SDL_Color){0,255,0,255} : (SDL_Color){0,0,0,255};
			char letter[2] = {box->key, '\0'};
			SDL_Surface *surface = TTF_RenderText_Solid(Sans,letter ,color );
			SDL_Texture *texture = SDL_CreateTextureFromSurface(ren,surface );
			int text_width = surface->w, text_height = surface->h;
			SDL_FreeSurface(surface);
			SDL_Rect text_rect = {box->rect.x + (BOX_SIZE - text_width) /2 , box -> rect.y +( BOX_SIZE - text_height ) / 2, text_width,text_height};
			SDL_RenderCopy(ren,texture,NULL ,&text_rect );		
			SDL_DestroyTexture(texture);
		}
		switch (event.type)
		{
			case SDL_QUIT:
				close=1;
				break;
			case SDL_KEYDOWN:
				// Debuging
				printf( SDL_GetKeyName(event.key.keysym.sym), "%s\n\n");
				fflush(stdout);
				boxes[1].pressed=1;
				break;
		}
	}
SDL_RenderPresent(ren);
SDL_Delay(1000/60);
}

// Closing
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	TTF_Quit();
	SDL_Quit();

	return EXIT_SUCCESS;
}
