    .file "divd.S"
    .text
.global divd
divd: ;label
    mov     r15, r15
	clr		r13 ;This will be the value I increment to get back my quotient. I want to make sure it is initialized at 0	
 
	_cyc: ;label where I wish to define my loop 

	sub		r14, r15; subtract r14-r15
	inc		r13
	cmp		r14, r15;I'm looking for a conditional jump when the zf = 0 & sf = 0 
    jge		_cyc ;jump if greater or equal
	
	mov     r15, r14; move whatevers left of the quotient into the remainder register
	mov		r13, r15; move the quotient into the r15 register
		
    ret
