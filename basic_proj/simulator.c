#include "header.h"
//Input is an instruction in ML such as "0078B" which means in assembly "add $t0, $t1, $s1, 0"

// int pc = 0, inst = 0, $zero = 0, $imm = 0, $vo = 0, $a0 = 0, $a1 = 0, $a2 = 0, $a3 = 0, $t0 = 0, $t1 = 0, $t2 = 0, $s0 = 0, $s1 = 0, $s2 = 0, $gp = 0, $sp = 0, $ra = 0;
int trace_line[TRACE_OFFSET + NUM_OF_REGISTERS] = {0}; // {pc, instruction, -all 16 registers sorted-}
int io_line[IO_SIZE]; //input/output registers.
enum io_register { irq0enable, irq1enabl, irq2enable, irq0status, irq1status, irq2status, irqhandler, irqreturn, clks, leds, display7reg, timerenable, timercurrent, timermax, diskcmd, disksector, diskbuffer, diskstatus, reserved, reserved, monitoraddr, monitordata, monitorcmd };
char ram[MEM_MAX_SIZE][INSTRUCTION_BYTES+1];
char hard_disk[HARD_DISK_SIZE][INSTRUCTION_BYTES + 1];
int next_pc = 0;
int cycles = 0;

//for debugging.
char* registers[NUM_OF_REGISTERS] = { "$zero", "$imm", "$vo", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$s0", "$s1", "$s2", "$gp", "$sp", "$ra" };
char* opcodes[NUM_OF_OPCODES] = { "add", "sub", "mul","and" , "or" ,"xor", "sll", "sra", "srl", "beq", "bne", "blt", "bgt", "ble", "bge", "jal", "lw", "sw", "reti", "in", "out", "halt" }; //halt = 0x15000
char* io_registers[IO_SIZE] = { "irq0enable","irq1enable","irq2enable","irq0status","irq1status","irq2status","irqhandler","irqreturn","clks","leds","display7reg","timerenable","timercurrent","timermax","diskcmd","disksector","diskbuffer","diskstatus","reserved","reserved","monitoraddr","monitordata","monitorcmd" };
//end

void opcode_operation(instruction, int*, int);
void update_trace_file(char*);
void regout_file_generator(char*);
void download_memin_to_ram(char*);
void upload_ram_to_memout(char*);
void simulator(char*);

int main(int argc, char* argv[]) { //argv[1] = memin.txt, argv[2] = memout.txt, argv[3] = regout.txt, argv[4] = trace.txt, argv[5] = cycles.txt.
	
	download_memin_to_ram(argv[1]); //DOWNLOAD MEMIN TO AN INTERNAL ARRAY.
	simulator(argv[4]); //ACTIVATE THE SIMULATOR AND GENERATE TRACE.TXT

////****THE SIMULATOR IS DONE, PREPARE ALL THE OUTPUT FILES, TRACE.TXT IS GENERATED ALREADY BY THE SIMULATOR****////

	upload_ram_to_memout(argv[2]); 
	regout_file_generator(argv[3]);

	FILE* fptr_cycles = fopen(argv[5], "w");
	if (fptr_cycles == NULL) {
		printf("Error, couldn't open %s\n", argv[5]);
		exit(1);
	}
	fprintf(fptr_cycles, "%d", cycles);
	fclose(fptr_cycles);
	
	return 0;
}


void download_memin_to_ram(char* memin_path) { //downloads the entirety of memin to a char** ram.
	FILE* fptr_memin = fopen(memin_path, "r"); //holds the initial instructions and data that were translated by the assembler.
	if (fptr_memin == NULL) {
		printf("Error, couldn't open %s\n", memin_path);
		exit(1);
	}
	for (int i = 0; i < MEM_MAX_SIZE; i++) {
		fscanf(fptr_memin, "%s", &ram[i]);
	}
	fclose(fptr_memin);
}

void download_diskin_to_ram(char* diskin_path) { //downloads the entirety of diskin to a char** hard_disk.
	FILE* fptr_diskin = fopen(diskin_path, "r"); //holds the initial hard disk contents given by the user.
	if (fptr_diskin == NULL) {
		printf("Error, couldn't open %s\n", diskin_path);
		exit(1);
	}
	for (int i = 0; i < HARD_DISK_SIZE; i++) {
		fscanf(fptr_diskin, "%s", &hard_disk[i]);
	}
	fclose(fptr_diskin);
}


void upload_ram_to_memout(char* memout_path) {
	FILE* fptr_memout = fopen(memout_path, "w");
	if (fptr_memout == NULL) {
		printf("Error, couldn't open %s\n", memout_path);
		exit(1);
	}
	for (int i = 0; i < MEM_MAX_SIZE; i++) {
		if (i == MEM_MAX_SIZE-1)
			fprintf(fptr_memout, "%s", ram[i]); //last line
		else
			fprintf(fptr_memout, "%s\n", ram[i]);
	}
	fclose(fptr_memout);
}

void opcode_operation(instruction inst, int* halt, int $imm) {

	int rd = TRACE_OFFSET + inst.rd; //Initilizes rd as the "address" to the desired register.
	int rs = trace_line[TRACE_OFFSET + inst.rs]; //Initilizes rs as the value inside the register.
	int rt = trace_line[TRACE_OFFSET + inst.rt]; //Initilizes rt as the value inside the register.

	if ((rd == 2 || rd == 3) && (inst.opcode <= 8 || inst.opcode == 15 || inst.opcode == 16 || inst.opcode == 19)) { //no writing to $imm and $zero.
		printf("Error, cannot write directly to registers $zero and $imm!");
		exit(1);
	}

	if ($imm) //if $imm != 0 // I-format
		next_pc += 2;
	else //R-format
		next_pc += 1;

	switch (inst.opcode) {
	case 0: //add
		trace_line[rd] = rs + rt;
		break;

	case 1: //sub
		trace_line[rd] = rs - rt;
		break;

	case 2: //mul
		trace_line[rd] = rs * rt;
		break;

	case 3: //and
		trace_line[rd] = rs & rt;
		break;

	case 4: //or
		trace_line[rd] = rs | rt;
		break;

	case 5: //xor
		trace_line[rd] = rs ^ rt;
		break;

	case 6: //sll
		trace_line[rd] = rs << rt;
		break;

	case 7: //sra
		trace_line[rd] = rs >> rt; //C's shift operator already works on signed 32bit numbers and duplicates the msb.
		break;

	case 8: //srl
		trace_line[rd] = (rs >> rt) & 0x7fffffff; //the msb is replaced by a "0" always duo to the masking.
		break;

	case 9: //beq
		if (rs == rt)
			next_pc = trace_line[rd];
		break;

	case 10: //bne
		if (rs != rt)
			next_pc = trace_line[rd];
		break;

	case 11: //blt
		if (rs < rt)
			next_pc = trace_line[rd];
		break;

	case 12: //bgt
		if (rs > rt)
			next_pc = trace_line[rd];
		break;

	case 13: //ble
		if (rs <= rt)
			next_pc = trace_line[rd];
		break;

	case 14: //bge
		if (rs >= rt)
			next_pc = trace_line[rd];
		break;

	case 15: //jal
		trace_line[rd] = trace_line[0] + $imm + 1; //go to the line after the ivalue (if it exists)
		next_pc = rs;
		break;

	case 16: //lw
		cycles++; //load value from memory, increase 1 cycle.
		trace_line[rd] = hex_string_to_int_signed(ram[rs + rt]); 
		break;

	case 17: //sw
		cycles++; //store value in memory, increase 1 cycle.
		sprintf(ram[rs + rt], "%05X", trace_line[rd]);
		break;

	case 18: //reti
		break;

	case 19: //in
		break;

	case 20: //out
		break;

	case 21: //halt
		*halt = 1;
		break;
	}
}

void update_trace_file(char* trace_path) {
	FILE* fptr = fopen(trace_path, "a");
	if (fptr == NULL) {
		printf("Error, couldn't open %s\n", trace_path);
		exit(1);
	}
	for (int i = 0; i < NUM_OF_REGISTERS + 2; i++) { //prints out into the trace.txt file the pc, inst and 16 registers.
		if (i == 0) { //PC with 3 bytes.
			if (trace_line[i] == 0)
				fprintf(fptr, "%s", "000 ");
			else {
				for (int j = 0x100; j > trace_line[i]; j = j >> 4)  //Biggest number is 
					fprintf(fptr, "%d", 0); //Will print the missing 0 in the hex representation
				fprintf(fptr, "%X ", trace_line[i]); //Space is added here and not \t or \n.
			}
		}
		else if (i == 1) { //Instruction with 5 bytes.
			if (trace_line[i] == 0)
				fprintf(fptr, "%s", "00000 ");
			else {
				for (int j = 0x10000; j > trace_line[i]; j = j >> 4)
					fprintf(fptr, "%d", 0);
				fprintf(fptr, "%X ", trace_line[i]);
			}
		}
		else { //All the registers with 8 bytes.
			if (trace_line[i] == 0) {
				if (i != NUM_OF_REGISTERS + 1)
					fprintf(fptr, "00000000 ");
				else {
					if (trace_line[1] < 0x15000) //last trace line? if so dont go down a line.
						fprintf(fptr, "%s", "00000000\n"); //last register need to print thus going down a line.
					else
						fprintf(fptr, "%s", "00000000"); //last register need to print thus going down a line.
				}
			}
			else if (trace_line[i] < 0) { //sign extension is guranteed for negatives.
				if (i != NUM_OF_REGISTERS + 1)
					fprintf(fptr, "%X ", trace_line[i]);
				else {
					if (trace_line[1] < 0x15000)
						fprintf(fptr, "%X\n", trace_line[i]); //last register need to print thus going down a line.
					else
						fprintf(fptr, "%X\n", trace_line[i]); //last register need to print thus going down a line.
				}
			}
			else {
				for (int j = 0x10000000; j > trace_line[i]; j = j >> 4)
					fprintf(fptr, "%d", 0);
				if (i != NUM_OF_REGISTERS + 1)
					fprintf(fptr, "%X ", trace_line[i]);
				else {
					if (trace_line[1] < 0x15000)
						fprintf(fptr, "%X\n", trace_line[i]); //last register need to print thus going down a line.
					else
						fprintf(fptr, "%X", trace_line[i]); //last register need to print thus going down a line.

				}
			}
		}
	}
	fclose(fptr);
	return;
}

void simulator(char* trace_path) {
	FILE* fptr_trace = fopen(trace_path, "w");
	if (fptr_trace == NULL) {
		printf("Error, couldn't open %s\n", trace_path);
		exit(1);
	}
	fclose(fptr_trace); //just create the file, no need to write yet.

	for (int $imm = 0, halt = 0; !halt; trace_line[0] = next_pc, $imm = 0) { //while halt was not asserted, update to the next PC and and execute its command.
		instruction inst = parse_instruction(ram[trace_line[0]]); //convert it to the instruction structure.
		cycles++; //load instruction from memory, increase 1 cycle.
		trace_line[1] = (inst.opcode << 12) + (inst.rd << 8) + (inst.rs << 4) + inst.rt; //update the instruction "register".
		if (inst.rs == 1 || inst.rt == 1 || inst.rd == 1) { //$imm is present in the command, load ivalue from memory, increase 1 cycle.
			trace_line[3] = hex_string_to_int_signed(ram[trace_line[0] + 1]); //load I-value to $imm, C compiler will preform sign extension auto.
			cycles++;
			$imm++; //set $imm to be 1 (=true) since it is present.
		}
		else
			trace_line[3] = 0;
		printf("\n\n instruction = %X \t PC = %X \t %s, %s, %s, %s %d\n\n rd = %d\t rs = %d\t rt = %d\n", trace_line[1], trace_line[0], opcodes[inst.opcode], registers[inst.rd], registers[inst.rs], registers[inst.rt], trace_line[3], trace_line[2 + inst.rd], trace_line[2 + inst.rs], trace_line[2 + inst.rt]); //TEST
		update_trace_file(trace_path); //print out trace_line to trace.txt
		opcode_operation(inst, &halt, $imm); //do the instruction.
	}
}

void regout_file_generator(char* regout_path) {
	FILE* fptr = fopen(regout_path, "w");
	if (fptr == NULL) {
		printf("Error, couldn't open %s\n", regout_path);
		exit(1);
	}
	for (int i = TRACE_OFFSET; i < TRACE_OFFSET + NUM_OF_REGISTERS; i++) {
		if (trace_line[i] == 0) {
			if (i== TRACE_OFFSET + NUM_OF_REGISTERS-1)
				fprintf(fptr, "%s", "00000000"); //last register.
			else
				fprintf(fptr, "%s", "00000000\n"); 
		}
		else if (trace_line[i] < 0) { //sign extension to 32b is guaranteed by the C compiler.
			if (i == TRACE_OFFSET + NUM_OF_REGISTERS - 1)
				fprintf(fptr, "%X", trace_line[i]);
			else
				fprintf(fptr, "%X\n",trace_line[i]);
		}
		else {
			for (int j = 0x10000000; j > trace_line[i]; j = j >> 4)
				fprintf(fptr, "%d", 0);
			if (i == TRACE_OFFSET + NUM_OF_REGISTERS - 1)
				fprintf(fptr, "%X", trace_line[i]); //last register.
			else
				fprintf(fptr, "%X\n", trace_line[i]); 
		}
	}
	fclose(fptr);
}


void timer_handler() {
	if (io_registers[timerenable]) {
		io_registers[irq0enable] = 1;
		//IN PROGRESS. add to the "OUT" opcode operation that if irq0enable is set high we need to enable irq0status for a cycle.
		if (io_registers[timercurrent] == io_registers[timermax]) {
			io_registers[irq0status] = 0;
			//irqstatus = 1;  which irqstatus do i turn on??? IN PROGRESS
		}
		else {
			io_registers[timercurrent]++;
		}
	}
}

void led_handler(int input) {
	io_registers[leds] = input;
}