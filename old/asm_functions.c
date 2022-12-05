
//input example {add,$t0,$t1,$s1}.
//output should be opcode[19:12],rd[11:8],rs[7:4],rt[3:0] (8,4,4,4) in hex -> output example for input 0x0078B

#define NUM_OF_COMMANDS 4
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

enum registers {$zero, $imm, $vo, $a0, $a1, $a2, $a3, $t0, $t1, $t2, $s0, $s1, $s2, $gp, $sp, $ra} reg;
enum opcodes {add, sub, mul, halt} opcode;

int asm_to_ml(int asm_commands[NUM_OF_COMMANDS]) { //converts the asm lines into machine language.

	int full_ml = 0x0;

	for (int i = 0; i < NUM_OF_COMMANDS; i++) {
		full_ml += asm_commands[i];
		printf("number is %d and in hex is %X and the current asm_command is %d \n", full_ml, full_ml, asm_commands[i]);
		if (i != NUM_OF_COMMANDS - 1)
			full_ml = full_ml << 4; 
	}
	return full_ml;
}

int update_memin_file(int file_opened, int ml_hex_representation) {
	FILE* fptr; 

	if (!file_opened)
		fptr = fopen("memin.txt", "w");
	else
		fptr = fopen("memin.txt", "a");

	if (fptr == NULL)
		printf("ERROR, DIDNT FIND THE FILE OR COULDNT OPEN");

	else {

		for (int i = 0x10000; i > ml_hex_representation; i = i >> 4) { //Biggest number is 0x15FFF
			//printf("compare %X with %X \n", i, output); //TEST
			fprintf(fptr, "%d", 0); //Will print the missing 0 in the hex representation
		}
		fprintf(fptr, "%X \n", ml_hex_representation);
		fclose(fptr);
	}
	return 1;
}

int main() {

	int input1[NUM_OF_COMMANDS] = {add, $vo, $a1, $t0}; //bin = 0000_0000_0010_0100_0111, hex = 0x00247
	int input2[NUM_OF_COMMANDS] = {sub, $vo, $a1, $t0}; //bin = 0000_0001_0010_0100_0111, hex = 0x01247
	//int stop_sim[NUM_OF_COMMANDS] = {halt, $zero, $zero, $zero};
	//char sim_input[3] = {input1, input2, stop_sim};
	//for (int i = 0; i < NUM_OF_COMMANDS; i++)
		//printf("itteration %d and the num of command is %d \n", i, input1[i]);
	int ml_hex_representation, file_opened = 0;
	//printf(sim_input[0]);
	//printf(sim_input[0]);
	//for (int i = 0; sim_input[i] < 0x03000; i++) { //this will be the end of the memin file, need to think exactly how it will recive it.
	ml_hex_representation = asm_to_ml(input1); //Convert the asm to int ml.
	//printf("hex representation is %X", ml_hex_representation);
	file_opened = update_memin_file(file_opened, ml_hex_representation); //insert the ml into a line in memin
	ml_hex_representation = asm_to_ml(input2);
	file_opened = update_memin_file(file_opened, ml_hex_representation);
	
}