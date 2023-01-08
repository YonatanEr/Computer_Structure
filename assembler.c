
//Input examples: {"add","$t0","$t1","$s1","0"} R-format or {"add","$t0","$imm","$s1","16"} I-foramt
//Output should be opcode[19:12],rd[11:8],rs[7:4],rt[3:0] (8,4,4,4) in hex -> output example for input = 0078B

#include "header.h"
char* registers[NUM_OF_REGISTERS] = { "$zero", "$imm", "$v0", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$s0", "$s1", "$s2", "$gp", "$sp", "$ra" };
char* opcodes[NUM_OF_OPCODES] = { "add", "sub", "mul","and" , "or" ,"xor", "sll", "sra", "srl", "beq", "bne", "blt", "bgt", "ble", "bge", "jal", "lw", "sw", "reti", "in", "out", "halt" }; //halt = 0x15000
enum format { i_format, r_format, label, word };


void parse_asm_line(char asm_line[], char* parsed_asm[]);
int* asm_line_to_ml(char**, int*);
void update_memin_file(char**, FILE* , int*);
void replace_label(label_element*, char**);
label_element* init_labels(char*);
int format_checker(char*);

int main(int argc, char* argv[]) {

	char* parsed_asm[INSTRUCTION_BYTES];
	char asm_line[MAX_LINE_SIZE];

	label_element* head_of_label_list = init_labels(argv[1]); //Iterate over the .asm file the first time and "indexsize" the labels.

	FILE* fptr_asm = fopen(argv[1], "r");
	if (fptr_asm == NULL) {
		printf("Error, couldn't open fib.asm\n");
		exit(1);
	}

	FILE* fptr_memin = fopen(argv[2], "w");
	if (fptr_memin == NULL) {
		printf("Error, couldn't open memin.txt\n");
		exit(1);
	}
	int memin_size = 0; //in order to know how many 00000 to add in the end to reach 4096.
	int words_data[MEM_MAX_SIZE] = { 0 }; //will hold the data for that address index if there was a ".word data address".

	for (; !feof(fptr_asm); memin_size++) { //  ****************WARNING************ - WILL FAIL IF THE .ASM FILE HAS EMPTY LINES.
		fgets(asm_line, MAX_LINE_SIZE, fptr_asm); //load line from asm file.
		//printf("memin_size = %d, asm line = %s\n",memin_size,asm_line);
		if (format_checker(asm_line) == label){ //if the line is only a label we will move on to the next line.
			memin_size--;
			continue;
		}
		parse_asm_line(asm_line, parsed_asm); //parse it correctly and store it in parsed_asm.
		if (format_checker(asm_line) == word) {//if it finds a .word command it will store the correlated data into its address index.
			words_data[hex_or_dec_string_to_int(parsed_asm[1])] = hex_or_dec_string_to_int(parsed_asm[2]);
			memin_size--;
			continue;
		}
		if (head_of_label_list != NULL) //for the cases there are no labels at all.
			replace_label(head_of_label_list, parsed_asm); //if there are labels in the asm_line, replace them with the correct pc.
		update_memin_file(parsed_asm, fptr_memin, &memin_size); //convert to ml and update memin.
	}
	for (; memin_size < MEM_MAX_SIZE; memin_size++) {//appeneds 00000 line till the end of the file.
		if (words_data[memin_size]) { //if its not 0, append the data from .word.
			for (int i = 0x10000; i > words_data[memin_size]; i = i >> 4)
				fprintf(fptr_memin, "%d", 0);
			if (memin_size == MEM_MAX_SIZE - 1)
				fprintf(fptr_memin, "%X", words_data[memin_size]);
			else
				fprintf(fptr_memin, "%X\n", words_data[memin_size]);
		}
		else if (memin_size == MEM_MAX_SIZE - 1) //if were at the end of the file.
			fprintf(fptr_memin, "00000");
		else
			fprintf(fptr_memin, "00000\n");
	}

	fclose(fptr_memin);
	fclose(fptr_asm);

	free_label_list(head_of_label_list);

	return 0;
}

int* asm_line_to_ml(char* asm_line[INSTRUCTION_BYTES], int* $imm_present) {
	
	int ml_line = 0x0; //output[0]
	int imm_value = 0x0; //output[1]
	int* result = (int*) malloc (2*sizeof(int)); //output
	assert(result);
	*$imm_present = 0; //default is 0 (= false).
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
				if (register_index == 1) 
					*$imm_present = 1; //found $imm so need to update to take it into account.
				ml_line += register_index;
				if ((command_index != INSTRUCTION_BYTES - 2)) //Got to the last register.
					ml_line = ml_line << 4;
				break;
			}
		}
	}
	imm_value = hex_or_dec_string_to_int(asm_line[INSTRUCTION_BYTES - 1]); //converts decimal strings to integers.

	//printf("inside the function we got %X and %X \n", ml_line, imm_value);
	result[0] = ml_line;
	result[1] = imm_value;
	return result;
}

