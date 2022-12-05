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
	for (int i = 0; i < 5; i++) 
		result = result * 16 + hex_char_to_int(input[i]);
	return result;
}