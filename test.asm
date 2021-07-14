section .data
a : 
	dd 10
b : 
	dd 30
str : 
	dq CSTRING_0
CSTRING_0 : 
	db  72,101,108,108,111,9,87,111,114,108,100,33,10,0
CSTRING_1 : 
	db  65,32,101,113,117,97,108,115,32,116,111,32,66,10,0

        global  main
        extern  puts
        extern  getchar
section .text
main :


	push	rbp
	mov	rbp,rsp
	sub	rsp,4
	sub	rsp,4
	mov	 DWORD [rbp-8],2
	mov	DWORD [rbp-4],0
.FOR_START_0 : 

	mov	eax,DWORD [rbp-4]
	cmp	eax,DWORD [rbp-8]
	jl	.FOR_THEN_0
	jmp	.FOR_END_0
.FOR_THEN_0 : 

	sub     rsp, 28h
	mov	rcx,[rel str]
	call	puts
	add     rsp,28h
	INC	DWORD [rbp-4]
	jmp	.FOR_START_0
.FOR_END_0 : 

	sub     rsp, 28h
	call	fun1
	add     rsp, 28h 
	mov	edx,DWORD [rel b]
	add	edx,1
	mov	esi,DWORD [rel a]
	cmp	esi,edx
	je	.IF_THEN_1
	jmp	.IF_END_1
.IF_THEN_1: 

	sub     rsp, 28h
	mov	rcx,CSTRING_1
	call	puts
	add     rsp,28h
.IF_END_1: 

	sub     rsp, 28h
	push	rax
	call	getchar
	mov	edi,eax
	pop	rax
	add     rsp,28h
	mov	rsp,rbp
	mov	r8d,0
	pop	rbp
	ret
	add	rsp,8
	mov	rsp,rbp
	pop	rbp
	ret
fun1 :


	push	rbp
	mov	rbp,rsp
	mov	DWORD [rel a],30
	mov	rsp,rbp
	pop	rbp
	ret
