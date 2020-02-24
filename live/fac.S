	.text
.global	fac
fac:
	sub	    #4, r1
	mov	    r15, 2(r1)
	cmp	    #0, 2(r1)
	jne	    .Lloop
	mov	    #1, r15
	jmp	    .Lend
.Lloop:
	mov     2(r1), r15
	dec     r15
	call    #fac
	mov     2(r1), r14
	call	#__mulhi3
	mov	    r15, @r1
	mov	    @r1, r15
.Lend:
	add	    #4, r1
	ret
