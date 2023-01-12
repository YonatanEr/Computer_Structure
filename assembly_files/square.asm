.word   0x100   50                          #upper left point of the square
.word   0x100   10                          #length of all the sqaure edges 
lw		$s0,	$zero,	$imm,	0x100		#load 0x100 to $s0, start
blt     $imm,   $zero,  $s0,    HALT		# jump to HALT if square is overflowing (too small)
lw		$s1,	$zero,	$imm,	0x101		#load 0x101 to $s1, len
add     $s2,    $zero,  $imm,   65535       #s2 = 65535
add		$t0,	$s0,	$s1,	0	    	#t0 = start + len
mul		$t1,	$s0,	$imm,	256	    	#t1 = start * 256 
add     $t0,	$s0,	$s1,	0	    	#t0 = start + len + start * 256 
bgt     $imm,   $t0,    $s2,    HALT		# jump to HALT if square is overflowing (too big)

add		$t0,	$zero,	$s0,	0	    	#t0 = start
add		$t1,	$zero,	$zero,	0	    	#t1 = 0

# VALIDATED Input #


HALT:
halt	$zero,	$zero,	$zero,	0			#Termination
