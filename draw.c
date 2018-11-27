#include "common.h"

void prepare_scr(void) {
	// fill background
	SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 0);
	SDL_RenderClear(app.renderer);
	
	// draw sprites 
	for (int row=0; row<32; row++) {
		for (int col=0; col<64; col++) {
			if (gfx[(row * 64) + col]) {
				Pixel p;
				p.rect.x = col * PIXEL_SIZE;
				p.rect.y = row * PIXEL_SIZE;
				p.rect.w = PIXEL_SIZE;
				p.rect.h = PIXEL_SIZE;
				SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
				SDL_RenderFillRect(app.renderer, &p.rect);
			}
		}
	}
}

void present_scr(void) {
	SDL_RenderPresent(app.renderer);
}

void fill_gfx(uint8_t height, uint8_t x, uint8_t y, uint16_t addr) {
	reg[VF] = 0;
	
	for (int row=0; row<height; row++) {
		// grab next sprite number from memory 
		uint16_t pixel = memory[addr + row];
		for (int col=7; col>=0; col--) {
			// if this bit is set to 1
			if ((pixel & (0x80 >> col))) {
				if (gfx[(x + col + ((y + row) * 64))]) {
					reg[VF] = 1;
				}
				gfx[x + col + ((y + row) * 64)] ^= 1;
			}
		}
	}
}