void update_memin_file(char** parsed_asm, FILE* fptr, int* memin_size) {
	int $imm_present = 0;
	int* ml_representation = asm_line_to_ml(parsed_asm, &$imm_present);

	for (int i = 0x10000; i > ml_representation[0]; i = i >> 4)  //Biggest number is 0x15FFF
		fprintf(fptr, "%d", 0); //Will print the missing 0 in the hex representation
	fprintf(fptr, "%X\n", ml_representation[0]);

	if (ml_representation[1] < 0) { //Only if were in I-Format - sign extention.
		ml_representation[1] = twos_compliment_inversion(abs(ml_representation[1])); //twos compliment takes a positive number and spits out its negative one in 20b.
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
	else if ($imm_present) {
		fprintf(fptr, "%s", "00000\n");
		(*memin_size)++;
	}
	free(ml_representation);
	ml_representation = NULL;
}

void parse_asm_line(char asm_line[], char* parsed_asm[]) { //parses the asm line according to spaces, tabs, commas, comments and end of lines.
	for (int i = 0; i < INSTRUCTION_BYTES; i++) {
		if (!i)
			parsed_asm[i] = strtok(asm_line, " \t,#\n");
		else
			parsed_asm[i] = strtok(NULL, " \t,#\n");
	}
}

void replace_label(label_element* head_of_list, char** parsed_asm) { //if the label exists in the parsed line it replaces it with a string decimal int.
	char* x = parsed_asm[4];
	int pc_label = get_pc_label(head_of_list, x);
	if (pc_label != -1)
		sprintf(parsed_asm[4], "%d", pc_label);
}

int format_checker(char* line) { //input is a line from the .asm file and output will be enums (ints) of {i_format = 0, r_format = 1, label = 2, word = 3}
	if (strlen(line) > 5) {
		if (line[0] == '.' && line[1] == 'w' && line[2] == 'o' && line[3] == 'r' && line[4] == 'd')
			return word;
	}
	
	for (int i = 0; i < strlen(line); i++) {
		if (line[i] == ':')
			return label;
		if (line[i] == '#')
			return r_format; //found a comment before finding "word" or mentions of "$imm".
		if (i < strlen(line) - 3) {
			if ((line[i] == '$') && (line[i + 1] == 'i') && (line[i + 2] == 'm') && (line[i + 3] == 'm'))
				return i_format;
		}
	}
	return r_format; //default is r_format since there might no comments.
}


label_element* init_labels(char* path) { 
	//Given a path to an assembly file, itterates over it and counts the "pc lines" in order
	//to correspond them to the labels.
	bool first_label = true;
	int pc_line_counter = 0;
	char line[MAX_LINE_SIZE + 1];
	line[MAX_LINE_SIZE] = '\0';
	label_element* head_of_label_list = NULL;

	FILE* f = fopen(path, "r");
	while (fgets(line, MAX_LINE_SIZE, f) != NULL) {
		int format = format_checker(line);
		if (format == label) {
			if (first_label) {
				first_label = false;
				head_of_label_list = new_label(line, pc_line_counter);
			}
			else {
				if (get_pc_label(head_of_label_list, line) == -1) {
					label_element* cur_label = new_label(line, pc_line_counter);
					append_to_label_list(head_of_label_list, cur_label);
				}
			}
		}

		else if (format == i_format) {
			pc_line_counter += 2;
		}
		else if (format == r_format) {
			pc_line_counter += 1;
		}
	}
	fclose(f);
	return head_of_label_list;
}