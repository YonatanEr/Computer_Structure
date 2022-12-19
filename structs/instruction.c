#include <stdio.h>

typedef struct instruction {
	int  opcode;
	int  rd;
	int  rs;
	int  rt;
} instruction;

instruction parse_instruction(char* memin_line) { //input example is "10B32".
	instruction new_inst;

	new_inst.opcode = (memin_line[0] == 49) * 16 + hex_char_to_int(memin_line[1]); //Biggest hex is 0x15. 49 in ASCII is '1'.
	new_inst.rd = hex_char_to_int(memin_line[2]);
	new_inst.rs = hex_char_to_int(memin_line[3]);
	new_inst.rt = hex_char_to_int(memin_line[4]);

	return new_inst;
}


int main() {

	int lines_count = 0;

	char*** asm_lines = asm_to_ml_p1(&lines_count); // will update directly 
	asm_to_ml_p2(asm_lines, lines_count);
	
	return 0;
}