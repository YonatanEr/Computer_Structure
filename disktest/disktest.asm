add		$s0,	$zero,	$imm,	8				# init s0 = 8.
add		$s1,	$zero,	$imm,	0x200			# init s1 = 0x200.
add		$s2,	$zero,	$imm,	1				# init s2 = 1.
add		$gp,	$zero,	$imm,	0x100			# init gp = 0x100.
add		$t0,	$zero,	$imm,	ISR				# set $t0 to the address of the ISR operation.
out		$t0,	$zero,	$imm,	6				# set irqhandler as the address of the ISR operation.
out		$s2,	$zero,	$imm,	1				# enable irq1, irq1 = 1.
out		$s1,	$zero,	$imm,	16				# diskbuffer = $s1 = 512.
out		$s2,	$zero,	$imm,	14				# diskcmd = READ, disksector is by default set to 0 at the start.
DMA1:
in		$t0,	$zero,	$imm,	17				# $t0 = diskstatus.
bne		$imm,	$t0,	$zero,	DMA1			# keep checking if dma is busy, if yes then wait, if not then continue.
add		$s1,	$s1,	$imm,	128				# set $s1 = 512 + 128 = 640.
out		$s1,	$zero,	$imm,	16				# diskbuffer = 640.
out		$s2,	$zero,	$imm,	15				# disksector = 1.
out		$s2,	$zero,	$imm,	14				# diskcmd = READ.
DMA2:
in		$t0,	$zero,	$imm,	17				# $t0 = diskstatus.
bne		$imm,	$t0,	$zero,	DMA2			# keep checking if dma is busy, if yes then wait, if not then continue.
lw		$t0,	$zero,	$imm,	0x100			# load first sum to $t0.
lw		$t1,	$zero,	$imm,	0x101			# load second sum to $t1.
bgt		$imm,	$t0,	$t1,	Correct			# if first sum is greater than second sum, branch to Correct.
sw		$t1,	$zero,	$gp,	0				# we didn't branch to "Correct" meaning the second sum is greater, store and halt.
halt	$zero,	$zero,	$zero,	0
Correct:
sw		$t0,	$zero,	$gp,	0				# store and halt
halt	$zero,	$zero,	$zero,	0
ISR:
add		$t0,	$zero,	$zero,	0				# t0 = 0
add		$t2,	$zero,	$zero,	0				# t2 = 0
Calculate:
lw		$t1,	$t0,	$s1,	0				# $t1 = ram[$s1 + $t0], $s1 holds diskbuffer.
add		$t2,	$t2,	$t1,	0				# $t2 += $t1
add		$t0,	$t0,	$imm,	1				# $t0++
blt		$imm,	$t0,	$s0,	Calculate		# while i<8 keep computing the sum of the first 8 words.	
sw		$t2,	$gp,	$zero,	0				# store calculated sum in ram[$gp].
add		$gp,	$gp,	$imm,	1				# gp++
reti	$zero,	$zero,	$zero,	0				# returns