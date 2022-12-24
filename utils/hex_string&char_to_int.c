#include <math.h>

int hex_char_to_int(char input) { //converts a character to hex
	int result = 0;

	if (input > 64) { //Capital letters only, doesn't count for lower case letters in memin.txt.
		result = 10;
		for (int i = 65; input != i && i < 71; i++)
			result++;
	}
	else
		result = input - '0'; //returns a number in range 0 to 9.
	return result;
}

int hex_string_to_int(char* input) {
	int result = 0;
	for (int i = 0; i < strlen(input); i++) 
		result = result * 16 + hex_char_to_int(input[i]);
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