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
	if (input[0] == '0' && (input[1] == 'x' || input[1] =='X')) //if the string is like 0x1A5, could also be 1A5 or 001A5 for example.
		i += 2;
	if ((hex_char_to_int(input[i]) >= 8) && (strlen(input)-i == 5)) { //is only calculated if the msb is set to HIGH and confirms there are 5 nibbles.
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


//char* convert_to_standard_imm_format(char* ivalue) { //takes a hex/dec string and converts it to standard dec string.
	//if (ivalue[0] == '0' && (ivalue[1] == 'x' || ivalue[1] == 'X')) {
	//	int int_ivalue = hex_string_to_int_signed(ivalue);
	//	return dec_int_to_string(int_ivalue); 
	//}
	//return ivalue;
//}


int twos_compliment_inversion(int input) { //recieves a positive number an negates it.
	int mask = 0xFFF00000; //Size of int is 4bytes, need to mask only the 20 lsb.
	int result = (~(mask | input) + 1); //Invert all the bits and add one to the result
	return result;
}


int hex_or_dec_string_to_int(char* ivalue) { //takes a hex/dec string and converts it to int.

	if (ivalue[0] == '0' && (ivalue[1] == 'x' || ivalue[1] == 'X'))
		return hex_string_to_int_signed(ivalue);
	return dec_string_to_int(ivalue);
}
