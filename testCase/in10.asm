section .data
a : 
	dd 0
b : 
	dd 10
c : 
	db 97
p : 
	dq 0
pa : 
	dq a
str : 
	dq CSTRING_0
CSTRING_0 : 
	db  92,72,101,108,108,111,10,0
str2 : 
	dq CSTRING_1
CSTRING_1 : 
	db  92,87,111,114,108,100,10,0
arr : 
	times 10 dd 0
sa : 
	dd 0
	db 0
	dq 0


        global  main
        extern  puts
        section .text
main :


	push	rbp
	mov	rbp,rsp
	sub     rsp, 28h
	call	fun
	add     rsp, 28h 
	sub     rsp, 28h
	push	QWORD [rel b]
	call	fun1
	pop	rcx
	pop	rdx
	add     rsp, 28h 
	mov	DWORD [rel a],ecx
	mov	esi,DWORD [rel a]
	cmp	esi,20
	je	.IF_THEN_1
	jmp	.IF_END_1
.IF_THEN_1: 

	sub     rsp, 28h
	mov	rcx,[rel str]
	call	puts
	add     rsp,28h
	sub     rsp, 28h
	mov	rcx,[rel str2]
	call	puts
	add     rsp,28h
.IF_END_1: 

	sub     rsp, 28h
	mov	rcx,[rel str]
	call	puts
	add     rsp,28h
	mov	rsp,rbp
	mov	edi,0
	pop	rbp
	ret
	mov	rsp,rbp
	pop	rbp
	ret
fun :


	push	rbp
	mov	rbp,rsp
	sub	rsp,4
	mov	 DWORD [rbp-4],0
	sub	rsp,4
	mov	 DWORD [rbp-8],2
	mov	eax,DWORD [rbp-8]
	cmp	eax,DWORD [rbp-4]
	jg	.IF_THEN_0
	mov	ecx,DWORD [rbp-8]
	add	ecx,DWORD [rbp-4]
	mov	DWORD [rbp-8],ecx
	jmp	.IF_END_0
.IF_THEN_0: 

	sub     rsp, 28h
	mov	rcx,[rel str2]
	call	puts
	add     rsp,28h
.IF_END_0: 

	add	rsp,8
	mov	rsp,rbp
	pop	rbp
	ret
fun1 :


	push	rbp
	mov	rbp,rsp
	mov	eax,DWORD [rbp+16]
	add	eax,10
	mov	DWORD [rbp+16],eax
	mov	rsp,rbp
	mov	ecx,DWORD [rbp+16]
	pop	rbp
	ret
	mov	rsp,rbp
	pop	rbp
	ret
