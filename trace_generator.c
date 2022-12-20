#include "header.h"
//Input is an instruction in ML such as "0078B" which means in assembly "add $t0, $t1, $s1, 0"

// int pc = 0, inst = 0, $zero = 0, $imm = 0, $vo = 0, $a0 = 0, $a1 = 0, $a2 = 0, $a3 = 0, $t0 = 0, $t1 = 0, $t2 = 0, $s0 = 0, $s1 = 0, $s2 = 0, $gp = 0, $sp = 0, $ra = 0;
int trace_line[NUM_OF_REGISTERS + 2] = {0}; // {pc, instruction, -all 16 registers sorted-}
int next_pc = 0;

//example of how to call this function: opcode_operation(&next_pc, trace_line[0], 0, &(trace_line + 7), trace_line[8] , trace_line[12]), this was disection of 0078B
void opcode_operation(int* next_pc, int current_pc, instruction inst) {

	int rd = TRACE_OFFSET + inst.rd; //Initilizes rd as the "address" to the desired register.
	int rs = trace_line[TRACE_OFFSET + inst.rs]; //Initilizes rs as the value inside the register.
	int rt = trace_line[TRACE_OFFSET + inst.rt]; //Initilizes rt as the value inside the register.

	if (trace_line[3]) //if $imm != 0 // I-format
		*next_pc += 2;
	else //R-format
		*next_pc += 1;

	switch (inst.opcode) {
	case 0: //add
		trace_line[rd] = rs + rt;
		break;

	case 1: //sub
		trace_line[rd] = rs - rt;
		break;

	case 2: //mul
		trace_line[rd] = rs * rt;
		break;

	case 3: //and
		trace_line[rd] = rs & rt;
		break;

	case 4: //or
		trace_line[rd] = rs | rt;
		break;

	case 5: //xor
		trace_line[rd] = rs ^ rt;
		break;

	case 6: //sll
		trace_line[rd] = rs << rt;
		break;
		
	case 7: //sra
		trace_line[rd] = rs / pow(2,rt);
		break;
		
	case 8: //srl
		trace_line[rd] = rs >> rt;
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
		trace_line[rd] = current_pc + 1;
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

	case 21: //halt
		exit(0);
		break;
	}
}

void update_trace_file(int* trace_line, FILE* fptr) {
	fptr = fopen("trace.txt", "a");
	if (fptr == NULL) {
		printf("Error, couldn't open trace.txt\n");
		return;
	}
	for (int i = 0; i < NUM_OF_REGISTERS + 2; i++) { //prints out into the trace.txt file the pc, inst and 16 registers.
		if (i == 0) { //PC with 3 bytes.
			if (trace_line[i] == 0)
				fprintf(fptr, "%s", "000 ");
			else {
				for (int j = 0x100; j > trace_line[i]; j = j >> 4)  //Biggest number is 
					fprintf(fptr, "%d", 0); //Will print the missing 0 in the hex representation
				fprintf(fptr, "%X ", trace_line[i]); //Space is added here and not \t or \n.
			}
		}
		else if (i == 1){ //Instruction with 5 bytes.
			if (trace_line[i] == 0)
				fprintf(fptr, "%s", "00000 ");
			else {
				for (int j = 0x10000; j > trace_line[i]; j = j >> 4)
					fprintf(fptr, "%d", 0);
				fprintf(fptr, "%X ", trace_line[i]);
			}
		}
		else { //All the registers with 8 bytes.
			if (trace_line[i] == 0) {
				if (i != NUM_OF_REGISTERS + 1)
					fprintf(fptr, "00000000 ");
				else
					fprintf(fptr, "%s", "00000000\n"); //last register need to print thus going down a line.
			}
			else {
				for (int j = 0x10000000; j > trace_line[i]; j = j >> 4)
					fprintf(fptr, "%d", 0);
				if (i != NUM_OF_REGISTERS + 1)
					fprintf(fptr, "%X ", trace_line[i]);
				else
					fprintf(fptr, "%X\n", trace_line[i]); //last register need to print thus going down a line.
			}
		}
	}
	fclose(fptr);
	return;
}

int main() {
	char instruction_string[INSTRUCTION_BYTES+1];

	FILE* fptr1;
	FILE* fptr2;
	fptr1 = fopen("memin.txt", "r");
	fptr2 = fopen("trace.txt", "w");

	if (fptr1 == NULL) {
		printf("Error, couldn't open memin.txt\n");
		return 0;
	}
	if (fptr2 == NULL) {
		printf("Error, couldn't open trace.txt\n");
		return 0;
	}
	for (int i = 0; i < 3 ; i++) {
		fscanf(fptr1, "%s", &instruction_string); //read a line from the memin.txt instructions
		instruction inst = parse_instruction(instruction_string); //convert it to the instruction structure.
		if (inst.rs == 1 || inst.rt == 1)
			fscanf(fptr1, "%X", &trace_line[3]);
		else
			trace_line[3] = 0;
		//printf("instruction_string = %s, inst.rd = %d, .rs = %d, .rt = %d, $imm = %d\n", instruction_string, inst.rd, inst.rs, inst.rt, trace_line[3]);
		trace_line[0] = next_pc; //update the current pc.
		trace_line[1] = (inst.opcode << 12) + (inst.rd << 8) + (inst.rs << 4) + inst.rt; //update the instruction
		update_trace_file(trace_line, fptr2); //print out trace_line to trace.txt
		opcode_operation(&next_pc, trace_line[0], inst); //do the instruction.
	}

	fclose(fptr1);
	fclose(fptr2);
	return 0;
}