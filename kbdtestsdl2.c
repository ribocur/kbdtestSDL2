#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
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
#include <emmintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

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
#define BOX_SIZE 60
#define BOX_PADDING 10

int last_pos_x = 0 - BOX_SIZE;
int last_pos_y = 0;

char* last_pressed_key = "None";
SDL_Rect setup_position() {
  SDL_Rect Rectangle;
  Rectangle.x = last_pos_x + BOX_SIZE;
  last_pos_x += BOX_SIZE;
  Rectangle.y = last_pos_y;
  Rectangle.w = BOX_SIZE;
  Rectangle.h = BOX_SIZE;
  return Rectangle;
}
SDL_Rect setup_poistion_with_custom_size(float width, float height) {
  SDL_Rect Rectangle;
  Rectangle.x = last_pos_x + BOX_SIZE;
  last_pos_x += width;
  Rectangle.y = last_pos_y;
  Rectangle.w = width;
  Rectangle.h = height;
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

  SDL_Window *win = SDL_CreateWindow("ZiobroKeyboardTester v2", 100, 100, 1501,
                                     450, SDL_WINDOW_SHOWN);
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
  TTF_Font *Sans = TTF_OpenFont("ttf/Hack-Regular.ttf", 1000);
  if (!Sans) {
    fprintf(stderr, "TTF_OpenFont Error %s\n", TTF_GetError());
    SDL_DestroyWindow(win);
    SDL_Quit();
    return EXIT_FAILURE;
  }
  // First row
  SDL_Rect first_row_rect[16] = {};
  for (int i = 0; i < 16; i++) {
    // Empty space between function keys
    switch (i) {
    case 1:
      last_pos_x += BOX_SIZE;
      first_row_rect[i] = setup_position();
      break;
    case 5:
      last_pos_x += BOX_SIZE;
      first_row_rect[i] = setup_position();
      break;
     case 9:
      last_pos_x += BOX_SIZE;
      first_row_rect[i] = setup_position();
      break;
    case 13:
      last_pos_x += BOX_SIZE;
      first_row_rect[i] = setup_position();
      break;
    default:
      first_row_rect[i] = setup_position();
      break;
    }
  }

  char first_row[16][10] = {"Esc", "F1",    "F2",    "F3",   "F4",  "F5",
                            "F6",  "F7",    "F8",    "F9",   "F10", "F11",
                            "F12", "PrtSc", "ScrLk", "Pause"};
  int first_row_state[16] = {0};
  SDL_Texture *first_row_non_presed[16] = {};
  SDL_Texture *first_row_pressed[16] = {};
  SDL_Texture *first_row_was_pressed[16] = {};

  for (int i = 0; i < 16; i++) {
    first_row_non_presed[i] =
        setup_standard_box_texture(Sans, first_row[i], ren, 0);
    first_row_pressed[i] =
        setup_standard_box_texture(Sans, first_row[i], ren, 1);
    first_row_was_pressed[i] =
        setup_standard_box_texture(Sans, first_row[i], ren, 2);
  }

  // Second Row
  SDL_Rect second_row_rect[21] = {};
  // Increment position for second row
  last_pos_x = 0 - BOX_SIZE;
  last_pos_y += BOX_SIZE;
  for (int i = 0; i < 21; i++) {
    switch (i) {
    case 13:
      second_row_rect[i] =
          setup_poistion_with_custom_size((BOX_SIZE * 3), BOX_SIZE);
      break;
    case 14:
      last_pos_x += BOX_SIZE;
      second_row_rect[i] = setup_position();
      break;
    case 17:
      last_pos_x += BOX_SIZE;
      second_row_rect[i] = setup_position();
      break;
    default:
      second_row_rect[i] = setup_position();
    }
  }
  // Second row
  char second_row[21][10] = {
      "`", "1", "2",         "3",   "4",    "5",    "6",   "7", "8", "9", "0",
      "-", "=", "Backspace", "Ins", "Home", "PgUP", "Num", "/", "*", "-"};
  int second_row_state[21] = {0};
  SDL_Texture *second_row_non_pressed[21] = {};
  SDL_Texture *second_row_pressed[21] = {};
  SDL_Texture *second_row_was_pressed[21] = {};

  for (int i = 0; i < 21; i++) {
    second_row_non_pressed[i] =
        setup_standard_box_texture(Sans, second_row[i], ren, 0);
    second_row_pressed[i] =
        setup_standard_box_texture(Sans, second_row[i], ren, 1);
    second_row_was_pressed[i] =
        setup_standard_box_texture(Sans, second_row[i], ren, 2);
  }

  // Third row
  char third_row[21][10] = {"Tab", "Q",   "W",      "E", "R", "T", "Y",
                            "U",   "I",   "O",      "P", "[", "]", "\\",
                            "Del", "End", "PgDown", "7", "8", "9", "+"};
  int third_row_state[21] = {0};
  SDL_Texture *third_row_non_pressed[21] = {};
  SDL_Texture *third_row_pressed[21] = {};
  SDL_Texture *third_row_was_pressed[21] = {};

  for (int i = 0; i < 21; i++) {
    third_row_non_pressed[i] =
        setup_standard_box_texture(Sans, third_row[i], ren, 0);
    third_row_pressed[i] =
        setup_standard_box_texture(Sans, third_row[i], ren, 1);
    third_row_was_pressed[i] =
        setup_standard_box_texture(Sans, third_row[i], ren, 2);
  }

  last_pos_x = 0 - BOX_SIZE;
  last_pos_y += BOX_SIZE;
  SDL_Rect third_row_rect[21] = {};
  for (int i = 0; i < 21; i++) {
    switch (i) {
    case 0:
      third_row_rect[i] =
          setup_poistion_with_custom_size((BOX_SIZE * 1.5), BOX_SIZE);
      break;
    case 13:
      third_row_rect[i] =
          setup_poistion_with_custom_size((BOX_SIZE * 2.5), BOX_SIZE);
      break;
    case 14:
      last_pos_x += BOX_SIZE;
      third_row_rect[i] = setup_position();
      break;
    case 17:
      last_pos_x += BOX_SIZE;
      third_row_rect[i] = setup_position();
      break;
    case 20:
      third_row_rect[i] =
          setup_poistion_with_custom_size(BOX_SIZE, (BOX_SIZE * 2));
      break;
    default:
      third_row_rect[i] = setup_position();
      break;
    }
  }
  // Fourth row
  char fourth_row[16][10] = {"CapsLock", "A", "S", "D", "F",     "G", "H", "J",
                             "K",        "L", ";", "'", "Enter", "4", "5", "6"};
  int fourth_row_state[16] = {0};
  SDL_Texture *fourth_row_non_pressed[16] = {};
  SDL_Texture *fourth_row_pressed[16] = {};
  SDL_Texture *fourth_row_was_pressed[16] = {};

  for (int i = 0; i < 16; i++) {
    fourth_row_non_pressed[i] =
        setup_standard_box_texture(Sans, fourth_row[i], ren, 0);
    fourth_row_pressed[i] =
        setup_standard_box_texture(Sans, fourth_row[i], ren, 1);
    fourth_row_was_pressed[i] =
        setup_standard_box_texture(Sans, fourth_row[i], ren, 2);
  }

  last_pos_x = 0 - BOX_SIZE;
  last_pos_y += BOX_SIZE;
  SDL_Rect fourth_row_rect[16] = {};
  for (int i = 0; i < 16; i++) {
    switch (i) {
    case 0:
      fourth_row_rect[i] =
          setup_poistion_with_custom_size((BOX_SIZE * 2), BOX_SIZE);
      break;
    case 12:
      fourth_row_rect[i] =
          setup_poistion_with_custom_size((BOX_SIZE * 3), BOX_SIZE);
      break;
    case 13:
      last_pos_x += (BOX_SIZE * 5);
      fourth_row_rect[i] = setup_position();
      break;
    default:
      fourth_row_rect[i] = setup_position();
      break;
    }
  }

  // Fifth row
  char fifth_row[17][10] = {"Shift", "Z", "X",     "C", "V", "B", "N", "M", ",",
                            ".",     "/", "Shift", "^", "1", "2", "3", "E"};
  int fifth_row_state[17] = {0};
  SDL_Texture *fifth_row_non_pressed[17] = {};
  SDL_Texture *fifth_row_pressed[17] = {};
  SDL_Texture *fifth_row_was_pressed[17] = {};

  for (int i = 0; i < 17; i++) {
    fifth_row_non_pressed[i] =
        setup_standard_box_texture(Sans, fifth_row[i], ren, 0);
    fifth_row_pressed[i] =
        setup_standard_box_texture(Sans, fifth_row[i], ren, 1);
    fifth_row_was_pressed[i] =
        setup_standard_box_texture(Sans, fifth_row[i], ren, 2);
  }

  last_pos_x = 0 - BOX_SIZE;
  last_pos_y += BOX_SIZE;
  SDL_Rect fifth_row_rect[17] = {};
  for (int i = 0; i < 17; i++) {
    switch (i) {
    case 0:
      fifth_row_rect[i] =
          setup_poistion_with_custom_size((BOX_SIZE * 2.5), BOX_SIZE);
      break;
    case 11:
      fifth_row_rect[i] =
          setup_poistion_with_custom_size((BOX_SIZE * 3.5), BOX_SIZE);
      break;
    case 12:
      last_pos_x += (BOX_SIZE * 2);
      fifth_row_rect[i] = setup_position();
      break;
    case 13:
      last_pos_x += (BOX_SIZE * 2);
      fifth_row_rect[i] = setup_position();
      break;
    case 16:
      fifth_row_rect[i] =
          setup_poistion_with_custom_size(BOX_SIZE, (BOX_SIZE * 2));
      break;
    default:
      fifth_row_rect[i] = setup_position();
      break;
    }
  }

  // Sixth row
  char sixth_row[13][10] = {"CTRL", "WIN", "Alt", "Space", "Alt", "Win", "Menu",
                            "CTRL", "<",   "v",   ">",     "0",   "."};
  int sixth_row_state[13] = {0};
  SDL_Texture *sixth_row_non_pressed[13] = {};
  SDL_Texture *sixth_row_pressed[13] = {};
  SDL_Texture *sixth_row_was_pressed[13] = {};

  for (int i = 0; i < 13; i++) {
    sixth_row_non_pressed[i] =
        setup_standard_box_texture(Sans, sixth_row[i], ren, 0);
    sixth_row_pressed[i] =
        setup_standard_box_texture(Sans, sixth_row[i], ren, 1);
    sixth_row_was_pressed[i] =
        setup_standard_box_texture(Sans, sixth_row[i], ren, 2);
  }

  last_pos_x = 0 - BOX_SIZE;
  last_pos_y += BOX_SIZE;
  SDL_Rect sixth_row_rect[13] = {};
  for (int i = 0; i < 13; i++) {
    switch (i) {
    case 0:
      sixth_row_rect[i] =
          setup_poistion_with_custom_size((BOX_SIZE * 2), BOX_SIZE);
      break;
    case 3:
      sixth_row_rect[i] =
          setup_poistion_with_custom_size((BOX_SIZE * 7), BOX_SIZE);
      break;
    case 7:
      sixth_row_rect[i] =
          setup_poistion_with_custom_size((BOX_SIZE * 2), BOX_SIZE);
      break;
    case 8:
      last_pos_x += BOX_SIZE;
      sixth_row_rect[i] = setup_position();
      break;
    case 11:
      last_pos_x += BOX_SIZE;
      sixth_row_rect[i] = setup_position();
      break;
    case 12:
      sixth_row_rect[i] =
          setup_poistion_with_custom_size((BOX_SIZE * 2), BOX_SIZE);
      break;
    default:
      sixth_row_rect[i] = setup_position();
      break;
    }
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

        last_pressed_key = SDL_GetScancodeName(event.key.keysym.scancode);
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
        case SDL_SCANCODE_GRAVE:
          second_row_state[0] = 1;
          break;
        case SDL_SCANCODE_1:
          second_row_state[1] = 1;
          break;
        case SDL_SCANCODE_2:
          second_row_state[2] = 1;
          break;
        case SDL_SCANCODE_3:
          second_row_state[3] = 1;
          break;
        case SDL_SCANCODE_4:
          second_row_state[4] = 1;
          break;
        case SDL_SCANCODE_5:
          second_row_state[5] = 1;
          break;
        case SDL_SCANCODE_6:
          second_row_state[6] = 1;
          break;
        case SDL_SCANCODE_7:
          second_row_state[7] = 1;
          break;
        case SDL_SCANCODE_8:
          second_row_state[8] = 1;
          break;
        case SDL_SCANCODE_9:
          second_row_state[9] = 1;
          break;
        case SDL_SCANCODE_0:
          second_row_state[10] = 1;
          break;
        case SDL_SCANCODE_MINUS:
          second_row_state[11] = 1;
          break;
        case SDL_SCANCODE_EQUALS:
          second_row_state[12] = 1;
          break;
        case SDL_SCANCODE_BACKSPACE:
          second_row_state[13] = 1;
          break;
        case SDL_SCANCODE_INSERT:
          second_row_state[14] = 1;
          break;
        case SDL_SCANCODE_HOME:
          second_row_state[15] = 1;
          break;
        case SDL_SCANCODE_PAGEUP:
          second_row_state[16] = 1;
          break;
        // ON PC Numlock, CLEAR on Mac
        case SDL_SCANCODE_NUMLOCKCLEAR:
          second_row_state[17] = 1;
          break;
        case SDL_SCANCODE_KP_DIVIDE:
          second_row_state[18] = 1;
          break;
        case SDL_SCANCODE_KP_MULTIPLY:
          second_row_state[19] = 1;
          break;
        case SDL_SCANCODE_KP_MINUS:
          second_row_state[20] = 1;
          break;
        case SDL_SCANCODE_TAB:
          third_row_state[0] = 1;
          break;
        case SDL_SCANCODE_Q:
          third_row_state[1] = 1;
          break;
        case SDL_SCANCODE_W:
          third_row_state[2] = 1;
          break;
        case SDL_SCANCODE_E:
          third_row_state[3] = 1;
          break;
        case SDL_SCANCODE_R:
          third_row_state[4] = 1;
          break;
        case SDL_SCANCODE_T:
          third_row_state[5] = 1;
          break;
        case SDL_SCANCODE_Y:
          third_row_state[6] = 1;
          break;
        case SDL_SCANCODE_U:
          third_row_state[7] = 1;
          break;
        case SDL_SCANCODE_I:
          third_row_state[8] = 1;
          break;
        case SDL_SCANCODE_O:
          third_row_state[9] = 1;
          break;
        case SDL_SCANCODE_P:
          third_row_state[10] = 1;
          break;
        case SDL_SCANCODE_LEFTBRACKET:
          third_row_state[11] = 1;
          break;
        case SDL_SCANCODE_RIGHTBRACKET:
          third_row_state[12] = 1;
          break;
        case SDL_SCANCODE_BACKSLASH:
          third_row_state[13] = 1;
          break;
        case SDL_SCANCODE_DELETE:
          third_row_state[14] = 1;
          break;
        case SDL_SCANCODE_END:
          third_row_state[15] = 1;
          break;
        case SDL_SCANCODE_PAGEDOWN:
          third_row_state[16] = 1;
          break;
        case SDL_SCANCODE_KP_7:
          third_row_state[17] = 1;
          break;
        case SDL_SCANCODE_KP_8:
          third_row_state[18] = 1;
          break;
        case SDL_SCANCODE_KP_9:
          third_row_state[19] = 1;
          break;
        case SDL_SCANCODE_KP_PLUS:
          third_row_state[20] = 1;
          break;

        case SDL_SCANCODE_CAPSLOCK:
          fourth_row_state[0] = 1;
          break;
        case SDL_SCANCODE_A:
          fourth_row_state[1] = 1;
          break;
        case SDL_SCANCODE_S:
          fourth_row_state[2] = 1;
          break;
        case SDL_SCANCODE_D:
          fourth_row_state[3] = 1;
          break;
        case SDL_SCANCODE_F:
          fourth_row_state[4] = 1;
          break;
        case SDL_SCANCODE_G:
          fourth_row_state[5] = 1;
          break;
        case SDL_SCANCODE_H:
          fourth_row_state[6] = 1;
          break;
        case SDL_SCANCODE_J:
          fourth_row_state[7] = 1;
          break;
        case SDL_SCANCODE_K:
          fourth_row_state[8] = 1;
          break;
        case SDL_SCANCODE_L:
          fourth_row_state[9] = 1;
          break;
        case SDL_SCANCODE_SEMICOLON:
          fourth_row_state[10] = 1;
          break;
        case SDL_SCANCODE_APOSTROPHE:
          fourth_row_state[11] = 1;
          break;
        case SDL_SCANCODE_RETURN:
          fourth_row_state[12] = 1;
          break;
        case SDL_SCANCODE_KP_4:
          fourth_row_state[13] = 1;
          break;
        case SDL_SCANCODE_KP_5:
          fourth_row_state[14] = 1;
          break;
        case SDL_SCANCODE_KP_6:
          fourth_row_state[15] = 1;
          break;

        case SDL_SCANCODE_LSHIFT:
          fifth_row_state[0] = 1;
          break;
        case SDL_SCANCODE_Z:
          fifth_row_state[1] = 1;
          break;
        case SDL_SCANCODE_X:
          fifth_row_state[2] = 1;
          break;
        case SDL_SCANCODE_C:
          fifth_row_state[3] = 1;
          break;
        case SDL_SCANCODE_V:
          fifth_row_state[4] = 1;
          break;
        case SDL_SCANCODE_B:
          fifth_row_state[5] = 1;
          break;
        case SDL_SCANCODE_N:
          fifth_row_state[6] = 1;
          break;
        case SDL_SCANCODE_M:
          fifth_row_state[7] = 1;
          break;
        case SDL_SCANCODE_COMMA:
          fifth_row_state[8] = 1;
          break;
        case SDL_SCANCODE_PERIOD:
          fifth_row_state[9] = 1;
          break;
        case SDL_SCANCODE_SLASH:
          fifth_row_state[10] = 1;
          break;
        case SDL_SCANCODE_RSHIFT:
          fifth_row_state[11] = 1;
          break;
        case SDL_SCANCODE_UP:
          fifth_row_state[12] = 1;
          break;
        case SDL_SCANCODE_KP_1:
          fifth_row_state[13] = 1;
          break;
        case SDL_SCANCODE_KP_2:
          fifth_row_state[14] = 1;
          break;
        case SDL_SCANCODE_KP_3:
          fifth_row_state[15] = 1;
          break;
        case SDL_SCANCODE_KP_ENTER:
          fifth_row_state[16] = 1;
          break;

        case SDL_SCANCODE_LCTRL:
          sixth_row_state[0] = 1;
          break;
        case SDL_SCANCODE_LGUI:
          sixth_row_state[1] = 1;
          break;
        case SDL_SCANCODE_LALT:
          sixth_row_state[2] = 1;
          break;
        case SDL_SCANCODE_SPACE:
          sixth_row_state[3] = 1;
          break;
        case SDL_SCANCODE_RALT:
          sixth_row_state[4] = 1;
          break;
        case SDL_SCANCODE_RGUI:
          sixth_row_state[5] = 1;
          break;
        case SDL_SCANCODE_APPLICATION:
          sixth_row_state[6] = 1;
          break;
        case SDL_SCANCODE_RCTRL:
          sixth_row_state[7] = 1;
          break;
        case SDL_SCANCODE_LEFT:
          sixth_row_state[8] = 1;
          break;
        case SDL_SCANCODE_DOWN:
          sixth_row_state[9] = 1;
          break;
        case SDL_SCANCODE_RIGHT:
          sixth_row_state[10] = 1;
          break;
        case SDL_SCANCODE_KP_0:
          sixth_row_state[11] = 1;
          break;
        case SDL_SCANCODE_KP_PERIOD:
          sixth_row_state[12] = 1;
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
        case SDL_SCANCODE_GRAVE:
          second_row_state[0] = 2;
          break;
        case SDL_SCANCODE_1:
          second_row_state[1] = 2;
          break;
        case SDL_SCANCODE_2:
          second_row_state[2] = 2;
          break;
        case SDL_SCANCODE_3:
          second_row_state[3] = 2;
          break;
        case SDL_SCANCODE_4:
          second_row_state[4] = 2;
          break;
        case SDL_SCANCODE_5:
          second_row_state[5] = 2;
          break;
        case SDL_SCANCODE_6:
          second_row_state[6] = 2;
          break;
        case SDL_SCANCODE_7:
          second_row_state[7] = 2;
          break;
        case SDL_SCANCODE_8:
          second_row_state[8] = 2;
          break;
        case SDL_SCANCODE_9:
          second_row_state[9] = 2;
          break;
        case SDL_SCANCODE_0:
          second_row_state[10] = 2;
          break;
        case SDL_SCANCODE_MINUS:
          second_row_state[11] = 2;
          break;
        case SDL_SCANCODE_EQUALS:
          second_row_state[12] = 2;
          break;
        case SDL_SCANCODE_BACKSPACE:
          second_row_state[13] = 2;
          break;
        case SDL_SCANCODE_INSERT:
          second_row_state[14] = 2;
          break;
        case SDL_SCANCODE_HOME:
          second_row_state[15] = 2;
          break;
        case SDL_SCANCODE_PAGEUP:
          second_row_state[16] = 2;
          break;
        // ON PC Numlock, CLEAR on Mac
        case SDL_SCANCODE_NUMLOCKCLEAR:
          second_row_state[17] = 2;
          break;
        case SDL_SCANCODE_KP_DIVIDE:
          second_row_state[18] = 2;
          break;
        case SDL_SCANCODE_KP_MULTIPLY:
          second_row_state[19] = 2;
          break;
        case SDL_SCANCODE_KP_MINUS:
          second_row_state[20] = 2;
          break;
        case SDL_SCANCODE_TAB:
          third_row_state[0] = 2;
          break;
        case SDL_SCANCODE_Q:
          third_row_state[1] = 2;
          break;
        case SDL_SCANCODE_W:
          third_row_state[2] = 2;
          break;
        case SDL_SCANCODE_E:
          third_row_state[3] = 2;
          break;
        case SDL_SCANCODE_R:
          third_row_state[4] = 2;
          break;
        case SDL_SCANCODE_T:
          third_row_state[5] = 2;
          break;
        case SDL_SCANCODE_Y:
          third_row_state[6] = 2;
          break;
        case SDL_SCANCODE_U:
          third_row_state[7] = 2;
          break;
        case SDL_SCANCODE_I:
          third_row_state[8] = 2;
          break;
        case SDL_SCANCODE_O:
          third_row_state[9] = 2;
          break;
        case SDL_SCANCODE_P:
          third_row_state[10] = 2;
          break;
        case SDL_SCANCODE_LEFTBRACKET:
          third_row_state[11] = 2;
          break;
        case SDL_SCANCODE_RIGHTBRACKET:
          third_row_state[12] = 2;
          break;
        case SDL_SCANCODE_BACKSLASH:
          third_row_state[13] = 2;
          break;
        case SDL_SCANCODE_DELETE:
          third_row_state[14] = 2;
          break;
        case SDL_SCANCODE_END:
          third_row_state[15] = 2;
          break;
        case SDL_SCANCODE_PAGEDOWN:
          third_row_state[16] = 2;
          break;
        case SDL_SCANCODE_KP_7:
          third_row_state[17] = 2;
          break;
        case SDL_SCANCODE_KP_8:
          third_row_state[18] = 2;
          break;
        case SDL_SCANCODE_KP_9:
          third_row_state[19] = 2;
          break;
        case SDL_SCANCODE_KP_PLUS:
          third_row_state[20] = 2;
          break;

        case SDL_SCANCODE_CAPSLOCK:
          fourth_row_state[0] = 2;
          break;
        case SDL_SCANCODE_A:
          fourth_row_state[1] = 2;
          break;
        case SDL_SCANCODE_S:
          fourth_row_state[2] = 2;
          break;
        case SDL_SCANCODE_D:
          fourth_row_state[3] = 2;
          break;
        case SDL_SCANCODE_F:
          fourth_row_state[4] = 2;
          break;
        case SDL_SCANCODE_G:
          fourth_row_state[5] = 2;
          break;
        case SDL_SCANCODE_H:
          fourth_row_state[6] = 2;
          break;
        case SDL_SCANCODE_J:
          fourth_row_state[7] = 2;
          break;
        case SDL_SCANCODE_K:
          fourth_row_state[8] = 2;
          break;
        case SDL_SCANCODE_L:
          fourth_row_state[9] = 2;
          break;
        case SDL_SCANCODE_SEMICOLON:
          fourth_row_state[10] = 2;
          break;
        case SDL_SCANCODE_APOSTROPHE:
          fourth_row_state[11] = 2;
          break;
        case SDL_SCANCODE_RETURN:
          fourth_row_state[12] = 2;
          break;
        case SDL_SCANCODE_KP_4:
          fourth_row_state[13] = 2;
          break;
        case SDL_SCANCODE_KP_5:
          fourth_row_state[14] = 2;
          break;
        case SDL_SCANCODE_KP_6:
          fourth_row_state[15] = 2;
          break;

        case SDL_SCANCODE_LSHIFT:
          fifth_row_state[0] = 2;
          break;
        case SDL_SCANCODE_Z:
          fifth_row_state[1] = 2;
          break;
        case SDL_SCANCODE_X:
          fifth_row_state[2] = 2;
          break;
        case SDL_SCANCODE_C:
          fifth_row_state[3] = 2;
          break;
        case SDL_SCANCODE_V:
          fifth_row_state[4] = 2;
          break;
        case SDL_SCANCODE_B:
          fifth_row_state[5] = 2;
          break;
        case SDL_SCANCODE_N:
          fifth_row_state[6] = 2;
          break;
        case SDL_SCANCODE_M:
          fifth_row_state[7] = 2;
          break;
        case SDL_SCANCODE_COMMA:
          fifth_row_state[8] = 2;
          break;
        case SDL_SCANCODE_PERIOD:
          fifth_row_state[9] = 2;
          break;
        case SDL_SCANCODE_SLASH:
          fifth_row_state[10] = 2;
          break;
        case SDL_SCANCODE_RSHIFT:
          fifth_row_state[11] = 2;
          break;
        case SDL_SCANCODE_UP:
          fifth_row_state[12] = 2;
          break;
        case SDL_SCANCODE_KP_1:
          fifth_row_state[13] = 2;
          break;
        case SDL_SCANCODE_KP_2:
          fifth_row_state[14] = 2;
          break;
        case SDL_SCANCODE_KP_3:
          fifth_row_state[15] = 2;
          break;
        case SDL_SCANCODE_KP_ENTER:
          fifth_row_state[16] = 2;
          break;

        case SDL_SCANCODE_LCTRL:
          sixth_row_state[0] = 2;
          break;
        case SDL_SCANCODE_LGUI:
          sixth_row_state[1] = 2;
          break;
        case SDL_SCANCODE_LALT:
          sixth_row_state[2] = 2;
          break;
        case SDL_SCANCODE_SPACE:
          sixth_row_state[3] = 2;
          break;
        case SDL_SCANCODE_RALT:
          sixth_row_state[4] = 2;
          break;
        case SDL_SCANCODE_RGUI:
          sixth_row_state[5] = 2;
          break;
        case SDL_SCANCODE_APPLICATION:
          sixth_row_state[6] = 2;
          break;
        case SDL_SCANCODE_RCTRL:
          sixth_row_state[7] = 2;
          break;
        case SDL_SCANCODE_LEFT:
          sixth_row_state[8] = 2;
          break;
        case SDL_SCANCODE_DOWN:
          sixth_row_state[9] = 2;
          break;
        case SDL_SCANCODE_RIGHT:
          sixth_row_state[10] = 2;
          break;
        case SDL_SCANCODE_KP_0:
          sixth_row_state[11] = 2;
          break;
        case SDL_SCANCODE_KP_PERIOD:
          sixth_row_state[12] = 2;
          break;

        }
        break;
      }
    }
    for (int x = 0; x < 16; x++) {
      switch (first_row_state[x]) {
      case 0:
        SDL_RenderCopy(ren, first_row_non_presed[x], NULL, &first_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
        SDL_RenderDrawRect(ren, &first_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        break;
      case 1:
        SDL_RenderCopy(ren, first_row_pressed[x], NULL, &first_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
        SDL_RenderDrawRect(ren, &first_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        break;
      case 2:
        SDL_RenderCopy(ren, first_row_was_pressed[x], NULL, &first_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
        SDL_RenderDrawRect(ren, &first_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        break;
      }
    }
    for (int x = 0; x < 21; x++) {
      switch (second_row_state[x]) {
      case 0:
        SDL_RenderCopy(ren, second_row_non_pressed[x], NULL,
                       &second_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
        SDL_RenderDrawRect(ren, &second_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        break;
      case 1:
        SDL_RenderCopy(ren, second_row_pressed[x], NULL, &second_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
        SDL_RenderDrawRect(ren, &second_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        break;
      case 2:
        SDL_RenderCopy(ren, second_row_was_pressed[x], NULL,
                       &second_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
        SDL_RenderDrawRect(ren, &second_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        break;
      }
    }
    for (int x = 0; x < 21; x++) {
      switch (third_row_state[x]) {
      case 0:
        SDL_RenderCopy(ren, third_row_non_pressed[x], NULL, &third_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
        SDL_RenderDrawRect(ren, &third_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        break;
      case 1:
        SDL_RenderCopy(ren, third_row_pressed[x], NULL, &third_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
        SDL_RenderDrawRect(ren, &third_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        break;
      case 2:
        SDL_RenderCopy(ren, third_row_was_pressed[x], NULL, &third_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
        SDL_RenderDrawRect(ren, &third_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        break;
      }
    }
    for (int x = 0; x < 16; x++) {
      switch (fourth_row_state[x]) {
      case 0:
        SDL_RenderCopy(ren, fourth_row_non_pressed[x], NULL,
                       &fourth_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
        SDL_RenderDrawRect(ren, &fourth_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        break;
      case 1:
        SDL_RenderCopy(ren, fourth_row_pressed[x], NULL, &fourth_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
        SDL_RenderDrawRect(ren, &fourth_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        break;
      case 2:
        SDL_RenderCopy(ren, fourth_row_was_pressed[x], NULL,
                       &fourth_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
        SDL_RenderDrawRect(ren, &fourth_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        break;
      }
    }
    for (int x = 0; x < 17; x++) {
      switch (fifth_row_state[x]) {
      case 0:
        SDL_RenderCopy(ren, fifth_row_non_pressed[x], NULL, &fifth_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
        SDL_RenderDrawRect(ren, &fifth_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        break;
      case 1:
        SDL_RenderCopy(ren, fifth_row_pressed[x], NULL, &fifth_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
        SDL_RenderDrawRect(ren, &fifth_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        break;
      case 2:
        SDL_RenderCopy(ren, fifth_row_was_pressed[x], NULL, &fifth_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
        SDL_RenderDrawRect(ren, &fifth_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        break;
      }
    }
    for (int x = 0; x < 13; x++) {
      switch (sixth_row_state[x]) {
      case 0:
        SDL_RenderCopy(ren, sixth_row_non_pressed[x], NULL, &sixth_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
        SDL_RenderDrawRect(ren, &sixth_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        break;
      case 1:
        SDL_RenderCopy(ren, sixth_row_pressed[x], NULL, &sixth_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
        SDL_RenderDrawRect(ren, &sixth_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        break;
      case 2:
        SDL_RenderCopy(ren, sixth_row_was_pressed[x], NULL, &sixth_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
        SDL_RenderDrawRect(ren, &sixth_row_rect[x]);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        break;
      }
    }

    SDL_Surface* lastPressed = TTF_RenderText_Solid(Sans, last_pressed_key, COLOR_YELLOW);
    SDL_Texture* lastPressedTexture = SDL_CreateTextureFromSurface(ren,lastPressed);
    SDL_Rect lastPressedLocation;
    lastPressedLocation.x = 0;
    lastPressedLocation.y = (BOX_SIZE*6);
    lastPressedLocation.w = 200;
    lastPressedLocation.h = 100;
    SDL_RenderCopy(ren, lastPressedTexture, NULL, &lastPressedLocation);
    SDL_FreeSurface(lastPressed);
    SDL_DestroyTexture(lastPressedTexture);
    SDL_RenderPresent(ren);
    SDL_Delay(10);
    SDL_RenderClear(ren);
  }

  // Closing
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  TTF_Quit();
  SDL_Quit();

  return EXIT_SUCCESS;
}
