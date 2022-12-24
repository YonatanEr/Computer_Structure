#include "header.h"
//Input is an instruction in ML such as "0078B" which means in assembly "add $t0, $t1, $s1, 0"

// int pc = 0, inst = 0, $zero = 0, $imm = 0, $vo = 0, $a0 = 0, $a1 = 0, $a2 = 0, $a3 = 0, $t0 = 0, $t1 = 0, $t2 = 0, $s0 = 0, $s1 = 0, $s2 = 0, $gp = 0, $sp = 0, $ra = 0;
int trace_line[TRACE_OFFSET + NUM_OF_REGISTERS] = {0}; // {pc, instruction, -all 16 registers sorted-}
int next_pc = 0;
int cycles = 0;

void opcode_operation(int, instruction);
void update_trace_file(int*, FILE*);
void trace_file_generator(FILE*, FILE*);
void regout_file_generator(FILE*);

//example of how to call this function: opcode_operation(&next_pc, trace_line[0], 0, &(trace_line + 7), trace_line[8] , trace_line[12]), this was disection of 0078B


int main() {
	FILE* fptr_memin = fopen("memin.txt", "r");
	FILE* fptr_trace = fopen("trace.txt", "w");
	FILE* fptr_regout = fopen("regout.txt", "w");

	if (fptr_memin == NULL) {
		printf("Error, couldn't open memin.txt\n");
		exit(0);
	}
	if (fptr_trace == NULL) {
		printf("Error, couldn't open trace.txt\n");
		exit(0);
	}
	if (fptr_regout == NULL) {
		printf("Error, couldn't open regout.txt\n");
		exit(0);
	}
	for (int i = 0; i < 3 ; i++) { //upper bound will be the max size of lines recieved from the assembler.
		trace_file_generator(fptr_memin, fptr_trace);
	}
	regout_file_generator(fptr_regout);

	fclose(fptr_memin);
	fclose(fptr_trace);
	fclose(fptr_regout);
	return 0;
}

void opcode_operation(int current_pc, instruction inst) {

	int rd = TRACE_OFFSET + inst.rd; //Initilizes rd as the "address" to the desired register.
	int rs = trace_line[TRACE_OFFSET + inst.rs]; //Initilizes rs as the value inside the register.
	int rt = trace_line[TRACE_OFFSET + inst.rt]; //Initilizes rt as the value inside the register.
	char lw_placeholder[5]; //Will temporarly hold the string fetched from memin.

	if ((rd == 1 || rd == 0) && (inst.opcode <= 8 || inst.opcode == 15 || inst.opcode == 16 || inst.opcode == 19)) { //no writing to $imm and $zero.
		printf("Error, cannot write directly to registers $zero and $imm!");
		exit(0);
	}

	if (trace_line[3]) //if $imm != 0 // I-format
		next_pc += 2;
	else //R-format
		next_pc += 1;

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
		trace_line[rd] = rs / pow(2, rt);
		break;

	case 8: //srl
		trace_line[rd] = rs >> rt;
		break;

	case 9: //beq
		if (rs == rt)
			next_pc = rd;
		break;

	case 10: //bne
		if (rs != rt)
			next_pc = rd;
		break;

	case 11: //blt
		if (rs < rt)
			next_pc = rd;
		break;

	case 12: //bgt
		if (rs > rt)
			next_pc = rd;
		break;

	case 13: //ble
		if (rs <= rt)
			next_pc = rd;
		break;

	case 14: //bge
		if (rs >= rt)
			next_pc = rd;
		break;

	case 15: //jal
		trace_line[rd] = current_pc + 1;
		next_pc = rs;
		break;

	case 16: //lw
		cycles += 1; //load value from memory, increase 1 cycle.
		read_line_number("memin.txt", rs + rt, lw_placeholder);
		trace_line[rd] = hex_string_to_int(lw_placeholder);
		break;

	case 17: //sw
		cycles += 1; //store value in memory, increase 1 cycle.
		overwrite_line_number("memin.txt", rs + rt, trace_line[rd]);
		break;

	case 18: //reti
		break;

	case 19: //in
		break;

	case 20: //out
		break;

	case 21: //halt
		exit(1);
		break;
	}
}

void update_trace_file(int* trac1e_line, FILE* fptr) {
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
		else if (i == 1) { //Instruction with 5 bytes.
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

void trace_file_generator(FILE* fptr_memin, FILE* fptr_trace) {
	char instruction_string[INSTRUCTION_BYTES + 1];
	fscanf(fptr_memin, "%s", &instruction_string); //load instruction from memory, increase 1 cycle.
	cycles += 1;
	instruction inst = parse_instruction(instruction_string); //convert it to the instruction structure.
	trace_line[1] = (inst.opcode << 12) + (inst.rd << 8) + (inst.rs << 4) + inst.rt; //update the instruction "register".
	if (inst.rs == 1 || inst.rt == 1 || inst.rd == 1) { //load ivalue from memory, increase 1 cycle.
		fscanf(fptr_memin, "%X", &trace_line[3]);
		cycles += 1;
	}
	else
		trace_line[3] = 0;
	printf("instruction_string = %s, inst.rd = %d, .rs = %d, .rt = %d, $imm = %d\n", instruction_string, inst.rd, inst.rs, inst.rt, trace_line[3]);
	update_trace_file(trace_line, fptr_trace); //print out trace_line to trace.txt
	opcode_operation(trace_line[0], inst); //do the instruction.
	trace_line[0] = next_pc; //update to the next PC.
}

void regout_file_generator(FILE* fptr) {
	for (int i = TRACE_OFFSET; i < TRACE_OFFSET + NUM_OF_REGISTERS; i++) {
		if (trace_line[i] == 0) {
			fprintf(fptr, "%s", "00000000\n"); //last register need to print thus going down a line.
		}
		else {
			for (int j = 0x10000000; j > trace_line[i]; j = j >> 4)
				fprintf(fptr, "%d", 0);
			fprintf(fptr, "%X\n", trace_line[i]); //last register need to print thus going down a line.
		}
 }
}