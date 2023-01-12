#include <stdio.h>
#include <stdlib.h>

int main() {

	int a = -4;
	int left = a << 1;
	int right = a >> 1;
	int arr[6] = { 1,2,3,3,3,3 };

	printf("a = 0x%X, left  = 0x%X, right = 0x%X, len(arr) = %d", a, left, right, sizeof(arr) / 4);


	return 0;
}