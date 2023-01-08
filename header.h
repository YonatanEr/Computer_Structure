#define _CRT_SECURE_NO_WARNINGS

///External Libraries///
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

////Parameters///
#define NUM_OF_COMMANDS 4
#define INSTRUCTION_BYTES 5
#define NUM_OF_OPCODES 22
#define NUM_OF_REGISTERS 16
#define TRACE_OFFSET 2
#define MEM_MAX_SIZE 4096
#define MAX_LINE_SIZE 301
#define IO_SIZE 23
#define MEMORY_LINE_SIZE 20
#define HARD_DISK_SIZE 128
#define READ_WRITE_HARD_DISK_TIME 16384

///Utilities///
#include "utils/hex_string&char_to_int.c"

////Data Types///
#include "structs/instruction.c"
#include "structs/label_element.c"