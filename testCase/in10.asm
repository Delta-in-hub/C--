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
	dd 0	db 0	dq 0

        global  main
        extern  puts
        section .text
main :


	mov	rbp,rsp
	sub     rsp, 28h
	call	fun
	add     rsp, 28h
	sub     rsp, 28h
	push	DWORD [rel b]
	call	fun1
	pop	ecx
	pop	edx
	add     rsp, 28h
	mov	rsp,rbp
	push	0
	ret
	mov	rsp,rbp
fun :


	mov	rbp,rsp
	sub	rsp,4
	mov	WORD [rbp-4],0
	sub	rsp,4
	mov	WORD [rbp-8],2
	cmp	DWORD [rbp-8],DWORD [rbp-4]
	jg	.IF_THEN_0
	mov	eax,DWORD [rbp-8]
	add	eax,DWORD [rbp-4]
	mov	DWORD [rbp-8],eax
	jmp	.IF_END_0
.IF_THEN_0:

	mov	ecx,DWORD [rbp-8]
	sub	ecx,DWORD [rbp-4]
	mov	DWORD [rbp-8],ecx
.IF_END_0:

	add	rsp,8
	mov	rsp,rbp
	
fun1 :


	mov	rbp,rsp
	mov	eax,DWORD [rbp+0]
	add	eax,10
	mov	DWORD [rbp+0],eax
	mov	rsp,rbp
	push	DWORD [rbp+0]
	ret
	mov	rsp,rbp
