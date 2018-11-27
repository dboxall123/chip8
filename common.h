#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "cpu.h"

#define PIXEL_SIZE 10
#define SCREEN_WIDTH 64 * PIXEL_SIZE
#define SCREEN_HEIGHT 32 * PIXEL_SIZE
#define NUMKEYS 16

// init.c
void init(char *);
void cleanup(void);

//emulate.c
void emulate_cycle(void);

// draw.c
void prepare_scr(void);
void present_scr(void);
void fill_gfx(uint8_t, uint8_t, uint8_t, uint16_t);

// input.c
void event_loop(void);
uint8_t wait_for_input(void);
void delay(void);

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
	bool running;
} App;

App app;

typedef struct {
	int x, y;
	SDL_Rect rect;
} Pixel;

bool draw_flag;
#endif