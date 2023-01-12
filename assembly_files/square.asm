.word   0x100   50                          #upper left point of the square
.word   0x100   10                          #length of all the sqaure edges 
lw		$s0,	$zero,	$imm,	0x100		#load 0x100 to $t0.
lw		$s1,	$zero,	$imm,	0x101		#load 0x101 to $t1.
lw		$s0,	$zero,	$imm,	65535		#load 0x101 to $s0.


z = a + len + len*256

ROW:
bgt $imm, $a0, $t2, L1				# jump to L1 if x > 1


COL:    
out     
lw		$t1,	$zero,	$imm,	0x101		#load 1 to $t1.
add		$s0,	$zero,	$imm,	0x7ffff		#set $s0 = 0x7ffff, upper bound of the fibo series, bigger number will cause overflow.
add		$s1,	$zero,	$imm,	1			#set $s1 = 1
add		$gp,	$zero,	$imm,	0x102		#$gp = 0x102, address from which we will start uploading the fibo series.
jal		$ra,	$imm,	$zero,	Fib			#start calculating fibo series and set $ra as the address for halt.
halt	$zero,	$zero,	$zero,	0			#after returning from calculating, halt.
Fib:
beq     $
add		$t0,	$t0,	$t1,	0			#calculate the next element in the series and set it as $t0.
bgt		$ra,	$t0,	$s0,	0			#if $t0 is overflowing, return to halt.
sw		$t0,	$zero,	$gp,	0			#store $t0 in the next row of the memory.
add		$gp,	$gp,	$s1,	0			#increment gp by 1 (gp++).
add		$t1,	$t0,	$t1,	0			#calculate the next element in the series and set it as $t1.
bgt		$ra,	$t1,	$s0,	0			#if $t1 is overflowing, return to halt.
sw		$t1,	$zero,	$gp,	0			#store $t1 in the next row of the memory.
add		$gp,	$gp,	$s1,	0			#increment gp by 1 (gp++).
bne		$imm,	$zero,	$s1,	Fib			#we set $s1 to 1 so always branches back to Fib.
.word	0x101	0x1							#load 1 to address 257, address 256 will hold 0 by default and the fibo series will start from there. 

HALT:
halt	$zero,	$zero,	$zero,	0			#Termination
