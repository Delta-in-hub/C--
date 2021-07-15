section .data
arr : 
	times 10 dd 0
a : 
	dd 12
p : 
	dq 0
CSTRING_0 : 
	db  97,32,61,61,32,49,48,0
CSTRING_1 : 
	db  87,104,105,108,101,32,76,111,111,112,105,110,103,0
CSTRING_2 : 
	db  70,111,114,32,76,111,111,112,105,110,103,0

        global  main
        extern  puts
        extern  getchar
section .text
main :


	push	rbp
	mov	rbp,rsp
	mov	eax,0
	shl	eax,2
	mov	DWORD [rel arr+eax],10
	mov	ecx,0
	shl	ecx,2
	mov	edx,0
	add	edx,DWORD [rel arr+ecx]
	mov	esi,1
	shl	esi,2
	mov	DWORD [rel arr+esi],edx
	mov	DWORD [rel p],a
	mov	edi,[rel p]
	mov	DWORD [edi],20
	mov	r8d,0
	shl	r8d,2
	mov	r9d,1
	shl	r9d,2
	mov	r10d,DWORD [rel arr+r8d]
	add	r10d,DWORD [rel arr+r9d]
	mov	r11d,DWORD [rel a]
	cmp	r11d,r10d
	je	.IF_THEN_0
	jmp	.IF_END_0
.IF_THEN_0: 

	sub     rsp, 28h
	mov	rcx,CSTRING_0
	call	puts
	add     rsp,28h
.IF_END_0: 

.WHILE_START_1 :

	mov	eax,DWORD [rel a]
	cmp	eax,23
	jle	.WHILE_THEN_1
	jmp	.WHILE_END_1
.WHILE_THEN_1 : 

	sub     rsp, 28h
	mov	rcx,CSTRING_1
	call	puts
	add     rsp,28h
	INC	DWORD [rel a]
	jmp	.WHILE_START_1
.WHILE_END_1 :

	mov	DWORD [rel a],5
.FOR_START_2 : 

	mov	ecx,DWORD [rel a]
	cmp	ecx,0
	jne	.FOR_THEN_2
	jmp	.FOR_END_2
.FOR_THEN_2 : 

	sub     rsp, 28h
	mov	rcx,CSTRING_2
	call	puts
	add     rsp,28h
	DEC	DWORD [rel a]
	jmp	.FOR_START_2
.FOR_END_2 : 

	sub     rsp, 28h
	push	rax
	call	getchar
	mov	edx,eax
	pop	rax
	add     rsp,28h
	mov	rsp,rbp
	mov	esi,0
	pop	rbp
	ret
	mov	rsp,rbp
	pop	rbp
	ret
