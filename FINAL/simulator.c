#include "header.h"
//Input is an instruction in ML such as "0078B" which means in assembly "add $t0, $t1, $s1, 0"

// int pc = 0, inst = 0, $zero = 0, $imm = 0, $vo = 0, $a0 = 0, $a1 = 0, $a2 = 0, $a3 = 0, $t0 = 0, $t1 = 0, $t2 = 0, $s0 = 0, $s1 = 0, $s2 = 0, $gp = 0, $sp = 0, $ra = 0;
int trace_line[TRACE_OFFSET + NUM_OF_REGISTERS] = {0}; // {pc, instruction, -all 16 registers sorted-}
int io_line[NUM_OF_IO_REGISTERS]; //input/output registers.
enum io_register { irq0enable, irq1enable, irq2enable, irq0status, irq1status, irq2status, irqhandler, irqreturn, clks, leds, display7reg, timerenable, timercurrent, timermax, diskcmd, disksector, diskbuffer, diskstatus, reserved0, reserved1, monitoraddr, monitordata, monitorcmd };
char ram[MEM_MAX_SIZE][INSTRUCTION_BYTES + 1];
char hard_disk[HARD_DISK_SIZE][INSTRUCTION_BYTES + 1];
char* io_registers[NUM_OF_IO_REGISTERS] = { "irq0enable","irq1enable","irq2enable","irq0status","irq1status","irq2status","irqhandler","irqreturn","clks","leds","display7reg","timerenable","timercurrent","timermax","diskcmd","disksector","diskbuffer","diskstatus","reserved","reserved","monitoraddr","monitordata","monitorcmd" };
linked_list* irq2_list = NULL;
monitor* display;
int next_pc = 0;
int cycles = 0; 

