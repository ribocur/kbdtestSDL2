#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

#define _COLOR(RED, GREEN, BLUE) {RED, GREEN, BLUE, 0xFF}
#define COLOR(RED, GREEN, BLUE) ((SDL_Color){RED, GREEN, BLUE, 0xFF})

const SDL_Color NO_COLOR = {0, 0, 0, 0};
const SDL_Color COLOR_BLACK = _COLOR(0, 0, 0);
const SDL_Color COLOR_WHITE = _COLOR(0xFF, 0xFF, 0xFF);
const SDL_Color COLOR_GRAY = _COLOR(0x64, 0x64, 0x64);
const SDL_Color COLOR_DARK_GRAY = _COLOR(0x1E, 0x1E, 0x1E);
const SDL_Color COLOR_LIGHT_GRAY = _COLOR(0xC8, 0xC8, 0xC8);
const SDL_Color COLOR_RED = _COLOR(0xF5, 0x3B, 0x56);
const SDL_Color COLOR_GREEN = _COLOR(0x01, 0x9F, 0x13);
const SDL_Color COLOR_BLUE = _COLOR(0x38, 0x95, 0xD3);
const SDL_Color COLOR_YELLOW = _COLOR(0xF7, 0xDC, 0x11);
const SDL_Color COLOR_ORANGE = _COLOR(0xFF, 0x85, 0);
const SDL_Color COLOR_PINK = _COLOR(0xFF, 0, 0xCE);
const SDL_Color COLOR_VIOLET = _COLOR(0x91, 0, 0xFF);

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define BOX_SIZE 50
#define BOX_PADDING 10

typedef struct {
  SDL_Rect rect;
  char key;
  // 0 = not pressed
  // 1 = pressed
  // 2 = was pressed
  short int state;
} Box;

int last_pos_x = 0 - BOX_SIZE;
int last_pos_y = 0;
SDL_Rect setup_position() {
  SDL_Rect Rectangle;
  Rectangle.x = last_pos_x + BOX_SIZE;
  last_pos_x += BOX_SIZE;
  Rectangle.y = last_pos_y;
  Rectangle.w = BOX_SIZE;
  Rectangle.h = BOX_SIZE;
  return Rectangle;
}
// Setup standard box textures
SDL_Texture *setup_standard_box_texture(TTF_Font *font, char text[],
                                        SDL_Renderer *ren, int state) {
  SDL_Surface *surfaceMessage;
  // Surface to render text onto
  switch (state) {
  case 0:
    surfaceMessage = TTF_RenderText_Solid(font, text, COLOR_RED);
    break;
  case 1:
    surfaceMessage = TTF_RenderText_Solid(font, text, COLOR_WHITE);
    break;
  case 2:
    surfaceMessage = TTF_RenderText_Solid(font, text, COLOR_GREEN);
    break;
  }
  // Convert to texture
  SDL_Texture *Message = SDL_CreateTextureFromSurface(ren, surfaceMessage);
  SDL_FreeSurface(surfaceMessage);
  return Message;
  SDL_DestroyTexture(Message);
}

