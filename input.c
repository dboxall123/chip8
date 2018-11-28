#include "common.h"

uint8_t scancodes[NUMKEYS] = {
	0x1e, 0x1f, 0x20, 0x21,    // 1, 2, 3, 4
	0x14, 0x1a, 0x08, 0x15,    // q, w, e, r 
	0x04, 0x16, 0x07, 0x09,    // a, s, d, f 
	0x1d, 0x1b, 0x06, 0x19     // z, x, c, v
};

static void keydown(SDL_KeyboardEvent *);
static void keyup(SDL_KeyboardEvent *);

void event_loop(void) {
	SDL_Event event;
	
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				app.running = false;
				break;
			case SDL_KEYDOWN:
				keydown(&event.key);
				break;
			case SDL_KEYUP:
				keyup(&event.key);
				break;
		}// end switch(event.type)
	}
}

static void keydown(SDL_KeyboardEvent *event) {
	for (int i=0; i<NUMKEYS; i++) {
		if (event->keysym.scancode == scancodes[i]) {
			//printf("%x\n", scancodes[i]);
			keys[i] = 1;
		}
	}
}

static void keyup(SDL_KeyboardEvent *event) {
	for (int i=0; i<NUMKEYS; i++) {
		if (event->keysym.scancode == scancodes[i]) {
			//printf("%x\n", scancodes[i]);
			keys[i] = 0;
		}
	}
}

uint8_t wait_for_input(void) {
	;
}

void delay(void) {
	if (delay_timer > 0) {
		--delay_timer; 
	}
	if (sound_timer > 0) {
		if (sound_timer == 1) {
			printf("\a");
		}
		--sound_timer;
	}
	SDL_Delay(1000/60);
}