//FOR DEBBUGGING.
char* registers[NUM_OF_REGISTERS] = { "$zero", "$imm", "$vo", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$s0", "$s1", "$s2", "$gp", "$sp", "$ra" };
char* opcodes[NUM_OF_OPCODES] = { "add", "sub", "mul","and" , "or" ,"xor", "sll", "sra", "srl", "beq", "bne", "blt", "bgt", "ble", "bge", "jal", "lw", "sw", "reti", "in", "out", "halt" }; //halt = 0x15000
//END

void opcode_operation(instruction, int*, int, char*, char*, char*, int*);
void update_trace_file(char*);
void regout_file_generator(char*);
void download_memin_to_ram(char*);
void upload_ram_to_memout(char*);
void download_diskin_to_hard_disk(char*);
void upload_hard_disk_to_diskout(char*);
void download_irq2(char*);
void simulator(char*,char*, char*, char*);
void update_leds_display_file(char*, int);
void timer_manager();
void hard_disk_manager(int*);
void monitor_manager();
void irq2_manager();
void isr_operation(int*);
void update_hwregtrace_file(char*, int, int);
void io_register_size_validator();
void clk_manager();

int main(int argc, char* argv[]) { 
	
	// gcc simulator.c -o sim

	// ./sim.exe memin.txt diskin.txt irq2in.txt memout.txt regout.txt trace.txt hwregtrace.txt cycles.txt leds.txt display7seg.txt diskout.txt monitor.txt
	
	/*
	0	sim.exe
	1	memin.txt
	2	diskin.txt
	3	irq2in.txt
	4	memout.txt
	5	regout.txt 
	6	trace.txt 
	7	hwregtrace.txt 
	8	cycles.txt 
	9	leds.txt 
	10	display7seg.txt 
	11	diskout.txt 
	12	monitor.txt
	*/

	display = init_monitor();
	download_memin_to_ram(argv[1]); //DOWNLOAD MEMIN TO AN INTERNAL ARRAY.
	download_diskin_to_hard_disk(argv[2]);
	download_irq2(argv[3]);
	simulator(argv[6], argv[7], argv[9], argv[10]); //ACTIVATE THE SIMULATOR AND GENERATE TRACE.TXT and HWREGTRACE.TXT.

////****THE SIMULATOR IS DONE, PREPARE ALL THE OUTPUT FILES, TRACE.TXT IS GENERATED ALREADY BY THE SIMULATOR****////

	free_linked_list(irq2_list);
	upload_ram_to_memout(argv[4]);
	upload_hard_disk_to_diskout(argv[11]);
	regout_file_generator(argv[5]);
	monitor_to_txt(display, argv[12]);
	free_monitor(display);

	FILE* fptr_cycles = fopen(argv[8], "w");
	if (fptr_cycles == NULL) {
		printf("Error, couldn't open %s\n", argv[5]);
		exit(1);
	}
	fprintf(fptr_cycles, "%u", cycles);
	fclose(fptr_cycles);
		
	return 0;
}

void download_irq2(char* irq2_path) { //downloads the contents of the irq2in input file into a linked list for easier use.
	FILE* fptr_irq2 = fopen(irq2_path, "r");
	if (fptr_irq2 == NULL) {
		printf("Error, couldn't open %s\n", irq2_path);
		exit(1);
	}
	linked_list* new_element = NULL;
	int data;
	while(!feof(fptr_irq2)) { //while we haven't reach the end of the file, keep going.
		fscanf(fptr_irq2, "%d", &data);
		new_element = create_new_element(data);
		if (irq2_list == NULL) //if the list is empty, the list is now created.
			irq2_list = new_element;
		else
			append_to_linked_list(irq2_list, new_element); //if its not empty, append the next element.
	}
	fclose(fptr_irq2);
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

void download_diskin_to_hard_disk(char* diskin_path) { //downloads the entirety of diskin to a char** hard_disk.
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

void upload_ram_to_memout(char* memout_path) { //uploads the content of the ram into memout.txt
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

void upload_hard_disk_to_diskout(char* diskout_path) { //uploads the content of the hd into memout.txt
	FILE* fptr_diskout = fopen(diskout_path, "w");
	if (fptr_diskout == NULL) {
		printf("Error, couldn't open %s\n", diskout_path);
		exit(1);
	}
	for (int i = 0; i < HARD_DISK_SIZE; i++) {
		if (i == HARD_DISK_SIZE - 1)
			fprintf(fptr_diskout, "%s", hard_disk[i]); //last line
		else
			fprintf(fptr_diskout, "%s\n", hard_disk[i]);
	}
	fclose(fptr_diskout);
}

void opcode_operation(instruction inst, int* halt, int $imm, char* hwregtrace_path, char* leds_path, char* display7reg_path, int* isr_active) {
	//The heart of the simulator, recieves a parsed instruction and executes it.
	int rd = TRACE_OFFSET + inst.rd; //Initilizes rd as the "address" to the desired register.
	int rs = trace_line[TRACE_OFFSET + inst.rs]; //Initilizes rs as the value inside the register.
	int rt = trace_line[TRACE_OFFSET + inst.rt]; //Initilizes rt as the value inside the register.

	if ((rd == 2 || rd == 3) && (inst.opcode <= 8 || inst.opcode == 15 || inst.opcode == 16 || inst.opcode == 19)) { //no writing to $imm and $zero.
		printf("Error, cannot write directly to registers $zero and $imm!");
		exit(1);
	}

	if ($imm) //if $imm != 0 // I-format
		next_pc = trace_line[0] + 2; //increment pc by 2
	else //R-format
		next_pc = trace_line[0] + 1; //increment pc by 1
	
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
		clk_manager();
		if (rs + rt >= MEM_MAX_SIZE || rs + rt < 0) {
			printf("Error, memory request RAM[%d] out of bounds, Exiting.", rs + rt);
			exit(1);
		}
		trace_line[rd] = hex_string_to_int_signed(ram[rs + rt]); 
		break;

	case 17: //sw
		cycles++; //store value in memory, increase 1 cycle.
		clk_manager();
		if (rs + rt >= MEM_MAX_SIZE || rs + rt < 0) {
			printf("Error, memory request RAM[%d] out of bounds, Exiting.", rs + rt);
			exit(1);
		}
		sprintf(ram[rs + rt], "%05X", 0x000fffff & trace_line[rd]); //i want to copy only the 20lsb.
		break;

	case 18: //reti
		next_pc = io_line[irqreturn];
		*isr_active = 0;
		break;

	case 19: //in
		trace_line[rd] = io_line[rs + rt];
		update_hwregtrace_file(hwregtrace_path, inst.opcode, rs + rt);
		break;

	case 20: //out
		io_line[rs + rt] = trace_line[rd];
		if (rs + rt == leds){
			update_leds_display_file(leds_path, leds);
		}
		if (rs + rt == display7reg){
			update_leds_display_file(display7reg_path, display7reg);
		}
		update_hwregtrace_file(hwregtrace_path, inst.opcode, rs + rt);
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
				fprintf(fptr, "%03X ", trace_line[i]); //Space is added here and not \t or \n.
			}
		}
		else if (i == 1) { //Instruction with 5 bytes.
			if (trace_line[i] == 0)
				fprintf(fptr, "%s", "00000 ");
			else {
				fprintf(fptr, "%05X ", trace_line[i]);
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
					fprintf(fptr, "%08X ", trace_line[i]);
				else {
					if (trace_line[1] < 0x15000) //if instruction is halt aka last instruction.
						fprintf(fptr, "%08X\n", trace_line[i]); //last register need to print thus going down a line.
					else
						fprintf(fptr, "%08X\n", trace_line[i]); //last register need to print thus going down a line.
				}
			}
			else {
				if (i != NUM_OF_REGISTERS + 1)
					fprintf(fptr, "%08X ", trace_line[i]);
				else {
					if (trace_line[1] < 0x15000) //if instruction is halt aka last instruction.
						fprintf(fptr, "%08X\n", trace_line[i]); //last register need to print thus going down a line.
					else
						fprintf(fptr, "%08X", trace_line[i]); //last register need to print thus going down a line.
				}
			}
		}
	}
	fclose(fptr);
	return;
}

void simulator(char* trace_path, char* hwregtrace_path, char* leds_path, char* display7reg_path) { 
	//simulates the proccessor, inside of it it will fetch an instruction, decode it and execute it with the help of opcode_operation().
	FILE* fptr_trace = fopen(trace_path, "w"); //init file.
	if (fptr_trace == NULL) {
		printf("Error, couldn't open %s\n", trace_path);
		exit(1);
	}
	fclose(fptr_trace);
	FILE* fptr_hwregtrace = fopen(hwregtrace_path, "w"); //init file.
	if (fptr_hwregtrace == NULL) {
		printf("Error, couldn't open %s\n", hwregtrace_path);
		exit(1);
	}
	fclose(fptr_hwregtrace);
	FILE* fptr_leds = fopen(leds_path, "w"); //init file.
	if (fptr_leds == NULL) {
		printf("Error, couldn't open %s\n", leds_path);
		exit(1);
	}
	fclose(fptr_leds);
	FILE* fptr_display7reg = fopen(display7reg_path, "w"); //init file.
	if (fptr_display7reg == NULL) {
		printf("Error, couldn't open %s\n", display7reg_path);
		exit(1);
	}
	fclose(fptr_display7reg);

	int isr_active = 0, dma_start_cycle = 0;
	for (int $imm = 0, halt = 0; !halt; trace_line[0] = next_pc, $imm = 0) { //while halt was not asserted, update to the next PC and and execute its command.
		isr_operation(&isr_active); //check if there are interrupts, if there are than it will update pc (trace_line[0]) accordingly. 
		cycles++; //load instruction from memory, increase 1 cycle.
		clk_manager();
		instruction inst = parse_instruction(ram[trace_line[0]]); //convert it to the instruction structure.
		trace_line[1] = (inst.opcode << 12) + (inst.rd << 8) + (inst.rs << 4) + inst.rt; //update the instruction "register".
		if (inst.rs == 1 || inst.rt == 1 || inst.rd == 1) { //$imm is present in the command, load ivalue from memory, increase 1 cycle.
			trace_line[3] = hex_string_to_int_signed(ram[trace_line[0] + 1]); //load I-value to $imm, C compiler will preform sign extension auto.
			//printf("$imm found, equal to %d\n", trace_line[3]); //TEST
			cycles++;
			clk_manager();
			$imm = 1; //set $imm to be 1 (=true) since it is present.
		}
		else
			trace_line[3] = 0;
		update_trace_file(trace_path); //print out trace_line to trace.txt
		//printf("instruction = %X \t PC = %X \t %s, %s, %s, %s %d\n\n rd = %d\t rs = %d\t rt = %d\n\n", trace_line[1], trace_line[0], opcodes[inst.opcode], registers[inst.rd], registers[inst.rs], registers[inst.rt], trace_line[3], trace_line[2 + inst.rd], trace_line[2 + inst.rs], trace_line[2 + inst.rt]); //TEST
		opcode_operation(inst, &halt, $imm, hwregtrace_path, leds_path, display7reg_path, &isr_active); //do the instruction.

		//IO REGISTERS MANAGERS
		io_register_size_validator();
		monitor_manager();
		timer_manager();
		hard_disk_manager(&dma_start_cycle);
		irq2_manager();
		//printf("irqstatus0/1/2 = %d/%d/%d \t timercurrent = %d, cycles = %d\n\n", io_line[3], io_line[4], io_line[5], io_line[12], cycles); //TEST
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
				fprintf(fptr, "%08X", trace_line[i]);
			else
				fprintf(fptr, "%08X\n",trace_line[i]);
		}
		else {
			if (i == TRACE_OFFSET + NUM_OF_REGISTERS - 1)
				fprintf(fptr, "%08X", trace_line[i]); //last register.
			else
				fprintf(fptr, "%08X\n", trace_line[i]); 
		}
	}
	fclose(fptr);
}

