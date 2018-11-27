// ram 
uint8_t memory[4096];
// stack pointer and stack 
int8_t sp;
uint16_t stack[16];
// program counter
uint16_t pc;
// registers V0 to VF 
uint8_t reg[16];
// I register 
uint16_t I_reg;
// screen pixel representation
uint8_t gfx[64 * 32];
// timers
uint8_t delay_timer;
uint8_t sound_timer;
// keyboard
uint8_t keys[16]; //1234 qwer asdf zxcv

enum REGISTERS {
	V0, V1, V2, V3, V4, V5, V6, V7,
	V8, V9, VA, VB, VC, VD, VE, VF
};