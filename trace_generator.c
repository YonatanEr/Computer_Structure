#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define NUM_OF_COMMANDS 5
#define NUM_OF_BYTES 5
#define NUM_OF_OPCODES 22
#define NUM_OF_REGISTERS 16
#define MAX_SIZE_OF_ASM_LINES_ARRAY 100

//Input is an instruction in ML such as "0178B"

char* registers[NUM_OF_REGISTERS] = { "$zero", "$imm", "$vo", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$s0", "$s1", "$s2", "$gp", "$sp", "$ra" };
char* opcodes[NUM_OF_OPCODES] = { "add", "sub", "mul","and" , "or" ,"xor", "sll", "sra", "srl", "beq", "bne", "blt", "bgt", "ble", "bge", "jal", "lw", "sw", "reti", "in", "out", "halt" }; //halt = 0x15000

// int pc = 0, inst = 0, $zero = 0, $imm = 0, $vo = 0, $a0 = 0, $a1 = 0, $a2 = 0, $a3 = 0, $t0 = 0, $t1 = 0, $t2 = 0, $s0 = 0, $s1 = 0, $s2 = 0, $gp = 0, $sp = 0, $ra = 0;
int trace_line[NUM_OF_REGISTERS + 2] = {0}; // {pc, instruction, -all 16 registers sorted-}
int next_pc = 0;

void slice(const char* str, char* result, size_t start, size_t end) { //A function that slices a string, make sure end > start.
	strncpy(result, str + start, end - start);
}

//example of how to call this function: opcode_operation(&next_pc, trace_line[0], 0, &(trace_line + 7), trace_line[8] , trace_line[12]), this was disection of 0078B
void opcode_operation(int* next_pc, int current_pc, int opcode, int* rd, int rs, int rt) {
	if (rs == 1 || rt == 1) //if $imm is present/ I-format
		*next_pc = current_pc + 2;
	else //R-format
		*next_pc = current_pc + 1;

	switch (opcode) {
	case 0: //add
		*rd = rs + rt;
		break;

	case 1: //sub
		*rd = rs - rt;
		break;

	case 2: //mul
		*rd = rs * rt;
		break;

	case 3: //and
		*rd = rs & rt;
		break;

	case 4: //or
		*rd = rs | rt;
		break;

	case 5: //xor
		*rd = rs ^ rt;
		break;

	case 6: //sll
		*rd = rs << rt;
		break;
		/*
	case 7: //sra - NOTE, THERE IS AN EQUATION FOR THIS, CANT FIND IT.
		result = reg1 >> reg2;
		break;
		*/
	case 8: //srl
		*rd = rs >> rt;
		break;

	case 9: //beq
		if (rs == rt)
			*next_pc = rd;
		break;

	case 10: //bne
		if (rs != rt)
			*next_pc = rd;
		break;

	case 11: //blt
		if (rs < rt)
			*next_pc = rd;
		break;

	case 12: //bgt
		if (rs > rt)
			*next_pc = rd;
		break;

	case 13: //ble
		if (rs <= rt)
			*next_pc = rd;
		break;

	case 14: //bge
		if (rs >= rt)
			*next_pc = rd;
		break;

		/*
	case 15: //jal - Didnt understand the requirement.
		if (rs <= rt)
			*next_pc = rd;
		break;
		*/

	case 16: //lw
		break;

	case 17: //sw
		break;

	case 18: //reti
		break;

	case 19: //in
		break;

	case 20: //out
		break;
	}
}

void update_trace_file(int* trace_line) {
	FILE* fptr;
	fptr = fopen("trace.txt", "w");
	if (fptr == NULL) {
		printf("ERROR, COULDN'T OPEN THE FILE");
		return;
	}
	for (int i = 0; i < NUM_OF_REGISTERS + 2; i++) {
		//not finished
	}

}
int main() {
	FILE* fptr;
	fptr = fopen("memin.txt", "r");
	if (fptr == NULL) {
		printf("ERROR, COULDN'T OPEN THE FILE");
		return;
	}
	char instruction_string[NUM_OF_BYTES];
	fscanf(fptr, "%s", &instruction_string);
	
	return 0;
}

