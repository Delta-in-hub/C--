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
	db "Hello"
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
	sub     rsp, 28h
	mov	rcx,[rel str]
	call	puts
	add     rsp,28h
	mov	rsp,rbp
	mov	esi,0
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

	mov	edx,DWORD [rbp-8]
	sub	edx,DWORD [rbp-4]
	mov	DWORD [rbp-8],edx
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
