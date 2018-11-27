#include "common.h"

static void read_file(FILE *);
static void init_cpu(void);
static void init_SDL(char *);
static void read_font(void);

uint8_t fontset[80] = { 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void init(char *filename) {
	FILE *f;
	if (!(f = fopen(filename, "rb"))) {
		printf("file not found!\n");
		exit(-1);
	} else {
		init_cpu();
		read_file(f);
		read_font();
		init_SDL(filename);
	}
}

static void init_SDL(char *filename) {
	// initialise SDL window and renderer 
	int rflags, wflags;
	rflags = SDL_RENDERER_ACCELERATED;
	wflags = 0;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("can't init SDL: %s\n", SDL_GetError());
		exit(2);
	}
	app.window = SDL_CreateWindow(filename, SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, 
								  SCREEN_HEIGHT, wflags);
	if (!app.window) {
		printf("cannot open window: %s\n", SDL_GetError());
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
	
	app.renderer = SDL_CreateRenderer(app.window, -1, rflags);
	
	if (!app.renderer) {
		printf("failed to create renderer %s\n", SDL_GetError());
	}
}

static void read_font(void) {
	// read fontset into memory from 0 to 80 (0x50)
	for (int i=0; i<80; i++) {
		memory[i] = fontset[i];
	}
}

static void read_file(FILE *f) {
	// read program into memory 
	size_t fsize;
	fseek(f, 0, SEEK_END);
	fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	
	// read into memory at address 0x200 
	fread(memory+0x200, fsize, 1, f);
	fclose(f);
}

static void init_cpu(void) {
	// initialise and reset all variables in cpu.h 
	pc = 0x200;
	sp = -1;
	I_reg = 0;
	delay_timer = 0;
	sound_timer = 0;
	memset(stack, 0, sizeof(stack));
	memset(reg, 0, sizeof(reg));
	memset(memory, 0, sizeof(memory));
	memset(gfx, 0, sizeof(gfx));
	draw_flag = 0;
}

void cleanup(void) {
	// SDL exit functions
	SDL_DestroyRenderer(app.renderer);
	SDL_DestroyWindow(app.window);
	SDL_Quit();
}