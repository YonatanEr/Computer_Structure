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
#define NUM_OF_IO_REGISTERS 23
#define MEMORY_LINE_SIZE 20
#define HARD_DISK_SIZE 16384
#define DMA_ACTIVE_DURATION 1024
#define SECTOR_SIZE 128


///Utilities///
#include "utils/c&i_conversions.c"

////Data Types///
#include "structs/instruction.c"
#include "structs/label_element.c"
#include "structs/monitor.c"
#include "structs/linked_list.c"