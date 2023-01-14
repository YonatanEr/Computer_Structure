.word	0x100   3                                          #start_address
.word	0x101	1                                            #len
lw		$t0,	$zero,	$imm,	0x100		                 #load 0x100 to $t0 = start_address
lw		$s1,	$zero,	$imm,	0x101	    	             #load 0x101 to $s1, len
add     $t2,    $t0,    $s1,    0                            #t2 = t0 + s1
blt     $imm,   $t0,    $zero,  HALT	        	         #jump to HALT if square is overflowing (start_address<0)
add     $t2,    $zero,  $imm,   65535                        #t2 = 65535
bgt     $imm,   $t0,    $t2,    HALT	        	         #jump to HALT if square is overflowing (start_address>65535)
blt     $imm,   $s1,    $zero,  HALT	        	         #jump to HALT if square is overflowing (len<0)
add     $t2,    $zero,  $imm,   255                          #t2 = 255
bgt     $imm,   $s1,    $t2,    HALT	        	         #jump to HALT if square is overflowing (len>255)
add		$t1,	$t0,	$s1,	0	    	                 #t1 = start_address + len
bgt     $imm,   $t1,    $t2,    HALT	        	         #jump to HALT if square is overflowing (start_address+len>255)
mul		$t2,	$s1,	$imm,	256	    	                 #t2 = len * 256 
add     $s2,	$t1,	$t2,	0	    	                 #s2 = t1 + t2 = start_address + len + len * 256 = square_stop_drawing_pixel
add     $t2,    $zero,  $imm,   65535                        #t2 = 65535
bgt     $imm,   $s2,    $t2,    HALT		                 #jump to HALT if square is overflowing (too big)
add     $t2,    $imm,   $zero,  256                          #t2 = 256
CalcRightLen:
blt     $imm,	$t0,	$t2,	Next	                     #if t0 < t2, branch
add		$t2,	$t2,	$imm,	256	    	                 #t2 = t2 + 256
beq     $imm,   $t2,    $t2,    CalcRightLen                 #Goto CalcRightLen              
Next:
sub     $t2,    $t2,    $t0,    0                            #t2 = t2 - t0 = 256 - (start_address % 256)
sub     $t2,    $imm,   $t2,    256                          #t2 = 256 - t0 = start_address % 256
add     $s0,    $s1,   $t2,     0                            #s0 = s1 + t2 = (len + start_address) % 256
add     $t2,    $zero,  $imm,   255			                 #t2 = 255
out     $t2,    $zero,  $imm,   21			                 #set pixel color to white
out     $t0,    $zero,  $imm,   20			                 #update address
add     $t1,    $zero,  $imm,   1			                 #t2 = 255
out     $t1,    $zero,  $imm,   22			                 # draw pixel
add		$ra,	$t0,	$imm,	0	    	                        #t0 = start_address
add		$ra,	$s0,	$imm,	0	    	                        #s0 = (start_address+len) % 256
add		$ra,	$s1,	$imm,	0	    	                        #s1 = len
add		$ra,	$s2,	$imm,	0	    	                        #s2 = square_stop_drawing_pixel
add		$t2,	$zero,	$s0,	0	    	                 #t2 = line_stop_drawing_pixel
SetLine:
add		$t1,	$zero,	$t0,	0	    	                 #t1 = t0 = line_start_drawing_pixel
DrawLine:               
out     $t1,    $zero,  $imm,   20			                 # update address
add     $t1,    $t1,    $imm,   1                            # t1 = t1 + 1
blt     $imm    $t1,    $t2,    DrawLine
bge     $imm,   $t1,    $s2,    HALT                         # reached max_square_pixel
bge     $imm,   $t1,    $s2,    IncLine                      # reached line_stop_drawing_pixel
IncLine:
add     $t0,    $t0,    $imm,   256                          #t0 = t0 + 256, next_line_start_drawing_pixel  
add     $t2,    $t2,    $imm,   256                          #t2 = t2 + 256, next_line_stop_drawing_pixel  
beq     $imm,   $t2,    $t2,    SetLine                      #Goto SetLine              
HALT:
add		$t1,	$zero,	$imm,	0	    	                 #t1 = 0
out     $t1,    $zero,  $imm,   22			                 # stop drawing pixels
halt	$zero,	$zero,	$zero,	0			                 #Termination