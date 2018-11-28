#include "common.h"

void emulate_cycle(void) {
	// 4 digit instruction in hex 
	uint16_t instr = (memory[pc++] << 8) | memory[pc++];
	// rightmost hex digit
	uint8_t hb = (instr >> 12) & 0xf;
	// second digit from the left
	uint8_t x = (instr >> 8) & 0xf;
	// third hex digit from the left
	uint8_t y = (instr >> 4) & 0xf;
	// lowest hex digit (rightmost digit)
	uint8_t n = instr & 0xf;
	// two rightmost hex digits
	uint8_t kk = instr & 0xff;
	// 12 bit memory address, 3 rightmost hex digits
	uint16_t addr = instr & 0xfff;
	
	switch (hb) {
		case 0x0: {
			if (kk == 0xe0) {
				// clear screen
				memset(gfx, 0, sizeof(gfx));
				draw_flag = 1;
			} else if (kk == 0xee) {
				// return from subroutine 
				pc = stack[sp--];
			} else {
				printf("warning... invalid instruction %.4x\n", instr);
			}
			break;
		}
		case 0x1: {
			// unconditional jump
			pc = addr;
			break;
		}
		case 0x2: {
			// call subroutine
			stack[++sp] = pc;
			pc = addr;
			break;
		}
	    case 0x3: {
			// skip next instruction if Vx == kk
			if (reg[x] == kk) {
				pc += 2;
			}
			break;
		} 
		case 0x4: {
			// skip next instr if Vx != kk 
			if (reg[x] != kk) {
				pc += 2;
			}
			break;
		}
		case 0x5: {
			// skip next instr if Vx == Vy
			if (reg[x] == reg[y]) {
				pc += 2;
			}
			break;
		}
		case 0x6: {
			// mov kk into Vx 
			reg[x] = kk;
			break;
		}
		case 0x7: {
			// add Vx to kk, store result in Vx 
			reg[x] += kk;
			break;
		}
		case 0x8: {
			switch (n) {
				case 0x0: {
					// mov Vy into Vx 
					reg[x] = reg[y];
					break;
				}
				case 0x1: {
					// set Vx to Vx | Vy
					reg[x] |= reg[y];
					break;
				}
				case 0x2: {
					// set Vx to Vx & Vy 
					reg[x] &= reg[y];
					break;
				}
				case 0x3: {
					// set Vx to Vx ^ Vy
					reg[x] ^= reg[y];
					break;
				}
				case 0x4: {
					// add Vx to Vy, only store first 8 bits. If
					// result is more than 8 bits, carry flag is set
					int result = reg[x] + reg[y];
					if (result > 255) {
						reg[VF] = 1;
					} else {
						reg[VF] = 0;
					}
					reg[x] = result & 0xff;
					break;
				}
				case 0x5: {
					// if Vx > Vy set carry flag, then subract Vy from Vx 
					if (reg[x] > reg[y]) {
						reg[VF] = 1;
					} else {
						reg[VF] = 0;
					}
					reg[x] -= reg[y];
					break;
				}
				case 0x6: {
					// set VF to lowest bit in Vx, then divide by 2
					reg[VF] = reg[x] & 0x1;
					reg[x] >>= 1;
					break;
				}
				case 0x7: {
					// subtract Vx from Vy 
					if (reg[y] > reg[x]) {
						reg[VF] = 1;
					} else {
						reg[VF] = 0;
					}
					reg[x] = reg[y] -  reg[x];
					break;
				}
				case 0xe: {
					reg[VF] = (reg[x] >> 7) & 0x1;
					reg[x] <<= 1;
					break;
				}
				default: {
					printf("Warning, unimplemented instruction %.4x", instr);
					break;
				}
			}// end switch (n)
			break;
		}
		case 0x9: {
			// skip next instr if Vx != Vy 
			if (reg[x] != reg[y]) {
				pc += 2;
			}
			break;
		}
		case 0xa: {
			// store address in I
			I_reg = addr;
			break;
		}
		case 0xb: {
			// jump to addr + num stored in V0
			pc = addr + reg[V0];
			break;
		}
		case 0xc: {
			// generate random number, then And with kk
			srand(time(0));
			uint8_t rnd = rand() % 256;
			reg[x] = rnd & kk;
			printf("%x\n", reg[x]);
			break;
		}
		case 0xd: {
			// fill up gfx array, which will be used to draw the relevant pixels to the screen
			draw_flag = 1;
			fill_gfx(n, reg[x], reg[y], I_reg);
			break;
		}
		case 0xe: {
			// skip instruction if key pressed 
			if (kk == 0x9e) {
				if (keys[reg[x]]) {
					pc += 2;
				}
			// skip instruction if key not pressed 
			} else if (kk == 0xa1) {
				if (keys[reg[x]] == 0) {
					pc += 2;
				}
			}
			break;
		}
		case 0xf: {
			switch (kk) {
				case 0x07: {
					// store  delay timer
					reg[x] = delay_timer;
					break;
				}
				case 0x0a: {
					// wait for key press 
					printf("waiting for input...\n");
					uint8_t key = wait_for_input();
					reg[x] = key;
					break;
				}
				case 0x15: {
					// set delay timer
					delay_timer = reg[x];
					break;
				}
				case 0x18: {
					// set sound timer 
					sound_timer = reg[x];
					break;
				}
				case 0x1e: {
					// set i to i + Vx 
					I_reg += reg[x];
					break;
				}
				case 0x29: {
					// load location of hex sprite into I. Hex sprits
					// are 4 x 5 pixels, therefore to access, for exameple,
					// number 2, it must be multiplied by 5 
					I_reg = reg[x] * 5;
					break;
				}
				case 0x33: {
					// store BCD representation of Vx in I 
					memory[I_reg]      = reg[x] / 100;
					memory[I_reg + 1]  = (reg[x] / 10) % 10;
					memory[I_reg + 2]  = (reg[x] % 100) % 10;
					break;
				}
				case 0x55: {
					// store V0 through Vx in memory 
					for (int i=0; i<=x; i++) {
						memory[I_reg + i] = reg[i];
					}
					break;
				}
				case 0x65: {
					for (int i=0; i<=x; i++) {
						reg[i] = memory[I_reg + i];
					}
					break;
				}
			}// end of switch (kk)
			break;
		}
		default: {
			printf("Unimplemented instruction %.4x\n", instr);
			break;
		}
	}// end main switch 
}
