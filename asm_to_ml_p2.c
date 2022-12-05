
//Input examples: {"add","$t0","$t1","$s1","0"} R-format or {"add","$t0","$imm","$s1","16"} I-foramt
//Output should be opcode[19:12],rd[11:8],rs[7:4],rt[3:0] (8,4,4,4) in hex -> output example for input = 0078B
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define NUM_OF_COMMANDS 5
#define NUM_OF_OPCODES 22
#define NUM_OF_REGISTERS 16

char* registers[NUM_OF_REGISTERS] = { "$zero", "$imm", "$vo", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$s0", "$s1", "$s2", "$gp", "$sp", "$ra" };
char* opcodes[NUM_OF_OPCODES] = { "add", "sub", "mul","and" , "or" ,"xor", "sll", "sra", "srl", "beq", "bne", "blt", "bgt", "ble", "bge", "jal", "lw", "sw", "reti", "in", "out", "halt" }; //halt = 0x15000

int* asm_line_to_ml(char* asm_line[NUM_OF_COMMANDS]) {

	int ml_line = 0x0; //output[0]
	int imm_value = 0x0; //output[1]
	int result[2] = { 0 }; //output
	printf("input is %s, %s, %s, %s, %s \n", asm_line[0], asm_line[1], asm_line[2], asm_line[3], asm_line[4]);
	//printf("%s \n", opcodes[2]);

	for (int opcode_index = 0; opcode_index < NUM_OF_OPCODES; opcode_index++) { //Takes care of the 8 MSB of the ml.
		//printf("compare %s with %s \n", asm_line[0], opcodes[opcode_index]); 
		if (asm_line[0] == opcodes[opcode_index]) {
			ml_line += opcode_index;
			ml_line = ml_line << 4;
			break;
		}
	}
	
	for (int command_index = 1; command_index < NUM_OF_COMMANDS - 1; command_index++) { //Loops over the 3 registers.
		for (int register_index = 0; register_index < NUM_OF_REGISTERS; register_index++) {
			//printf("compare %s with %s \n", asm_line[command_index], registers[register_index]);
			if (asm_line[command_index] == registers[register_index]) {
				ml_line += register_index;
				if ((command_index != NUM_OF_COMMANDS - 2)) //Got to the last register.
					ml_line = ml_line << 4;
				break;
			}
		}
	}
	if (asm_line[NUM_OF_COMMANDS - 1] != "0")
		imm_value = atoi(asm_line[NUM_OF_COMMANDS - 1]); //atio converts numeric strings to integers.


	//printf("inside the function we got %X and %X \n", ml_line, imm_value);
	result[0] = ml_line;
	result[1] = imm_value;
	return result;
}

void update_memin_file(char** asm_lines_array[3]) {
	FILE* fptr;
	fptr = fopen("memin.txt", "w");
	if (fptr == NULL) {
		printf("ERROR, COULDN'T OPEN THE FILE");
		return;
	}

	for (int index = 0; index < 3; index++) {

		int* ml_representation = asm_line_to_ml(asm_lines_array[index]);

		for (int i = 0x10000; i > ml_representation[0]; i = i >> 4)  //Biggest number is 0x15FFF
			fprintf(fptr, "%d", 0); //Will print the missing 0 in the hex representation
		fprintf(fptr, "%X \n", ml_representation[0]);

		if (ml_representation[1] != 0x0) { //Only if were in I-Format.
			for (int i = 0x10000; i > ml_representation[1]; i = i >> 4)
				fprintf(fptr, "%d", 0);
			fprintf(fptr, "%X \n", ml_representation[1]);
		}
	}
	fclose(fptr);
	
}


int main() {
	char* r_input1[NUM_OF_COMMANDS] = { "sub", "$t0", "$t1", "$s1", "0" };
	char* i_input[NUM_OF_COMMANDS] = { "sub","$t0","$imm","$s1","16" };
	char* r_input2[NUM_OF_COMMANDS] = { "ble", "$t0", "$t1", "$imm", "2538" };
	char** array_of_asm_inputs[3] = { r_input1, i_input, r_input2 };

	update_memin_file(array_of_asm_inputs); 

	return 0;
}