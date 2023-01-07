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

///Utilities///
#include "utils/hex_string&char_to_int.c"

////Data Types///
#include "structs/instruction.c"
#include "structs/label_element.c"

//Global Variables///
//char* registers[NUM_OF_REGISTERS] = { "$zero", "$imm", "$vo", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$s0", "$s1", "$s2", "$gp", "$sp", "$ra" };
//char* opcodes[NUM_OF_OPCODES] = { "add", "sub", "mul","and" , "or" ,"xor", "sll", "sra", "srl", "beq", "bne", "blt", "bgt", "ble", "bge", "jal", "lw", "sw", "reti", "in", "out", "halt" }

