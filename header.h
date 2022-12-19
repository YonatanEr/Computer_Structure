#define _CRT_SECURE_NO_WARNINGS
////Constants///
#define NUM_OF_COMMANDS 4
#define INSTRUCTION_BYTES 5
#define NUM_OF_OPCODES 22
#define NUM_OF_REGISTERS 16
#define TRACE_OFFSET 2

///External Libraries///
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

///Utilities///
//#include "utils/file_utils.c"
#include "utils/string_utils.c"
//#include "utils/slice_utils_nati.c"
//#include "utils/split_utils.c"
#include "utils/hex_string&char_to_int.c"

////Data Types///
#include "structs/word.c"
#include "structs/int_array.c"
#include "structs/instruction.c"
#include "structs/labels_array.c"


//Static Variables///
//char* registers[NUM_OF_REGISTERS] = { "$zero", "$imm", "$vo", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$s0", "$s1", "$s2", "$gp", "$sp", "$ra" };
//char* opcodes[NUM_OF_OPCODES] = { "add", "sub", "mul","and" , "or" ,"xor", "sll", "sra", "srl", "beq", "bne", "blt", "bgt", "ble", "bge", "jal", "lw", "sw", "reti", "in", "out", "halt" }