void timer_manager() { //will maintain irq0 and all the timer-relevent registers.
	if (io_line[timerenable]) {
		io_line[irq0enable] = 1; //set irq0 high when timerenable is high.

		if (io_line[timercurrent] == io_line[timermax]) { //when currenttimer reached timermax.
			io_line[irq0status] = 1; //set irq0status.
			io_line[timercurrent] = 0; //reset timer
		}
		else {
			io_line[timercurrent] += 1; //increment timercurrent.
		}
	}
	else
		io_line[irq0enable] = 0; //if timerenable is not on, reset interrupt. 
}

void hard_disk_manager(int* dma_start_cycle) { //updates the hard disk status and maintains the HD-relevent registers.
	if (!io_line[diskstatus] && io_line[diskcmd]) { // if diskstatus == 0  and diskcmd != 0, as in not busy and requesting transaction.
		io_line[diskstatus] = 1; //set dma to busy.
		*dma_start_cycle = cycles; //save the moment we've started the transaction.
	}
	else if (cycles - *dma_start_cycle >= DMA_ACTIVE_DURATION && io_line[diskstatus]) { //if 1024 clock cyles had passed since the DMA started, finish transaction.

		if (io_line[diskbuffer] + SECTOR_SIZE >= MEM_MAX_SIZE){ //making sure we didnt overflow.
			printf("Overflowing alocatted RAM size\n");
			assert(NULL);
		}
		if (io_line[diskcmd] == 1) { //read HD -> RAM
			for (int i = 0; i < SECTOR_SIZE; i++) {
				sprintf(ram[io_line[diskbuffer] + i], "%s", hard_disk[SECTOR_SIZE*io_line[disksector] + i]);
			}
		}
		else if (io_line[diskcmd] == 2) { //write RAM -> HD
			for (int i = 0; i < SECTOR_SIZE; i++){
				sprintf(hard_disk[SECTOR_SIZE*io_line[disksector] + i], "%s", ram[io_line[diskbuffer] + i]);
			}
		}
		io_line[diskstatus] = io_line[diskcmd] = 0; //DMA done, reset registers.
		io_line[irq1status] = 1; //assert interrupt.
	}
}

