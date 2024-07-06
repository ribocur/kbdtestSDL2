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
#define COLOR(RED, GREEN, BLUE)     ((SDL_Color) { RED, GREEN, BLUE, 0xFF })

const SDL_Color NO_COLOR            = {0, 0, 0, 0};
const SDL_Color COLOR_BLACK         = _COLOR(0, 0, 0);
const SDL_Color COLOR_WHITE         = _COLOR(0xFF, 0xFF, 0xFF);
const SDL_Color COLOR_GRAY          = _COLOR(0x64, 0x64, 0x64);
const SDL_Color COLOR_DARK_GRAY     = _COLOR(0x1E, 0x1E, 0x1E);
const SDL_Color COLOR_LIGHT_GRAY    = _COLOR(0xC8, 0xC8, 0xC8);
const SDL_Color COLOR_RED           = _COLOR(0xF5, 0x3B, 0x56);
const SDL_Color COLOR_GREEN         = _COLOR(0x01, 0x9F, 0x13);
const SDL_Color COLOR_BLUE          = _COLOR(0x38, 0x95, 0xD3);
const SDL_Color COLOR_YELLOW        = _COLOR(0xF7, 0xDC, 0x11);
const SDL_Color COLOR_ORANGE        = _COLOR(0xFF, 0x85, 0);
const SDL_Color COLOR_PINK          = _COLOR(0xFF, 0, 0xCE);
const SDL_Color COLOR_VIOLET        = _COLOR(0x91, 0, 0xFF);


#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define BOX_SIZE 50
#define BOX_PADDING 10

typedef struct
{
	SDL_Rect rect;
	char key;
	// 0 = not pressed
	// 1 = pressed
	// 2 = was pressed
	short int state;
} Box;

void setup_standard_box()
{
	// Surface to render text onto
}
void setup_standard_box_pressed()
{
	
}
void setup_standard_box_was_pressed()
{
	
}

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
				// Debuging
				printf("KeyDown: ");
				printf(SDL_GetScancodeName(event.key.keysym.scancode), "%s");
				printf("\n");
				fflush(stdout);
				break;
			case SDL_KEYUP:
				printf("KeyUp: ");
				printf(SDL_GetScancodeName(event.key.keysym.scancode), "%s");
				printf("\n");
				fflush(stdout);
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
