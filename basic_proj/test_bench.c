#include <stdio.h>
#include <stdlib.h>

int main() {

	int a = -4;
	int left = a << 1;
	int right = a >> 1;

	printf("a = 0x%X, left  = 0x%X, right = 0x%X", a, left, right);


	return 0;
}