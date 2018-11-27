#include "common.h"

void emulate_cycle(void) {
	// fetch opcode
	uint16_t instr = (memory[pc] << 8) | memory[pc+1];
	printf("%x\n", instr);
	// decode opcode
	switch ((instr >> 12) & 0xf) {
		case 0x0: {
			uint8_t lower = instr & 0xff;
			if (lower == 0xe0) {
				// clear screen
				memset(gfx, 0, sizeof(gfx));
				draw_flag = true;
				pc += 2;
			} else if (lower == 0xee) {
				// return 
				pc = stack[sp--];
			}
			break;
		}
		case 0x1: {
			// unconditional jump
			pc += 2;
			uint16_t addr = instr & 0xfff;
			stack[++sp] = pc;
			pc = addr;
		}
		case 0x2: {
			// call subroutine at addr 
			pc += 2;
			uint16_t addr = instr & 0xfff;
			stack[++sp] = pc;
			pc = addr;
		}
		case 0x3: {
			// skip next instruction if VX == kk
			uint8_t x = (instr >> 8) & 0xf;
			uint8_t kk = instr & 0xff;
			if (reg[x] == kk) {
				pc += 4;
			} else {
				pc += 2;
			}
			break;
		}
		case 0x4: {
			// skip next instruction if Vx != kk
			uint8_t x = (instr >> 8) & 0xf;
			uint8_t kk = instr & 0xff;
			if (reg[x] != kk) {
				pc += 4;
			} else {
				pc += 2;
			}
			break;
		}
		case 0x5: {
			// skip next instruction if Vx == Vy
			uint8_t x = (instr >> 8) & 0xf;
			uint8_t y = (instr >> 4) & 0xf;
			if (reg[x] == reg[y]) {
				pc += 4;
			} else {
				pc += 2;
			}
			break;
		}
		case 0x6: {
			// mov kk into Vx
			//uint8_t x = (instr & 0x0f00) >> 8;
			uint8_t x = (instr >> 8) & 0xf;
			uint8_t kk = instr & 0xff;
			reg[x] = kk;
			//printf("%d\n", reg[x]);
			pc += 2;
			break;
		}
		case 0x7: {
			// add kk to Vx (Vx = Vx + kk)
			uint8_t x = (instr >> 8) & 0xf;
			uint8_t kk = instr & 0xff;
			reg[x] += kk;
			pc += 2;
			break;
		}
		case 0x8: {
			uint8_t low = instr & 0xf;
			uint8_t x = (instr >> 8) & 0xf;
			uint8_t y = (instr >> 4) & 0xf;
			switch (low) {
				case 0x0: {
					// mov Vy into Vx 
					reg[x] = reg[y];
					break;
				}
				case 0x1: {
					// Vx |= Vy
					reg[x] |= reg[y];
					break;
				}
				case 0x2: {
					// Vx &= Vy
					reg[x] &= reg[y];
					break;
				}
				case 0x3: {
					// xor Vx, Vy 
					reg[x] = reg[x] ^ reg[y];
					break;
				}
				case 0x4: {
					// add Vy to Vx, store in Vx
					// if result greater than 8 bits (255) then
					// VF carry flag set and only store lower 8 bits
					int res = reg[x] + reg[y];
					if (res > 255) {
						reg[VF] = 1;
					} else {
						reg[VF] = 0;
					}
					reg[x] = res & 0xff;
					break;
				}
				case 0x5: {
					/* if Vx > Vy the VF flag is set to 1,
					 * else 0. Then Vy is subtracted from Vx,
					 * and the result stored in Vx */
					if (reg[x] > reg[y]) {
						reg[VF] = 1;
					} else {
						reg[VF] = 0;
					}
					reg[x] -= reg[y];
					break;
				}
				case 0x6: {
					/* set VF to lsb of Vx, then divide
					 * Vx by two */
					reg[VF] = reg[x] & 1;
					reg[x] >>= 1;
					break;
				}
				case 0x7: {
					// Vx = Vy - Vx, set VF = NOT borrow
					if (reg[y] > reg[x]) {
						reg[VF] = 1;
					} else {
						reg[VF] = 0;
					}
					reg[x] = reg[y] - reg[x];
					break;
				}
				case 0xe: {
					reg[VF] = (x >> 7) & 1;
					reg[x] <<= 1;
					break;
				}
			} // end 0x8 switch 
			pc += 2;
			break;
		}// end case 0x8 
		case 0x9: {
			// if Vx != Vy skip next instruction 
			uint8_t x = (instr >> 8) & 0xf;
			uint8_t y = (instr >> 4) & 0xf;
			if (reg[x] != reg[y]) {
				pc += 4;
			} else {
				pc += 2;
			}
			break;
		}
		case 0xa: {
			// mov addr into I
			uint16_t addr = instr & 0xfff;
			I_reg = addr;
			pc += 2;
			break;
		}
		case 0xb: {
			// jmp to addr + V0
			uint16_t addr = instr & 0xfff;
			stack[++sp] = pc;
			pc = reg[V0] + addr;
		}
		case 0xc: {
			// Vx = random 8 bit number AND kk 
			srand(time(0));
			uint8_t x = (instr >> 8) & 0xf;
			uint8_t kk = instr & 0xff;
			uint8_t rnd = (rand() % 256);
			reg[x] = rnd & kk;
			pc += 2;
			break;
		}
		case 0xd: {
			draw_flag = true;
			uint8_t x = (instr >> 8) & 0xf;
			uint8_t y = (instr >> 4) & 0xf;
			uint8_t n = instr & 0xf;
			printf("draw x:%x y:%x addr:%x depth: %d\n", reg[x], reg[y], I_reg, n);
			draw_sprite(n, reg[x], reg[y], I_reg);
			pc += 2;
			break;
		}
		case 0xe: {
			uint8_t x = (instr >> 8) & 0xf;
			// skip next instruction if key in 
			// Vx is pressed 
			if ((instr & 0xff) == 0x9e) {
				if (keys[reg[x]]) {
					pc += 4;
				} else {
					pc += 2;
				}
			// skip next instruction if key in Vx 
			// in up posiiton
			} else if ((instr & 0xff) == 0xa1) {
				if (!keys[reg[x]]) {
					pc += 4;
				} else {
					pc += 2;
				}
			}
			break;
		}
		case 0xf: {
			uint8_t x = (instr >> 8) & 0xf;
			switch (instr & 0xff) {
				case 0x07: {
					// store delay_timer val in Vx
					reg[x] = delay_timer;
					break;
				}
				case 0x0a: {
					// wait  for key press and store val in Vx
					reg[x] = getchar();
					break;
				}
				case 0x15: {
					// set delay_timer to Vx 
					delay_timer = reg[x];
					break;
				}
				case 0x18: {
					// set sound timer to val of Vx 
					sound_timer = reg[x];
					break;
				}
				case 0x1e: {
					// add I to Vx, store result in I
					I_reg += reg[x];
					break;
				}
				case 0x29: {
					// load location of sprite into I
					I_reg = reg[x] * 5;
					break;
				}
				case 0x33: {
					// store BCD rep of Vx in I, I+1, I+2
					memory[I_reg]     = reg[x] / 100;
					memory[I_reg + 1] = (reg[x] / 10) % 10;
					memory[I_reg + 2] = (reg[x] % 100) % 10;
					break;
				}
				case 0x55: {
					// store registers V0 through Vx in memory,
					// starting at location I 
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
			}
			pc += 2;
			break;
		}// end case 0xf
	}// end main switch 
}