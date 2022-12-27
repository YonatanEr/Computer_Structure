#include "header.h"

void parse_asm_line(char asm_line[], char* parsed_asm[]) {
	for (int i = 0; i < INSTRUCTION_BYTES; i++) {
		if (!i)
			parsed_asm[i] = strtok(asm_line, " \t,#");
		else
			parsed_asm[i] = strtok(NULL, " \t,#");
	}
}

int main() {
	char asm_line[MAX_LINE_SIZE] = "out $t2, $zero, $imm, 2				# enable irq2";
	char* parsed_asm[INSTRUCTION_BYTES];
	parse_asm_line(asm_line, parsed_asm);
	for (int i = 0; i < INSTRUCTION_BYTES; i++)
		printf("%s\n", parsed_asm[i]); //printing each token
	return 0;
}




/*
#include "header.h"

void parse_asm_line(char*, char**);

int main() {
	char asm_line[MAX_LINE_SIZE] = "out $t2, $zero, $imm, 2				# enable irq2";
	char* parsed_asm[INSTRUCTION_BYTES]; //initilizing as { ,  ,  ,  ,  , '\0'}

	void parse_asm_line(char* asm_line, char** parsed_asm);

	for (int i = 0; i < INSTRUCTION_BYTES; i++)
		printf("%s\n", parsed_asm[i]); //printing each token

	return 0;
}



*/