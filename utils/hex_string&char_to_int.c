#include <math.h>

int hex_char_to_int(char input) { //converts a hex represented character (number/capital letter/small letter) to int.
	int result = 0;

	if (input > 96) { //small letters.
		result = 10;
		for (int i = 97; input != i && i < 103; i++) //looking for my letter.
			result++;
	}
	else if (input > 64) { //capital letters.
		result = 10;
		for (int i = 65; input != i && i < 71; i++) //looking for my letter.
			result++;
	}
	else //decimal numbers.
		result = input - '0'; //returns a number in range 0 to 9.
	return result;
}

int hex_string_to_int_unsigned(char* input) {
	int result = 0, i = 0;
	if (input[0] == '0' && (input[1] == 'x' || input[1] == 'X')) //if the string is like 0x1A5.
		i += 2;
	for (; i < strlen(input); i++) 
		result = result * 16 + hex_char_to_int(input[i]);
	return result;
}

int hex_string_to_int_signed(char* input) {
	int result = 0, i = 0, msnibble = 0;
	if (input[0] == '0' && (input[1] == 'x' || input[1] =='X')) //if the string is like 0x1A5.
		i += 2;
	if (hex_char_to_int(input[i]) >= 8) { //is only calculated if the msb is set to HIGH.
		int bits_16_to_18 = (0x7 & hex_char_to_int(input[i])) << 16; //looking at bits 16-18 of input[19:0], masking the msb of the nibble.
		msnibble = -1 * pow(2, 19) + bits_16_to_18;
		i++;
	}
	for (; i < strlen(input); i++)
		result = result * 16 + hex_char_to_int(input[i]);
	result += msnibble; //msnibble is zero if the number is unsigned.
	return result;
}

int dec_string_to_int(char* input) {
	int result = 0, negative_number = 0;
	for (int i = 0; i < strlen(input); i++) {
		if (input[i] == '-') {
			negative_number = 1;
			continue;
		}
		result = result * 10 + hex_char_to_int(input[i]);
	}
	return (pow(-1, negative_number) * result); //if negative_number = 0 will return pos, and if 1 will return neg.
}

char* dec_int_to_string(int input) {
	int char_array_size = 0;
	if (!input) {
		char* result = (char*)malloc(2);
		assert(result);
		result[0] = '0';
		result[1] = '\0';
		return result;
	}
	if (input < 0)
		char_array_size++; //space for the minus sign.
	int abs_input = abs(input); //checking for amount of digits.
	while (abs_input > 0) { 
		char_array_size++;
		abs_input /= 10; //division is rounded down, counting the number of digits to allocate for chars.
	}
	char* result = (char*)malloc(char_array_size + 1); //allocating memory for the size of the string.
	assert(result);
	result[char_array_size] = '\0';

	int stop = 0; //checker if there is a - sign, relevent for the for loop.
	if (input < 0) {
		result[0] = '-';
		stop++;
	}
	for (int abs_input = abs(input), i = char_array_size - 1; i >= stop; i--) { //going from lowest to highest.
		result[i] = abs_input % 10 + '0';
		abs_input /= 10;
	}
	return result;

}

int twos_compliment_inversion(int input) { //recieves a positive number an negates it.
	int mask = 0xFFF00000; //Size of int is 4bytes, need to mask only the 20 lsb.
	int result = (~(mask | input) + 1); //Invert all the bits and add one to the result
	return result;
}

char* convert_to_standard_imm_format(char* ivalue) { //takes a hex/dec string and converts it to standard dec string.
	if (ivalue[0] == '0' && (ivalue[1] == 'x' || ivalue[1] == 'X')) {
		int int_ivalue = hex_string_to_int_signed(ivalue);
		return dec_int_to_string(int_ivalue); //SHOULD PROB BE REPLACED WITH SPRINTF.
	}
	return ivalue;
}

int hex_or_dec_string_to_int(char* ivalue) { //takes a hex/dec string and converts it to int.

	if (ivalue[0] == '0' && (ivalue[1] == 'x' || ivalue[1] == 'X'))
		return hex_string_to_int_signed(ivalue);
	return dec_string_to_int(ivalue);
}


/*
char* sw_to_ram(int input){ //takes an integer as an input and spits out a string (char*) formatted for ram array.
	int temp = 0;
	char result[5]; // 5 = INSTRUCTION BYTES

	if (input < 0) { //Only if were in I-Format - sign extention.
		temp = twos_compliment_inversion(abs(input)); //twos compliment takes a positive number and spits out its negative one in 20b.
		for (int i = 0x10000, j = 0; i > temp; i = i >> 4, j++)
			result[j] = 'F';
		fprintf(fptr, "%X\n", ml_representation[1]);
	}
	else if (input > 0) {
		for (int i = 0x10000; i > input; i = i >> 4)
			fprintf(fptr, "%d", 0);
		fprintf(fptr, "%X\n", ml_representation[1]);
	}
	else{//input is exactly 0.

	}
}
*/