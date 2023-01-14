lw		$t0,	$zero,	$imm,	0x100		#load 0 to $t0.
lw		$t1,	$zero,	$imm,	0x101		#load 1 to $t1.
add		$t2,	$zero,	$imm,	IRQ			#set $t2 = address of IRQ
out		$t2,	$zero,	$imm,	6			#set irqhandler = IRQ.
add		$t2,	$zero,	$imm,	40			#t2 = 40
out		$t2,	$zero,	$imm,	13			#set timermax
out		$t1,	$zero,	$imm,	11			#set timerenable
out		$t1,	$zero,	$imm,	1			#set irq1
out		$t1,	$zero,	$imm,	2			#set irq2
add		$s0,	$zero,	$imm,	0x7ffff		#set $s0 = 0x7ffff, upper bound of the fibo series, bigger number will cause overflow.
add		$s1,	$zero,	$imm,	1			#set $s1 = 1
add		$gp,	$zero,	$imm,	0x102		#$gp = 0x102, address from which we will start uploading the fibo series.
jal		$ra,	$imm,	$zero,	Fib			#start calculating fibo series and set $ra as the address for halt.
halt	$zero,	$zero,	$zero,	0			#after returning from calculating, halt.
Fib:
add		$t0,	$t0,	$t1,	0			#calculate the next element in the series and set it as $t0.
bgt		$ra,	$t0,	$s0,	0			#if $t0 is overflowing, return to halt.
sw		$t0,	$zero,	$gp,	0			#store $t0 in the next row of the memory.
add		$gp,	$gp,	$s1,	0			#increment gp by 1 (gp++).
add		$t1,	$t0,	$t1,	0			#calculate the next element in the series and set it as $t1.
bgt		$ra,	$t1,	$s0,	0			#if $t1 is overflowing, return to halt.
sw		$t1,	$zero,	$gp,	0			#store $t1 in the next row of the memory.
add		$gp,	$gp,	$s1,	0			#increment gp by 1 (gp++).
bne		$imm,	$zero,	$s1,	Fib			#we set $s1 to 1 so always branches back to Fib.
IRQ:
add		$a0,	$imm,	$zero,	255			#set a0 = 255
add		$a1,	$imm,	$zero,	1			#set a1 = 1
add		$a2,	$imm,	$zero,	0xFF			#set a2 = 15
out		$a0,	$imm,	$zero,	20			#set monitordata = 255
out		$a2,	$imm,	$zero,	21			#set monitoraddr = 1
out		$a1,	$imm,	$zero,	22			#set monitorcmd = 1
out		$t0,	$imm,	$zero,	9			#set leds = $t0
out		$t1,	$imm,	$zero,	10			#set display7seg = $t1
reti	$zero,	$zero,	$zero,	0			#return/set pc = irqreturn
.word	0x101	0x1							#load 1 to address 257, address 256 will hold 0 by default and the fibo series will start from there.