void monitor_manager(){ //updates the monitor current status and maintains its relvent registers.
	if (io_line[monitorcmd])
		set_pixel(display, io_line[monitoraddr]/MONITOR_DIM, io_line[monitoraddr]%MONITOR_DIM, io_line[monitordata]);
}

void irq2_manager(){ //updates the linked list that holds all the interrupts from irq2 file.
	if (irq2_list != NULL) //if we havent reached the end of the file = end of the linked list.
		if (cycles >= irq2_list->data) {
			io_line[irq2status] = 1;
			irq2_list = irq2_list->next;
		}
	}

void isr_operation(int* isr_active) { 
	//assuming the proccess is:  check irq -> fetch irqhandler instruction -> handle IRQ ->fetch irqreturn instruction.
	//this function maintains the interrupt-relvent registers and if ISR is not busy and an IRQ is set it will handle it.
	if (!(*isr_active)) { //check 1 status at a time and handle them seperately.
		if ((io_line[irq0enable] & io_line[irq0status])) {
			io_line[irq0status] = 0; //reset irq0status
			*isr_active = 1; //set the ISR active for future proding.
			io_line[irqreturn] = trace_line[0]; //saves current pc in irqreturn register.
			trace_line[0] = io_line[irqhandler]; //sets current pc to address set inside irqhandler.
			//printf("######################ISR STARTED#####################\n");//FOR TEST
		}
		else if (io_line[irq1enable] & io_line[irq1status]) {
			io_line[irq1status] = 0;
			*isr_active = 1;
			io_line[irqreturn] = trace_line[0]; 
			trace_line[0] = io_line[irqhandler];
			//printf("######################ISR STARTED#####################\n");//FOR TEST
		}
		else if (io_line[irq2enable] & io_line[irq2status]) { 
			io_line[irq2status] = 0;
			*isr_active = 1;
			io_line[irqreturn] = trace_line[0]; 
			trace_line[0] = io_line[irqhandler]; 
			//printf("######################ISR STARTED#####################\n"); //FOR TEST
		}
	}
}

