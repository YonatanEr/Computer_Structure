#include "header.h"
//Input is an instruction in ML such as "0178B"

// int pc = 0, inst = 0, $zero = 0, $imm = 0, $vo = 0, $a0 = 0, $a1 = 0, $a2 = 0, $a3 = 0, $t0 = 0, $t1 = 0, $t2 = 0, $s0 = 0, $s1 = 0, $s2 = 0, $gp = 0, $sp = 0, $ra = 0;
int trace_line[NUM_OF_REGISTERS + 2] = {0}; // {pc, instruction, -all 16 registers sorted-}
int next_pc = 0;

//example of how to call this function: opcode_operation(&next_pc, trace_line[0], 0, &(trace_line + 7), trace_line[8] , trace_line[12]), this was disection of 0078B
void opcode_operation(int* next_pc, int current_pc, int opcode, int* rd, int rs, int rt) {
	if (rs == 1 || rt == 1) //if $imm is present/ I-format
		*next_pc = current_pc + 2;
	else //R-format
		*next_pc = current_pc + 1;

	trace_line[1] = opcode;

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
	case 7: //sra - NOTE, THERE IS AN EQUATION FOR THIS, CAN'T FIND IT.
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
		
	case 15: //jal
		*rd = current_pc + 1;
		*next_pc = rs;
		break;

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
	fptr = fopen("trace.txt", "a");
	if (fptr == NULL) {
		printf("Error, couldn't open trace.txt\n");
		return;
	}
	for (int i = 0; i < NUM_OF_REGISTERS + 2; i++) { //prints out into the trace.txt file the pc, inst and 16 registers.
		if (i == 0) { //PC with 3 bytes.
			for (int i = 0x100; i > trace_line[i]; i = i >> 4)  //Biggest number is 
				fprintf(fptr, "%d", 0); //Will print the missing 0 in the hex representation
			fprintf(fptr, "%X ", trace_line[i][0]); //Space is added here and not \t or \n.
		}
		else if (i == 1){ //Instruction with 5 bytes.
			for (int i = 0x10000; i > trace_line[i]; i = i >> 4)  
				fprintf(fptr, "%d", 0); 
			fprintf(fptr, "%X ", trace_line[i]); 
		}
		else { //All the registers with 8 bytes.
			for (int i = 0x10000000; i > trace_line[i]; i = i >> 4)
				fprintf(fptr, "%d", 0);
			if (i != NUM_OF_REGISTERS + 1)
				fprintf(fptr, "%X ", trace_line[i]);
			else
				fprintf(fptr, "%X\n", trace_line[i]); //last register need to print thus going down a line.
		}
	}
}

int main() {
	char instruction_string[NUM_OF_BYTES];

	FILE* fptr1, fptr2;
	fptr1 = fopen("memin.txt", "r");
	fptr2 = fopen("trace.txt", "w");

	if (fptr1 == NULL) {
		printf("Error, couldn't open memin.txt\n");
		return;
	}
	if (fptr2 == NULL) {
		printf("Error, couldn't open trace.txt\n");
		return;
	}
	for (int i = 0; i < 100; i++) {
		fscanf(fptr1, "%s", &instruction_string); //read a line from the memin.txt instructions
		instruction inst = parse_instruction(instruction_string); //convert it to the instruction structure.
		opcode_operation(&next_pc, trace_line[0], inst.opcode, trace_line[TRACE_OFFSET + inst.rd], trace_line[TRACE_OFFSET + inst.rs], trace_line[TRACE_OFFSET + inst.rt]);
		update_trace_file(trace_line);
	}

	return 0;
}