int main() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Window *win = SDL_CreateWindow("ZiobroKeyboardTester v2", 100, 100, 1200,
                                     500, SDL_WINDOW_SHOWN);
  if (!win) {
    fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Renderer *ren = SDL_CreateRenderer(
      win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!ren) {
    fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(win);
    SDL_Quit();
    return EXIT_FAILURE;
  }

  // Grab keyboard
  SDL_SetWindowKeyboardGrab(win, SDL_TRUE);

  TTF_Init();
  // Opening a font
  TTF_Font *Sans = TTF_OpenFont("ttf/Hack-Regular.ttf", 24);
  if (!Sans) {
    fprintf(stderr, "TTF_OpenFont Error %s\n", TTF_GetError());
    SDL_DestroyWindow(win);
    SDL_Quit();
    return EXIT_FAILURE;
  }
  SDL_Rect first_row_rect[16] = {};
  for (int i = 0; i < 16; i++) {
    printf("Setup positions %d\n", i);

    first_row_rect[i] = setup_position();
  }
  char first_row[16][10] = {"Esc", "F1",    "F2",    "F3",   "F4",  "F5",
                            "F6",  "F7",    "F8",    "F9",   "F10", "F11",
                            "F12", "PrtSc", "ScrLk", "Pause"};
  int first_row_state[16] = {0};
  SDL_Texture *first_row_non_presed[16] = {};
  SDL_Texture *first_row_pressed[16] = {};
  SDL_Texture *first_row_was_pressed[16] = {};

  for (int i = 0; i < 16; i++) {
    SDL_Texture *temp = setup_standard_box_texture(Sans, first_row[i], ren, 0);
    first_row_non_presed[i] =
        setup_standard_box_texture(Sans, first_row[i], ren, 0);
    first_row_pressed[i] =
        setup_standard_box_texture(Sans, first_row[i], ren, 1);
    first_row_was_pressed[i] =
        setup_standard_box_texture(Sans, first_row[i], ren, 2);
  }

  // Set to 1 to close window
  int close = 0;
  while (!close) {
    // Get events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        close = 1;
        break;
      case SDL_KEYDOWN:
        // Debuging
        printf("KeyDown: ");
        printf(SDL_GetScancodeName(event.key.keysym.scancode), "%s");
        printf("\n");
        fflush(stdout);

        switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_ESCAPE:
          first_row_state[0] = 1;
          break;
        case SDL_SCANCODE_F1:
          first_row_state[1] = 1;
          break;
        case SDL_SCANCODE_F2:
          first_row_state[2] = 1;
          break;
        case SDL_SCANCODE_F3:
          first_row_state[3] = 1;
          break;
        case SDL_SCANCODE_F4:
          first_row_state[4] = 1;
          break;
        case SDL_SCANCODE_F5:
          first_row_state[5] = 1;
          break;
        case SDL_SCANCODE_F6:
          first_row_state[6] = 1;
          break;
        case SDL_SCANCODE_F7:
          first_row_state[7] = 1;
          break;
        case SDL_SCANCODE_F8:
          first_row_state[8] = 1;
          break;
        case SDL_SCANCODE_F9:
          first_row_state[9] = 1;
          break;
        case SDL_SCANCODE_F10:
          first_row_state[10] = 1;
          break;
        case SDL_SCANCODE_F11:
          first_row_state[11] = 1;
          break;
        case SDL_SCANCODE_F12:
          first_row_state[12] = 1;
          break;
        case SDL_SCANCODE_PRINTSCREEN:
          first_row_state[13] = 1;
          break;
        case SDL_SCANCODE_SCROLLLOCK:
          first_row_state[14] = 1;
          break;
        case SDL_SCANCODE_PAUSE:
          first_row_state[15] = 1;
          break;
        }
        break;
      case SDL_KEYUP:
        printf("KeyUp: ");
        printf(SDL_GetScancodeName(event.key.keysym.scancode), "%s");
        printf("\n");
        fflush(stdout);
        switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_ESCAPE:
          first_row_state[0] = 2;
          break;
        case SDL_SCANCODE_F1:
          first_row_state[1] = 2;
          break;
        case SDL_SCANCODE_F2:
          first_row_state[2] = 2;
          break;
        case SDL_SCANCODE_F3:
          first_row_state[3] = 2;
          break;
        case SDL_SCANCODE_F4:
          first_row_state[4] = 2;
          break;
        case SDL_SCANCODE_F5:
          first_row_state[5] = 2;
          break;
        case SDL_SCANCODE_F6:
          first_row_state[6] = 2;
          break;
        case SDL_SCANCODE_F7:
          first_row_state[7] = 2;
          break;
        case SDL_SCANCODE_F8:
          first_row_state[8] = 2;
          break;
        case SDL_SCANCODE_F9:
          first_row_state[9] = 2;
          break;
        case SDL_SCANCODE_F10:
          first_row_state[10] = 2;
          break;
        case SDL_SCANCODE_F11:
          first_row_state[11] = 2;
          break;
        case SDL_SCANCODE_F12:
          first_row_state[12] = 2;
          break;
        case SDL_SCANCODE_PRINTSCREEN:
          first_row_state[13] = 2;
          break;
        case SDL_SCANCODE_SCROLLLOCK:
          first_row_state[14] = 2;
          break;
        case SDL_SCANCODE_PAUSE:
          first_row_state[15] = 2;
          break;
        }
		break;
      }
    }
    for (int x = 0; x < 16; x++) {
      switch (first_row_state[x]) {
      case 0:
        SDL_RenderCopy(ren, first_row_non_presed[x], NULL, &first_row_rect[x]);
        break;
      case 1:
        SDL_RenderCopy(ren, first_row_pressed[x], NULL, &first_row_rect[x]);
        break;
      case 2:
        SDL_RenderCopy(ren, first_row_was_pressed[x], NULL, &first_row_rect[x]);
        break;
      }
    }
    SDL_RenderPresent(ren);
    SDL_Delay(100);
    SDL_RenderClear(ren);
  }

  // Closing
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  TTF_Quit();
  SDL_Quit();

  return EXIT_SUCCESS;
}
