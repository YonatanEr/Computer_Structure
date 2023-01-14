lw		$t0,	$zero,	$imm,	0x100		                 #load 0x100 to $t0 = start_address
blt     $imm,   $zero,  $t0,    HALT	        	         #jump to HALT if square is overflowing (too small)
lw		$s1,	$zero,	$imm,	0x101	    	             #load 0x101 to $s1, len
add		$t1,	$t0,	$s1,	0	    	                 #t1 = start_address + len
mul		$t2,	$t0,	$imm,	256	    	                 #t2 = start_address * 256 
add     $s2,	$t1,	$t2,	0	    	                 #s2 = t1 + t2 = start_address + len + start_address * 256 = max_square_pixel
add     $t2,    $zero,  $imm,   65535                        #t2 = 65535
bgt     $imm,   $s2,    $t2,    HALT		                 #jump to HALT if square is overflowing (too big)
CalcRightLen:
blt     $imm,	$t0,	$t2,	NEXT	                     #if t0 < t2, branch
add		$t2,	$t2,	$zero,	256	    	                 #t2 = t2 + 256
beq     $imm,   $t2,    $t2,    CalcRightLen                 #Goto CalcRightLen              
NEXT:
sub     $s0,    $t2,    $t0,    0                            #s0 = t2 - t0 = (len - start_address) % 256 = square_pixels_per_line
add     $t0,    $zero,  $imm,   255			                 #$t0 = 255
out     $t0,    $zero,  $imm,   21			                 #set pixel color to white
add		$t0,	$s0,	$zero,	0	    	                 #t0 = start_address
out     $t0,    $zero,  $imm,   20			                 # update address
add		$t1,	$zero,	$imm,	1	    	                 #t1 = 1
out     $t1,    $zero,  $imm,   22			                 # draw pixel
SetLine:
add		$t1,	$t0,	$imm,	0	    	                 #t1 = t0 = line_start_drawing_pixel
add		$t2,	$t0,	$s1,	0	    	                 #t2 = t0 + s1 = line_stop_drawing_pixel
#t0 = line_start_drawing_pixel
#t1 = running_index:                    t0 <= t1 < t2
#t2 = line_stop_drawing_pixel
#s0 = square_pixels_per_line
#s1 = len
#s2 = max_square_pixel
Draw:               
out     $t1,    $zero,  $imm,   20			                 # update address
add     $t1,    $t1,    $imm,   1                            # t1 = t1 + 1
beq     $imm,   $t1,    $s2,    HALT                         # reached max_square_pixel
beq     $imm,   $t1,    $s2,    IncLine                      # reached line_stop_drawing_pixel
beq     $imm,   $t2,    $t2,    Draw                         #Goto Draw              
IncLine:
add     $t0,    $t0,    $imm,   256                            
beq     $imm,   $t2,    $t2,    SetLine                      #Goto SetLine              
HALT:
add		$t1,	$zero,	$imm,	0	    	                 #t1 = 0
out     $t1,    $zero,  $imm,   22			                 # stop drawing pixels
halt	$zero,	$zero,	$zero,	0			                 #Termination
