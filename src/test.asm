section .data
arr : 
	times 10 dd 0
a : 
	dd 12
p : 
	dq 0
CSTRING_0 : 
	db  97,32,61,61,32,49,48,0

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
	mov	ecx,1
	shl	ecx,2
	mov	DWORD [rel arr+ecx],10
	mov	DWORD [rel p],a
	mov	edx,[rel p]
	mov	DWORD [edx],20
	mov	esi,0
	shl	esi,2
	mov	edi,1
	shl	edi,2
	mov	r8d,DWORD [rel arr+esi]
	add	r8d,DWORD [rel arr+edi]
	mov	r9d,DWORD [rel a]
	cmp	r9d,r8d
	je	.IF_THEN_0
	jmp	.IF_END_0
.IF_THEN_0: 

	sub     rsp, 28h
	mov	rcx,CSTRING_0
	call	puts
	add     rsp,28h
.IF_END_0: 

	mov	rsp,rbp
	mov	r10d,0
	pop	rbp
	ret
	mov	rsp,rbp
	pop	rbp
	ret
