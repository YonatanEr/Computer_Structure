#include "header.h"

int main() {
	/*
	char* input1 = "-511";
	char* input2 = "523";
	char* input3 = "0x0";
	char* input4 = "0x5A4";
	char* input5 = "0x5a4";
	char* input6 = "0";
	
	int input1 = 0;
	int input2 = 511;
	int input3 = -511;
	int input4 = -8;
	int input5 = 8;
	
	char* inputs[6] = {input1, input2, input3, input4, input5, input6};
	//int inputs[5] = { input1, input2, input3, input4, input5};
	for (int i = 0, convert_req = 0; i < 6; i++) {
		if (inputs[i][0] == '0' && inputs[i][1] == 'x') {
			char* output = convert_to_standard_imm_format(inputs[i]);
			printf("output is %s for the input %s\n", output, inputs[i]);
			free(output);
		}
		else
			printf("input %s is already formatted correctly\n", inputs[i]);
	}
	
	char* input1 = "0xD0001";
	char* input2 = "D0001";
	char* input3 = "80001";
	char* inputs[3] = { input1, input2, input3 };
	for (int i = 0; i < 3; i++) {
		int result = hex_string_to_int_signed(inputs[i]);
		printf("%d\n", result);
	}
	*/

	char* filename = "memin.txt";
	char result[6];
	read_line_number(filename, 3, result);
	printf("%s\n", result);


	return 0;
}