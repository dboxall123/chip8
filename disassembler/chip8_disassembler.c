#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

FILE *f;
uint8_t memory[4096];
uint8_t sp = 0;
uint16_t stack[16];
uint16_t pc = 0x200;

typedef struct {
	uint8_t sprite [5];
} Sprite;

void disassemble(uint16_t instr) {
	printf("%.2x %.2x   ", instr >> 8 & 0xff, instr & 0xff);
	switch ((instr >> 12) & 0xf) {
		case 0x0: {
			uint8_t lower = instr & 0xff;
			if (lower == 0xe0) {
				printf("CLS\n");
			} else if (lower == 0xee) {
				printf("RET\n");
			}
			break;
		}
		case 0x1: {
			printf("JMP %x\n", instr & 0xfff);
			break;
		}
		case 0x2: {
			printf("CALL %x\n", instr & 0xfff);
			break;
		}
		case 0x3: {
			printf("SE V%x, %x\n",(instr >> 8) & 0xf, instr & 0xff);
			break;
		}
		case 0x4: {
			printf("SNE V%x, %x\n", (instr >> 8) & 0xf, instr & 0xff);
			break;
		}	
		case 0x5: {
			printf("SE V%x, V%x\n", (instr >> 8) & 0xf, (instr >> 4) & 0xf);
			break;
		}
		case 0x6: {
			printf("LD V%x, %x\n", (instr >> 8) & 0xf, instr & 0xff);
			break;
		}
		case 0x7: {
			printf("ADD V%x, %x\n", (instr >> 8) & 0xf, instr & 0xff);
			break;
		}
		case 0x8: {
			uint8_t lsn = instr & 0xf;
			uint8_t x = (instr >> 8) & 0xf;
			uint8_t y = (instr >> 4) & 0xf;
			switch (lsn) {
				case 0x0: {
					printf("LD V%x, V%x\n", x, y);
					break;
				}
				case 0x1: {
					printf("OR V%x, V%x\n", x, y);
					break;
				}
				case 0x2: {
					printf("AND V%x, V%x\n", x, y);
					break;
				}
				case 0x3: {
					printf("XOR V%x, V%x\n", x, y);
					break;
				}
				case 0x4: {
					printf("ADD V%x, V%x\n", x, y);
					break;
				}
				case 0x5: {
					printf("SUB V%x, V%x\n", x, y);
					break;
				}
				case 0x6: {
					printf("SHR V%x, V%x\n", x, y);
					break;
				}
				case 0x7: {
					printf("SUBN V%x, V%x\n", x, y);
					break;
				}
				case 0xe: {
					printf("SHL V%x, V%x\n", x, y);
					break;
				}
			}// end lsn switch 
			break;
		}// end case 0x8 
		case 0x9: {
			printf("SNE V%x, V%x\n", (instr >> 8) & 0xf, (instr >> 4) & 0xf);
			break;	
		}
		case 0xa: {
			printf("LD I, %x\n", instr & 0xfff);
			break;
		}
		case 0xb: {
			printf("JP V0, %x\n", instr & 0xfff);
			break;
		}
		case 0xc: {
			printf("RND V%x, %x\n", (instr >> 8) & 0xf, instr & 0xff);
			break;
		}
		case 0xd: {
			printf("DRW V%x, V%x, %x\n", (instr >> 8) & 0xf, (instr >> 4) & 0xf, instr & 0xf);
			break;
		}
		case 0xe: {
			if ((instr & 0xff) == 0x9e) {
				printf("SKP V%x\n", instr >> 8 & 0xf);
			} else if ((instr & 0xff) == 0xa1) {
				printf("SKNP V%x\n", instr >> 8 & 0xf);
			}
			break;
		}
		case 0xf: {
			uint8_t x = (instr >> 8) & 0xf;
			switch (instr & 0xff) {
				case 0x07: {
					printf("LD V%x, DT\n", x);
					break;
				}
				case 0xa: {
					printf("LD V%x, K\n", x);
					break;
				}
				case 0x15: {
					printf("LD DT, V%x\n", x);
					break;
				}
				case 0x18: {
					printf("LD ST, V%x\n", x);
					break;
				}
				case 0x1e: {
					printf("ADD I, V%x\n", x);
					break;
				}
				case 0x29: {
					printf("LD F, V%x\n", x);
					break;
				}
				case 0x33: {
					printf("LD B, V%x\n", x);
					break;
				}
				case 0x55: {
					printf("LD [I], V%x\n", x);
					break;
				}
				case 0x65: {
					printf("LD V%x, [I]\n", x);
					break;
				}
			}// end 0xf switch
		}// end of case 0xf 
	}// end switch
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: chip8disassemble <filename>\n");
		return 1;
	}
	if (!(f = fopen(*++argv, "rb"))) {
		printf("file not found\n");
		return -1;
	}
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	fseek(f, 0, SEEK_SET);
	fread(memory+0x200, size, 1, f);
	fclose(f);
	
	uint16_t instr;
	while (pc < (size + 0x200)) {
		printf("%.4x   ", pc);
		instr = (memory[pc++] << 8) | memory[pc++];
		disassemble(instr);
	}
	
	return 0;
}
