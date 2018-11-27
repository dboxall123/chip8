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
		//SDL_Delay(1000/60); 
	}
	return 0;
}