void update_leds_display_file(char* path, int io_reg) { //updates either the leds.txt or the display7seg.txt 
	FILE* fptr = fopen(path, "a");
	assert(fptr);
	fprintf(fptr, "%u %08X\n", cycles, io_line[io_reg]);
	fclose(fptr);
}

void update_hwregtrace_file(char* hwregtrace_path, int opcode, int hwregister_index) { //updates hwregtrace.txt file.
	FILE* fptr_hwregtrace = fopen(hwregtrace_path, "a");
	if (fptr_hwregtrace == NULL) {
		printf("Error, couldn't open %s\n", hwregtrace_path);
		exit(1);
	}

	char operation[6];
	if (opcode == 20)  //out
		strcpy(operation, "WRITE");
	else //in
		strcpy(operation, "READ");

	fprintf(fptr_hwregtrace, "%d %s %s %08X\n", cycles, operation, io_registers[hwregister_index], io_line[hwregister_index]); //format as instructed by the project instructions.
	fclose(fptr_hwregtrace);
}

void io_register_size_validator() { //validates the current status of the i/o registers according to their instructed size.
	int io_register_size[23] = { 1, 1, 1, 1, 1, 1, 12, 12, 32, 32, 32, 1, 32, 32, 2, 7, 12, 1, 0, 0, 16, 8, 1 };
	for (int i = 0; i < NUM_OF_IO_REGISTERS; i++) {
		if (0x1 << io_register_size[i] < io_line[i] && io_register_size[i] != 32) { //checks if the current status is bigger than whats allowed.
			printf("Error, size of i/o register '%s' is %d while current register status is %X, terminating!", io_registers[i], io_register_size[i], io_line[i]);
			exit(1);
		}
	}
}

void clk_manager() { //maintains and updates the clks registers each cycle.
	if (io_line[clks] == 0xffffffff)
		io_line[clks] = 0;
	else
		io_line[clks]++;
}