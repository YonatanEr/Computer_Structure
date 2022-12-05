#include <string.h>

void slice(const char* str, char* result, int start, int end) { //A function that slices a string, make sure end > start.
	strncpy(result, str + start, end - start);
}