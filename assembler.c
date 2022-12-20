#include "header.h"

int main(/*recives the .asm and a few files as arguments*/) {

	int lines_count = 0;

	char*** asm_lines = asm_to_ml_p1(&lines_count); // Updates "lines_count" directly and return an output like so {{ "sub","$t0","$imm","$s1","16" }, { "sub", "$t1", "$t0", "$s1", "0" }, ...}
	asm_to_ml_p2(asm_lines, lines_count); //Uses lines_count as an upper bound so it would know when to print "00000" lines until line number 4096, updates memin.txt.

	return 0;
}