
//Input examples: {"add","$t0","$t1","$s1","0"} R-format or {"add","$t0","$imm","$s1","16"} I-foramt
//Output should be opcode[19:12],rd[11:8],rs[7:4],rt[3:0] (8,4,4,4) in hex -> output example for input = 0078B

#include "header.h"
char* registers[NUM_OF_REGISTERS] = { "$zero", "$imm", "$vo", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$s0", "$s1", "$s2", "$gp", "$sp", "$ra" };
char* opcodes[NUM_OF_OPCODES] = { "add", "sub", "mul","and" , "or" ,"xor", "sll", "sra", "srl", "beq", "bne", "blt", "bgt", "ble", "bge", "jal", "lw", "sw", "reti", "in", "out", "halt" }; //halt = 0x15000

void parse_asm_line(char asm_line[], char* parsed_asm[]);
int* asm_line_to_ml(char**);
void update_memin_file(char**, FILE* , int*);

int main() {

	char* parsed_asm[INSTRUCTION_BYTES];
	char asm_line[MAX_LINE_SIZE];
	FILE* fptr_asm = fopen("fib.asm", "r");
	if (fptr_asm == NULL) {
		printf("Error, couldn't open fib.asm\n");
		exit(0);
	}

	FILE* fptr_memin = fopen("memin.txt", "w");
	if (fptr_memin == NULL) {
		printf("Error, couldn't open memin.txt\n");
		exit(0);
	}
	int memin_size = 0; //in order to know how many 00000 to add in the end to reach 4096.
	int words_data[MEM_MAX_SIZE] = { 0 }; //will hold the data for that address index if there was a ".word data address" .

	for (; !feof(fptr_asm); memin_size++) { //  ****************WARNING************ - WILL FAIL IF THE .ASM FILE HAS EMPTY LINES.
		fgets(asm_line, MAX_LINE_SIZE, fptr_asm); //load line from asm file.
		printf("asm line = %s\n", asm_line);
		parse_asm_line(asm_line, parsed_asm); //parse it correctly and store it in parsed_asm.
		if (!strcmp(parsed_asm[0],".word")) //if it finds a .word command it will store the correlated data into its address index.
			words_data[hex_or_dec_string_to_int(parsed_asm[1])] = hex_or_dec_string_to_int(parsed_asm[2]);
		else 
			update_memin_file(parsed_asm, fptr_memin, &memin_size); //convert to ml and update memin.
	}
	for (; memin_size < MEM_MAX_SIZE; memin_size++) {//appeneds 00000 line till the end of the file.
		if (words_data[memin_size]) { //if its not 0
			for (int i = 0x10000; i > words_data[memin_size]; i = i >> 4)
				fprintf(fptr_memin, "%d", 0);
			if (memin_size == MEM_MAX_SIZE - 1)
				fprintf(fptr_memin, "%X", words_data[memin_size]);
			else
				fprintf(fptr_memin, "%X\n", words_data[memin_size]);
		}
		else if (memin_size == MEM_MAX_SIZE - 1)
			fprintf(fptr_memin, "00000");
		else
			fprintf(fptr_memin, "00000\n");
	}

	fclose(fptr_memin);
	fclose(fptr_asm);
	return 0;
}

int* asm_line_to_ml(char* asm_line[INSTRUCTION_BYTES]) {

	int ml_line = 0x0; //output[0]
	int imm_value = 0x0; //output[1]
	int result[2] = { 0 }; //output
	//printf("input is %s, %s, %s, %s, %s\n", asm_line[0], asm_line[1], asm_line[2], asm_line[3], asm_line[4]);

	for (int opcode_index = 0; opcode_index < NUM_OF_OPCODES; opcode_index++) { //Takes care of the 8 MSB of the ml.
		//printf("compare %s with %s \n", asm_line[0], opcodes[opcode_index]); 
		if (!strcmp(asm_line[0],opcodes[opcode_index])) {
			ml_line += opcode_index;
			ml_line = ml_line << 4;
			break;
		}
	}
	
	for (int command_index = 1; command_index < INSTRUCTION_BYTES - 1; command_index++) { //Loops over the 3 registers.
		for (int register_index = 0; register_index < NUM_OF_REGISTERS; register_index++) {
			//printf("compare %s with %s \n", asm_line[command_index], registers[register_index]);
			if (!strcmp(asm_line[command_index], registers[register_index])) {
				ml_line += register_index;
				if ((command_index != INSTRUCTION_BYTES - 2)) //Got to the last register.
					ml_line = ml_line << 4;
				break;
			}
		}
	}
	if (asm_line[INSTRUCTION_BYTES - 1] != "0")
		imm_value = dec_string_to_int(asm_line[INSTRUCTION_BYTES - 1]); //converts decimal strings to integers, won't use atoi cus this is 32 bits.

	//printf("inside the function we got %X and %X \n", ml_line, imm_value);
	result[0] = ml_line;
	result[1] = imm_value;
	return result;
}

void update_memin_file(char** parsed_asm, FILE* fptr, int* memin_size) {

	int* ml_representation = asm_line_to_ml(parsed_asm);

	for (int i = 0x10000; i > ml_representation[0]; i = i >> 4)  //Biggest number is 0x15FFF
		fprintf(fptr, "%d", 0); //Will print the missing 0 in the hex representation
	fprintf(fptr, "%X\n", ml_representation[0]);

	if (ml_representation[1] < 0) { //Only if were in I-Format - sign extention.
		ml_representation[1] = twos_compliment_inversion(abs(ml_representation[1])); //twos compliment takes a pos number and spits out its negative one in 20b.
		for (int i = 0x10000; i > ml_representation[1]; i = i >> 4)
			fprintf(fptr, "%d", 0);
		fprintf(fptr, "%X\n", ml_representation[1]);
		(*memin_size)++; //by adding ivalue line we are increasing the data size inside of memin.
	}
	else if (ml_representation[1] > 0) {
		for (int i = 0x10000; i > ml_representation[1]; i = i >> 4)
			fprintf(fptr, "%d", 0);
		fprintf(fptr, "%X\n", ml_representation[1]);
		(*memin_size)++; //by adding ivalue line we are increasing the data size inside of memin.
	}
}

void parse_asm_line(char asm_line[], char* parsed_asm[]) {
	for (int i = 0; i < INSTRUCTION_BYTES; i++) {
		if (!i)
			parsed_asm[i] = strtok(asm_line, " \t,#\n");
		else
			parsed_asm[i] = strtok(NULL, " \t,#\n");
	}
}

