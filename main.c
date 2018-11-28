#include "common.h"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: chip8-emu <filename>\n");
		exit(-1);
	}
	init(*++argv);
	atexit(cleanup);
	app.running = true;
	
	prepare_scr();
	present_scr();
	double endMs, startMs;
	int frame_counter;
	startMs = SDL_GetTicks();
	while (app.running) {
		draw_flag = false;
		event_loop();
		emulate_cycle();
		if (draw_flag) {
			prepare_scr();
			//draw_sprite(5, 0, 0, 0x5);
			present_scr();
		}
		delay();
		counter++;
		endMs = SDL_GetTicks();
		if (endMs - startMs >= 1000) {
			printf("%d\n", frame_counter);
			frame_counter = 0;
			startMs = SDL_GetTicks();
		}
		//SDL_Delay(1000/60); 
	}
	return 0;
}
