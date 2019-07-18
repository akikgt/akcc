	.file	"parse.c"
	.intel_syntax noprefix
	.text
	.comm	user_input,8,8
	.local	tokens
	.comm	tokens,8,8
	.local	pos
	.comm	pos,4,4
	.local	prog
	.comm	prog,8,8
	.local	cur_fn
	.comm	cur_fn,8,8
	.local	lvars
	.comm	lvars,8,8
	.local	g_offset
	.comm	g_offset,4,4
	.local	str_count
	.comm	str_count,4,4
	.local	switches
	.comm	switches,8,8
	.local	breaks
	.comm	breaks,8,8
	.local	continues
	.comm	continues,8,8
	.globl	format
	.type	format, @function
format:
.LFB0:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 2288
	mov	QWORD PTR -2280[rbp], rdi
	mov	QWORD PTR -168[rbp], rsi
	mov	QWORD PTR -160[rbp], rdx
	mov	QWORD PTR -152[rbp], rcx
	mov	QWORD PTR -144[rbp], r8
	mov	QWORD PTR -136[rbp], r9
	test	al, al
	je	.L2
	movaps	XMMWORD PTR -128[rbp], xmm0
	movaps	XMMWORD PTR -112[rbp], xmm1
	movaps	XMMWORD PTR -96[rbp], xmm2
	movaps	XMMWORD PTR -80[rbp], xmm3
	movaps	XMMWORD PTR -64[rbp], xmm4
	movaps	XMMWORD PTR -48[rbp], xmm5
	movaps	XMMWORD PTR -32[rbp], xmm6
	movaps	XMMWORD PTR -16[rbp], xmm7
.L2:
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR -184[rbp], rax
	xor	eax, eax
	mov	DWORD PTR -2272[rbp], 8
	mov	DWORD PTR -2268[rbp], 48
	lea	rax, 16[rbp]
	mov	QWORD PTR -2264[rbp], rax
	lea	rax, -176[rbp]
	mov	QWORD PTR -2256[rbp], rax
	lea	rcx, -2272[rbp]
	mov	rdx, QWORD PTR -2280[rbp]
	lea	rax, -2240[rbp]
	mov	esi, 2048
	mov	rdi, rax
	call	vsnprintf@PLT
	lea	rax, -2240[rbp]
	mov	rdi, rax
	call	strdup@PLT
	mov	rsi, QWORD PTR -184[rbp]
	xor	rsi, QWORD PTR fs:40
	je	.L4
	call	__stack_chk_fail@PLT
.L4:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	format, .-format
	.local	env
	.comm	env,8,8
	.type	new_env, @function
new_env:
.LFB1:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	QWORD PTR -24[rbp], rdi
	mov	esi, 40
	mov	edi, 1
	call	calloc@PLT
	mov	QWORD PTR -8[rbp], rax
	mov	eax, 0
	call	new_map@PLT
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	QWORD PTR [rax], rdx
	mov	eax, 0
	call	new_map@PLT
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	QWORD PTR 8[rax], rdx
	mov	eax, 0
	call	new_map@PLT
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	QWORD PTR 16[rax], rdx
	mov	eax, 0
	call	new_map@PLT
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	QWORD PTR 24[rax], rdx
	mov	rax, QWORD PTR -8[rbp]
	mov	rdx, QWORD PTR -24[rbp]
	mov	QWORD PTR 32[rax], rdx
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	new_env, .-new_env
	.type	new_var, @function
new_var:
.LFB2:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 48
	mov	QWORD PTR -24[rbp], rdi
	mov	QWORD PTR -32[rbp], rsi
	mov	DWORD PTR -36[rbp], edx
	mov	esi, 72
	mov	edi, 1
	call	calloc@PLT
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rdx, QWORD PTR -24[rbp]
	mov	QWORD PTR [rax], rdx
	mov	rax, QWORD PTR -8[rbp]
	mov	rdx, QWORD PTR -32[rbp]
	mov	QWORD PTR 16[rax], rdx
	mov	rax, QWORD PTR -8[rbp]
	mov	edx, DWORD PTR -36[rbp]
	mov	DWORD PTR 24[rax], edx
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	new_var, .-new_var
	.globl	add_lvar
	.type	add_lvar, @function
add_lvar:
.LFB3:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	QWORD PTR -24[rbp], rdi
	mov	QWORD PTR -32[rbp], rsi
	mov	rcx, QWORD PTR -32[rbp]
	mov	rax, QWORD PTR -24[rbp]
	mov	edx, 1
	mov	rsi, rcx
	mov	rdi, rax
	call	new_var
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -24[rbp]
	mov	edx, DWORD PTR 4[rax]
	mov	eax, DWORD PTR g_offset[rip]
	add	eax, edx
	mov	DWORD PTR g_offset[rip], eax
	mov	rax, QWORD PTR -24[rbp]
	mov	edx, DWORD PTR 8[rax]
	mov	eax, DWORD PTR g_offset[rip]
	mov	esi, edx
	mov	edi, eax
	call	roundup@PLT
	mov	edx, eax
	mov	rax, QWORD PTR -8[rbp]
	mov	DWORD PTR 8[rax], edx
	mov	rax, QWORD PTR -8[rbp]
	mov	eax, DWORD PTR 8[rax]
	mov	DWORD PTR g_offset[rip], eax
	mov	rax, QWORD PTR env[rip]
	mov	rax, QWORD PTR [rax]
	mov	rdx, QWORD PTR -8[rbp]
	mov	rcx, QWORD PTR -32[rbp]
	mov	rsi, rcx
	mov	rdi, rax
	call	map_put@PLT
	mov	rax, QWORD PTR lvars[rip]
	mov	rdx, QWORD PTR -8[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	vec_push@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	add_lvar, .-add_lvar
	.globl	add_gvar
	.type	add_gvar, @function
add_gvar:
.LFB4:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 48
	mov	QWORD PTR -24[rbp], rdi
	mov	QWORD PTR -32[rbp], rsi
	mov	QWORD PTR -40[rbp], rdx
	mov	DWORD PTR -44[rbp], ecx
	mov	rcx, QWORD PTR -32[rbp]
	mov	rax, QWORD PTR -24[rbp]
	mov	edx, 0
	mov	rsi, rcx
	mov	rdi, rax
	call	new_var
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rdx, QWORD PTR -40[rbp]
	mov	QWORD PTR 32[rax], rdx
	mov	rax, QWORD PTR -8[rbp]
	mov	edx, DWORD PTR -44[rbp]
	mov	DWORD PTR 56[rax], edx
	mov	rax, QWORD PTR prog[rip]
	mov	rax, QWORD PTR [rax]
	mov	rdx, QWORD PTR -8[rbp]
	mov	rcx, QWORD PTR -32[rbp]
	mov	rsi, rcx
	mov	rdi, rax
	call	map_put@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	add_gvar, .-add_gvar
	.section	.rodata
.LC0:
	.string	"undefined variable, %s"
	.text
	.type	find_var, @function
find_var:
.LFB5:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	QWORD PTR -24[rbp], rdi
	mov	rax, QWORD PTR env[rip]
	mov	QWORD PTR -16[rbp], rax
	jmp	.L12
.L15:
	mov	rax, QWORD PTR -16[rbp]
	mov	rax, QWORD PTR [rax]
	mov	rdx, QWORD PTR -24[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	map_get@PLT
	mov	QWORD PTR -8[rbp], rax
	cmp	QWORD PTR -8[rbp], 0
	je	.L13
	mov	rax, QWORD PTR -8[rbp]
	jmp	.L16
.L13:
	mov	rax, QWORD PTR -16[rbp]
	mov	rax, QWORD PTR 32[rax]
	mov	QWORD PTR -16[rbp], rax
.L12:
	cmp	QWORD PTR -16[rbp], 0
	jne	.L15
	mov	rax, QWORD PTR -24[rbp]
	mov	rsi, rax
	lea	rdi, .LC0[rip]
	mov	eax, 0
	call	printf@PLT
	mov	edi, 1
	call	exit@PLT
.L16:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	find_var, .-find_var
	.type	find_tag, @function
find_tag:
.LFB6:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	QWORD PTR -24[rbp], rdi
	mov	rax, QWORD PTR env[rip]
	mov	QWORD PTR -16[rbp], rax
	jmp	.L18
.L21:
	mov	rax, QWORD PTR -16[rbp]
	mov	rax, QWORD PTR 8[rax]
	mov	rdx, QWORD PTR -24[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	map_get@PLT
	mov	QWORD PTR -8[rbp], rax
	cmp	QWORD PTR -8[rbp], 0
	je	.L19
	mov	rax, QWORD PTR -8[rbp]
	jmp	.L20
.L19:
	mov	rax, QWORD PTR -16[rbp]
	mov	rax, QWORD PTR 32[rax]
	mov	QWORD PTR -16[rbp], rax
.L18:
	cmp	QWORD PTR -16[rbp], 0
	jne	.L21
	mov	eax, 0
.L20:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	find_tag, .-find_tag
	.type	find_enum, @function
find_enum:
.LFB7:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	QWORD PTR -24[rbp], rdi
	mov	rax, QWORD PTR env[rip]
	mov	QWORD PTR -16[rbp], rax
	jmp	.L23
.L26:
	mov	rax, QWORD PTR -16[rbp]
	mov	rax, QWORD PTR 24[rax]
	mov	rdx, QWORD PTR -24[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	map_get@PLT
	mov	QWORD PTR -8[rbp], rax
	cmp	QWORD PTR -8[rbp], 0
	je	.L24
	mov	rax, QWORD PTR -8[rbp]
	jmp	.L25
.L24:
	mov	rax, QWORD PTR -16[rbp]
	mov	rax, QWORD PTR 32[rax]
	mov	QWORD PTR -16[rbp], rax
.L23:
	cmp	QWORD PTR -16[rbp], 0
	jne	.L26
	mov	eax, 0
.L25:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	find_enum, .-find_enum
	.type	find_typedef, @function
find_typedef:
.LFB8:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	QWORD PTR -24[rbp], rdi
	mov	rax, QWORD PTR env[rip]
	mov	QWORD PTR -16[rbp], rax
	jmp	.L28
.L31:
	mov	rax, QWORD PTR -16[rbp]
	mov	rax, QWORD PTR 16[rax]
	mov	rdx, QWORD PTR -24[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	map_get@PLT
	mov	QWORD PTR -8[rbp], rax
	cmp	QWORD PTR -8[rbp], 0
	je	.L29
	mov	rax, QWORD PTR -8[rbp]
	jmp	.L30
.L29:
	mov	rax, QWORD PTR -16[rbp]
	mov	rax, QWORD PTR 32[rax]
	mov	QWORD PTR -16[rbp], rax
.L28:
	cmp	QWORD PTR -16[rbp], 0
	jne	.L31
	mov	eax, 0
.L30:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	find_typedef, .-find_typedef
	.globl	consume
	.type	consume, @function
consume:
.LFB9:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	DWORD PTR -20[rbp], edi
	mov	rax, QWORD PTR tokens[rip]
	mov	rax, QWORD PTR [rax]
	mov	edx, DWORD PTR pos[rip]
	movsx	rdx, edx
	sal	rdx, 3
	add	rax, rdx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	DWORD PTR -20[rbp], eax
	je	.L33
	mov	eax, 0
	jmp	.L34
.L33:
	mov	eax, DWORD PTR pos[rip]
	add	eax, 1
	mov	DWORD PTR pos[rip], eax
	mov	eax, 1
.L34:
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	consume, .-consume
	.section	.rodata
.LC1:
	.string	"Not '%c'"
	.text
	.type	expect, @function
expect:
.LFB10:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	DWORD PTR -20[rbp], edi
	mov	rax, QWORD PTR tokens[rip]
	mov	rax, QWORD PTR [rax]
	mov	edx, DWORD PTR pos[rip]
	movsx	rdx, edx
	sal	rdx, 3
	add	rax, rdx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR -8[rbp], rax
	mov	eax, DWORD PTR -20[rbp]
	mov	edi, eax
	call	consume
	test	eax, eax
	jne	.L37
	mov	eax, DWORD PTR -20[rbp]
	mov	esi, eax
	lea	rdi, .LC1[rip]
	mov	eax, 0
	call	format
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rax, QWORD PTR 24[rax]
	mov	rsi, rdx
	mov	rdi, rax
	call	error_at@PLT
.L37:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	expect, .-expect
	.type	peek, @function
peek:
.LFB11:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	DWORD PTR -20[rbp], edi
	mov	rax, QWORD PTR tokens[rip]
	mov	rax, QWORD PTR [rax]
	mov	edx, DWORD PTR pos[rip]
	movsx	rdx, edx
	sal	rdx, 3
	add	rax, rdx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	DWORD PTR -20[rbp], eax
	sete	al
	movzx	eax, al
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	peek, .-peek
	.type	is_typename, @function
is_typename:
.LFB12:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	rax, QWORD PTR tokens[rip]
	mov	rax, QWORD PTR [rax]
	mov	edx, DWORD PTR pos[rip]
	movsx	rdx, edx
	sal	rdx, 3
	add	rax, rdx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 259
	jne	.L41
	mov	rax, QWORD PTR -8[rbp]
	mov	rax, QWORD PTR 8[rax]
	mov	rdi, rax
	call	find_typedef
	jmp	.L42
.L41:
	mov	rax, QWORD PTR -8[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 280
	je	.L43
	mov	rax, QWORD PTR -8[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 279
	je	.L43
	mov	rax, QWORD PTR -8[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 276
	je	.L43
	mov	rax, QWORD PTR -8[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 278
	je	.L43
	mov	rax, QWORD PTR -8[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 277
	jne	.L44
.L43:
	mov	eax, 1
	jmp	.L42
.L44:
	mov	eax, 0
.L42:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	is_typename, .-is_typename
	.section	.rodata
.LC2:
	.string	"Identifier expected"
	.text
	.type	ident, @function
ident:
.LFB13:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	rax, QWORD PTR tokens[rip]
	mov	rax, QWORD PTR [rax]
	mov	edx, DWORD PTR pos[rip]
	movsx	rdx, edx
	sal	rdx, 3
	add	rax, rdx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 259
	je	.L47
	mov	rax, QWORD PTR -8[rbp]
	mov	rax, QWORD PTR 24[rax]
	lea	rsi, .LC2[rip]
	mov	rdi, rax
	call	error_at@PLT
.L47:
	mov	eax, DWORD PTR pos[rip]
	add	eax, 1
	mov	DWORD PTR pos[rip], eax
	mov	rax, QWORD PTR -8[rbp]
	mov	rax, QWORD PTR 8[rax]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	ident, .-ident
	.section	.rodata
.LC3:
	.string	"Numeric value expected"
	.text
	.type	numeric, @function
numeric:
.LFB14:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	rax, QWORD PTR tokens[rip]
	mov	rax, QWORD PTR [rax]
	mov	edx, DWORD PTR pos[rip]
	movsx	rdx, edx
	sal	rdx, 3
	add	rax, rdx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 256
	je	.L50
	mov	rax, QWORD PTR -8[rbp]
	mov	rax, QWORD PTR 24[rax]
	lea	rsi, .LC3[rip]
	mov	rdi, rax
	call	error_at@PLT
.L50:
	mov	eax, DWORD PTR pos[rip]
	add	eax, 1
	mov	DWORD PTR pos[rip], eax
	mov	rax, QWORD PTR -8[rbp]
	mov	eax, DWORD PTR 4[rax]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	numeric, .-numeric
	.section	.rodata
.LC4:
	.string	"unknown type name"
	.text
	.type	type_specifier, @function
type_specifier:
.LFB15:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 96
	mov	rax, QWORD PTR tokens[rip]
	mov	rcx, QWORD PTR [rax]
	mov	eax, DWORD PTR pos[rip]
	lea	edx, 1[rax]
	mov	DWORD PTR pos[rip], edx
	cdqe
	sal	rax, 3
	add	rax, rcx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR -56[rbp], rax
	mov	rax, QWORD PTR -56[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 278
	je	.L53
	cmp	eax, 278
	jg	.L54
	cmp	eax, 259
	je	.L55
	jmp	.L56
.L54:
	cmp	eax, 279
	je	.L57
	cmp	eax, 280
	je	.L58
	jmp	.L56
.L53:
	mov	eax, 0
	call	void_ty@PLT
	jmp	.L59
.L57:
	mov	eax, 0
	call	char_ty@PLT
	jmp	.L59
.L58:
	mov	eax, 0
	call	int_ty@PLT
	jmp	.L59
.L55:
	mov	rax, QWORD PTR -56[rbp]
	mov	rax, QWORD PTR 8[rax]
	mov	rdi, rax
	call	find_typedef
	mov	QWORD PTR -48[rbp], rax
	cmp	QWORD PTR -48[rbp], 0
	jne	.L60
	mov	rax, QWORD PTR -56[rbp]
	mov	rax, QWORD PTR 24[rax]
	lea	rsi, .LC4[rip]
	mov	rdi, rax
	call	error_at@PLT
.L60:
	mov	rax, QWORD PTR -48[rbp]
	jmp	.L59
.L56:
	mov	rax, QWORD PTR -56[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 277
	jne	.L61
	mov	rax, QWORD PTR tokens[rip]
	mov	rax, QWORD PTR [rax]
	mov	edx, DWORD PTR pos[rip]
	movsx	rdx, edx
	sal	rdx, 3
	add	rax, rdx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR -40[rbp], rax
	mov	QWORD PTR -88[rbp], 0
	mov	QWORD PTR -80[rbp], 0
	mov	rax, QWORD PTR -40[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 259
	jne	.L62
	mov	eax, DWORD PTR pos[rip]
	add	eax, 1
	mov	DWORD PTR pos[rip], eax
	mov	rax, QWORD PTR -40[rbp]
	mov	rax, QWORD PTR 8[rax]
	mov	QWORD PTR -88[rbp], rax
	mov	rax, QWORD PTR -88[rbp]
	mov	rdi, rax
	call	find_tag
	mov	QWORD PTR -80[rbp], rax
.L62:
	cmp	QWORD PTR -80[rbp], 0
	je	.L63
	mov	rax, QWORD PTR -80[rbp]
	jmp	.L59
.L63:
	cmp	QWORD PTR -88[rbp], 0
	je	.L64
	mov	eax, 0
	call	int_ty@PLT
	mov	rdx, rax
	mov	rax, QWORD PTR -40[rbp]
	mov	rcx, QWORD PTR 8[rax]
	mov	rax, QWORD PTR env[rip]
	mov	rax, QWORD PTR 8[rax]
	mov	rsi, rcx
	mov	rdi, rax
	call	map_put@PLT
.L64:
	mov	DWORD PTR -96[rbp], 0
	mov	edi, 123
	call	expect
.L68:
	mov	edi, 125
	call	consume
	test	eax, eax
	jne	.L75
	mov	eax, 0
	call	ident
	mov	QWORD PTR -32[rbp], rax
	mov	edi, 61
	call	consume
	test	eax, eax
	je	.L67
	mov	eax, 0
	call	numeric
	mov	DWORD PTR -96[rbp], eax
.L67:
	mov	eax, DWORD PTR -96[rbp]
	mov	edi, eax
	call	enum_ty@PLT
	mov	QWORD PTR -80[rbp], rax
	mov	rax, QWORD PTR env[rip]
	mov	rax, QWORD PTR 24[rax]
	mov	rdx, QWORD PTR -80[rbp]
	mov	rcx, QWORD PTR -32[rbp]
	mov	rsi, rcx
	mov	rdi, rax
	call	map_put@PLT
	add	DWORD PTR -96[rbp], 1
	mov	edi, 44
	call	consume
	test	eax, eax
	jne	.L68
	mov	edi, 125
	call	consume
	test	eax, eax
	je	.L68
	jmp	.L66
.L75:
	nop
.L66:
	mov	eax, 0
	call	int_ty@PLT
	jmp	.L59
.L61:
	mov	rax, QWORD PTR -56[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 276
	jne	.L69
	mov	rax, QWORD PTR tokens[rip]
	mov	rax, QWORD PTR [rax]
	mov	edx, DWORD PTR pos[rip]
	movsx	rdx, edx
	sal	rdx, 3
	add	rax, rdx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR -24[rbp], rax
	mov	QWORD PTR -72[rbp], 0
	mov	QWORD PTR -64[rbp], 0
	mov	rax, QWORD PTR -24[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 259
	jne	.L70
	mov	eax, DWORD PTR pos[rip]
	add	eax, 1
	mov	DWORD PTR pos[rip], eax
	mov	rax, QWORD PTR -24[rbp]
	mov	rax, QWORD PTR 8[rax]
	mov	QWORD PTR -72[rbp], rax
	mov	rax, QWORD PTR -72[rbp]
	mov	rdi, rax
	call	find_tag
	mov	QWORD PTR -64[rbp], rax
.L70:
	cmp	QWORD PTR -64[rbp], 0
	je	.L71
	mov	rax, QWORD PTR -64[rbp]
	jmp	.L59
.L71:
	mov	esi, 1
	mov	edi, 5
	call	new_ty@PLT
	mov	QWORD PTR -64[rbp], rax
	mov	eax, 0
	call	new_map@PLT
	mov	rdx, rax
	mov	rax, QWORD PTR -64[rbp]
	mov	QWORD PTR 40[rax], rdx
	cmp	QWORD PTR -72[rbp], 0
	je	.L72
	mov	rax, QWORD PTR env[rip]
	mov	rax, QWORD PTR 8[rax]
	mov	rdx, QWORD PTR -64[rbp]
	mov	rcx, QWORD PTR -72[rbp]
	mov	rsi, rcx
	mov	rdi, rax
	call	map_put@PLT
.L72:
	mov	edi, 123
	call	expect
	mov	DWORD PTR -92[rbp], 0
	jmp	.L73
.L74:
	mov	eax, 0
	call	declaration_type
	mov	QWORD PTR -16[rbp], rax
	mov	rax, QWORD PTR -16[rbp]
	mov	rax, QWORD PTR 8[rax]
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -16[rbp]
	mov	rcx, QWORD PTR 40[rax]
	mov	rax, QWORD PTR -64[rbp]
	mov	rax, QWORD PTR 40[rax]
	mov	rdx, QWORD PTR -8[rbp]
	mov	rsi, rcx
	mov	rdi, rax
	call	map_put@PLT
	mov	rax, QWORD PTR -8[rbp]
	mov	edx, DWORD PTR 8[rax]
	mov	eax, DWORD PTR -92[rbp]
	mov	esi, edx
	mov	edi, eax
	call	roundup@PLT
	mov	DWORD PTR -92[rbp], eax
	mov	rax, QWORD PTR -8[rbp]
	mov	edx, DWORD PTR -92[rbp]
	mov	DWORD PTR 48[rax], edx
	mov	rax, QWORD PTR -8[rbp]
	mov	eax, DWORD PTR 4[rax]
	add	DWORD PTR -92[rbp], eax
	mov	rax, QWORD PTR -8[rbp]
	mov	edx, DWORD PTR 8[rax]
	mov	rax, QWORD PTR -64[rbp]
	mov	eax, DWORD PTR 8[rax]
	cmp	edx, eax
	jle	.L73
	mov	rax, QWORD PTR -8[rbp]
	mov	edx, DWORD PTR 8[rax]
	mov	rax, QWORD PTR -64[rbp]
	mov	DWORD PTR 8[rax], edx
.L73:
	mov	edi, 125
	call	consume
	test	eax, eax
	je	.L74
	mov	rax, QWORD PTR -64[rbp]
	mov	edx, DWORD PTR 8[rax]
	mov	eax, DWORD PTR -92[rbp]
	mov	esi, edx
	mov	edi, eax
	call	roundup@PLT
	mov	edx, eax
	mov	rax, QWORD PTR -64[rbp]
	mov	DWORD PTR 4[rax], edx
	mov	rax, QWORD PTR -64[rbp]
	jmp	.L59
.L69:
	mov	eax, 0
.L59:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	type_specifier, .-type_specifier
	.globl	ptr_to
	.type	ptr_to, @function
ptr_to:
.LFB16:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	QWORD PTR -24[rbp], rdi
	mov	esi, 56
	mov	edi, 1
	call	calloc@PLT
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	DWORD PTR [rax], 3
	mov	rax, QWORD PTR -8[rbp]
	mov	DWORD PTR 4[rax], 8
	mov	rax, QWORD PTR -8[rbp]
	mov	DWORD PTR 8[rax], 8
	mov	rax, QWORD PTR -8[rbp]
	mov	rdx, QWORD PTR -24[rbp]
	mov	QWORD PTR 16[rax], rdx
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE16:
	.size	ptr_to, .-ptr_to
	.section	.rodata
.LC5:
	.string	"constant expression expected"
	.text
	.type	const_expr, @function
const_expr:
.LFB17:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	rax, QWORD PTR tokens[rip]
	mov	rax, QWORD PTR [rax]
	mov	edx, DWORD PTR pos[rip]
	movsx	rdx, edx
	sal	rdx, 3
	add	rax, rdx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR -16[rbp], rax
	mov	eax, 0
	call	expr
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 256
	je	.L79
	lea	rdi, .LC5[rip]
	mov	eax, 0
	call	error@PLT
.L79:
	mov	rax, QWORD PTR -8[rbp]
	mov	eax, DWORD PTR 32[rax]
	cdqe
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE17:
	.size	const_expr, .-const_expr
	.type	arr_of, @function
arr_of:
.LFB18:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 48
	mov	QWORD PTR -40[rbp], rdi
	mov	rax, QWORD PTR -40[rbp]
	mov	QWORD PTR -24[rbp], rax
	mov	eax, 0
	call	new_vector@PLT
	mov	QWORD PTR -16[rbp], rax
	jmp	.L82
.L84:
	mov	edi, 93
	call	consume
	test	eax, eax
	je	.L83
	mov	rax, QWORD PTR -16[rbp]
	mov	esi, 0
	mov	rdi, rax
	call	vec_push@PLT
	jmp	.L82
.L83:
	mov	eax, 0
	call	const_expr
	mov	rdx, rax
	mov	rax, QWORD PTR -16[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	vec_push@PLT
	mov	edi, 93
	call	expect
.L82:
	mov	edi, 91
	call	consume
	test	eax, eax
	jne	.L84
	mov	rax, QWORD PTR -16[rbp]
	mov	eax, DWORD PTR 12[rax]
	test	eax, eax
	jne	.L85
	mov	rax, QWORD PTR -24[rbp]
	jmp	.L86
.L85:
	mov	rax, QWORD PTR -16[rbp]
	mov	eax, DWORD PTR 12[rax]
	sub	eax, 1
	mov	DWORD PTR -28[rbp], eax
.L89:
	mov	esi, 56
	mov	edi, 1
	call	calloc@PLT
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	DWORD PTR [rax], 4
	mov	rax, QWORD PTR -16[rbp]
	mov	rax, QWORD PTR [rax]
	mov	edx, DWORD PTR -28[rbp]
	movsx	rdx, edx
	sal	rdx, 3
	add	rax, rdx
	mov	rax, QWORD PTR [rax]
	mov	edx, eax
	mov	rax, QWORD PTR -8[rbp]
	mov	DWORD PTR 32[rax], edx
	mov	rax, QWORD PTR -8[rbp]
	mov	edx, DWORD PTR 32[rax]
	mov	rax, QWORD PTR -24[rbp]
	mov	eax, DWORD PTR 4[rax]
	imul	edx, eax
	mov	rax, QWORD PTR -8[rbp]
	mov	DWORD PTR 4[rax], edx
	mov	rax, QWORD PTR -40[rbp]
	mov	edx, DWORD PTR 8[rax]
	mov	rax, QWORD PTR -8[rbp]
	mov	DWORD PTR 8[rax], edx
	mov	rax, QWORD PTR -8[rbp]
	mov	rdx, QWORD PTR -24[rbp]
	mov	QWORD PTR 24[rax], rdx
	mov	rax, QWORD PTR -8[rbp]
	mov	rdx, QWORD PTR -24[rbp]
	mov	QWORD PTR 16[rax], rdx
	mov	rax, QWORD PTR -8[rbp]
	mov	QWORD PTR -24[rbp], rax
	cmp	DWORD PTR -28[rbp], 0
	je	.L91
	sub	DWORD PTR -28[rbp], 1
	jmp	.L89
.L91:
	nop
	mov	rax, QWORD PTR -24[rbp]
.L86:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE18:
	.size	arr_of, .-arr_of
	.globl	new_node
	.type	new_node, @function
new_node:
.LFB19:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	DWORD PTR -20[rbp], edi
	mov	esi, 176
	mov	edi, 1
	call	calloc@PLT
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	edx, DWORD PTR -20[rbp]
	mov	DWORD PTR [rax], edx
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE19:
	.size	new_node, .-new_node
	.globl	new_node_expr
	.type	new_node_expr, @function
new_node_expr:
.LFB20:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	DWORD PTR -20[rbp], edi
	mov	QWORD PTR -32[rbp], rsi
	mov	eax, DWORD PTR -20[rbp]
	mov	edi, eax
	call	new_node
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rdx, QWORD PTR -32[rbp]
	mov	QWORD PTR 48[rax], rdx
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE20:
	.size	new_node_expr, .-new_node_expr
	.globl	new_node_binop
	.type	new_node_binop, @function
new_node_binop:
.LFB21:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 48
	mov	DWORD PTR -20[rbp], edi
	mov	QWORD PTR -32[rbp], rsi
	mov	QWORD PTR -40[rbp], rdx
	mov	eax, DWORD PTR -20[rbp]
	mov	edi, eax
	call	new_node
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rdx, QWORD PTR -32[rbp]
	mov	QWORD PTR 16[rax], rdx
	mov	rax, QWORD PTR -8[rbp]
	mov	rdx, QWORD PTR -40[rbp]
	mov	QWORD PTR 24[rax], rdx
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE21:
	.size	new_node_binop, .-new_node_binop
	.globl	new_node_num
	.type	new_node_num, @function
new_node_num:
.LFB22:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	DWORD PTR -20[rbp], edi
	mov	edi, 256
	call	new_node
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	edx, DWORD PTR -20[rbp]
	mov	DWORD PTR 32[rax], edx
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE22:
	.size	new_node_num, .-new_node_num
	.globl	new_node_ident
	.type	new_node_ident, @function
new_node_ident:
.LFB23:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	QWORD PTR -24[rbp], rdi
	mov	edi, 258
	call	new_node
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rdx, QWORD PTR -24[rbp]
	mov	QWORD PTR 40[rax], rdx
	mov	rax, QWORD PTR -8[rbp]
	mov	DWORD PTR 56[rax], 1
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE23:
	.size	new_node_ident, .-new_node_ident
	.globl	new_node_varref
	.type	new_node_varref, @function
new_node_varref:
.LFB24:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	QWORD PTR -24[rbp], rdi
	mov	edi, 258
	call	new_node
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -24[rbp]
	mov	rdx, QWORD PTR 16[rax]
	mov	rax, QWORD PTR -8[rbp]
	mov	QWORD PTR 40[rax], rdx
	mov	rax, QWORD PTR -8[rbp]
	mov	rdx, QWORD PTR -24[rbp]
	mov	QWORD PTR 160[rax], rdx
	mov	rax, QWORD PTR -24[rbp]
	mov	rdx, QWORD PTR [rax]
	mov	rax, QWORD PTR -8[rbp]
	mov	QWORD PTR 8[rax], rdx
	mov	rax, QWORD PTR -8[rbp]
	mov	DWORD PTR 56[rax], 1
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE24:
	.size	new_node_varref, .-new_node_varref
	.globl	new_node_vardef
	.type	new_node_vardef, @function
new_node_vardef:
.LFB25:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	QWORD PTR -24[rbp], rdi
	mov	edi, 257
	call	new_node
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -24[rbp]
	mov	rdx, QWORD PTR 16[rax]
	mov	rax, QWORD PTR -8[rbp]
	mov	QWORD PTR 40[rax], rdx
	mov	rax, QWORD PTR -24[rbp]
	mov	rdx, QWORD PTR [rax]
	mov	rax, QWORD PTR -8[rbp]
	mov	QWORD PTR 8[rax], rdx
	mov	rax, QWORD PTR -8[rbp]
	mov	rdx, QWORD PTR -24[rbp]
	mov	QWORD PTR 160[rax], rdx
	mov	rax, QWORD PTR -8[rbp]
	mov	DWORD PTR 56[rax], 1
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE25:
	.size	new_node_vardef, .-new_node_vardef
	.section	.rodata
.LC6:
	.string	"tmp"
	.text
	.globl	new_node_assign_eq
	.type	new_node_assign_eq, @function
new_node_assign_eq:
.LFB26:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	push	rbx
	sub	rsp, 56
	.cfi_offset 3, -24
	mov	DWORD PTR -36[rbp], edi
	mov	QWORD PTR -48[rbp], rsi
	mov	QWORD PTR -56[rbp], rdx
	mov	edi, 269
	call	new_node
	mov	QWORD PTR -32[rbp], rax
	mov	eax, 0
	call	new_vector@PLT
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 144[rax], rdx
	mov	rax, QWORD PTR -48[rbp]
	mov	rax, QWORD PTR 8[rax]
	mov	rdi, rax
	call	ptr_to
	lea	rsi, .LC6[rip]
	mov	rdi, rax
	call	add_lvar
	mov	QWORD PTR -24[rbp], rax
	mov	rax, QWORD PTR -48[rbp]
	mov	rsi, rax
	mov	edi, 272
	call	new_node_expr
	mov	rbx, rax
	mov	rax, QWORD PTR -24[rbp]
	mov	rdi, rax
	call	new_node_varref
	mov	rdx, rbx
	mov	rsi, rax
	mov	edi, 61
	call	new_node_binop
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	rax, QWORD PTR 144[rax]
	mov	rsi, rdx
	mov	rdi, rax
	call	vec_push@PLT
	mov	rax, QWORD PTR -24[rbp]
	mov	rdi, rax
	call	new_node_varref
	mov	rsi, rax
	mov	edi, 273
	call	new_node_expr
	mov	rcx, rax
	mov	rdx, QWORD PTR -56[rbp]
	mov	eax, DWORD PTR -36[rbp]
	mov	rsi, rcx
	mov	edi, eax
	call	new_node_binop
	mov	rbx, rax
	mov	rax, QWORD PTR -24[rbp]
	mov	rdi, rax
	call	new_node_varref
	mov	rsi, rax
	mov	edi, 273
	call	new_node_expr
	mov	rdx, rbx
	mov	rsi, rax
	mov	edi, 61
	call	new_node_binop
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	rax, QWORD PTR 144[rax]
	mov	rsi, rdx
	mov	rdi, rax
	call	vec_push@PLT
	mov	rax, QWORD PTR -48[rbp]
	mov	rdx, QWORD PTR 8[rax]
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 8[rax], rdx
	mov	rax, QWORD PTR -32[rbp]
	add	rsp, 56
	pop	rbx
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE26:
	.size	new_node_assign_eq, .-new_node_assign_eq
	.section	.rodata
.LC7:
	.string	"#inside add_str"
.LC8:
	.string	".LC%d"
	.text
	.globl	add_str
	.type	add_str, @function
add_str:
.LFB27:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	push	rbx
	sub	rsp, 56
	.cfi_offset 3, -24
	mov	QWORD PTR -56[rbp], rdi
	lea	rdi, .LC7[rip]
	call	puts@PLT
	mov	eax, DWORD PTR str_count[rip]
	lea	edx, 1[rax]
	mov	DWORD PTR str_count[rip], edx
	mov	esi, eax
	lea	rdi, .LC8[rip]
	mov	eax, 0
	call	format
	mov	QWORD PTR -40[rbp], rax
	mov	rax, QWORD PTR -56[rbp]
	mov	eax, DWORD PTR 16[rax]
	lea	ebx, 1[rax]
	mov	eax, 0
	call	char_ty@PLT
	mov	esi, ebx
	mov	rdi, rax
	call	arr_ty@PLT
	mov	QWORD PTR -32[rbp], rax
	mov	rax, QWORD PTR -56[rbp]
	mov	rdx, QWORD PTR 8[rax]
	mov	rsi, QWORD PTR -40[rbp]
	mov	rax, QWORD PTR -32[rbp]
	mov	ecx, 0
	mov	rdi, rax
	call	add_gvar
	mov	QWORD PTR -24[rbp], rax
	mov	rax, QWORD PTR -24[rbp]
	mov	DWORD PTR 28[rax], 1
	mov	rax, QWORD PTR -24[rbp]
	add	rsp, 56
	pop	rbx
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE27:
	.size	add_str, .-add_str
	.section	.rodata
.LC9:
	.string	"#before add_str"
	.text
	.globl	string_literal
	.type	string_literal, @function
string_literal:
.LFB28:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	QWORD PTR -24[rbp], rdi
	lea	rdi, .LC9[rip]
	call	puts@PLT
	mov	rax, QWORD PTR -24[rbp]
	mov	rdi, rax
	call	add_str
	mov	QWORD PTR -16[rbp], rax
	mov	rax, QWORD PTR -16[rbp]
	mov	rdi, rax
	call	new_node_varref
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE28:
	.size	string_literal, .-string_literal
	.globl	local_variable
	.type	local_variable, @function
local_variable:
.LFB29:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 48
	mov	QWORD PTR -40[rbp], rdi
	mov	rax, QWORD PTR -40[rbp]
	mov	rax, QWORD PTR 8[rax]
	mov	rdi, rax
	call	find_enum
	mov	QWORD PTR -24[rbp], rax
	cmp	QWORD PTR -24[rbp], 0
	je	.L113
	mov	rax, QWORD PTR -24[rbp]
	mov	eax, DWORD PTR 52[rax]
	mov	edi, eax
	call	new_node_num
	jmp	.L114
.L113:
	mov	rax, QWORD PTR -40[rbp]
	mov	rax, QWORD PTR 8[rax]
	mov	rdi, rax
	call	find_var
	mov	QWORD PTR -16[rbp], rax
	mov	rax, QWORD PTR -16[rbp]
	mov	rdi, rax
	call	new_node_varref
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
.L114:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE29:
	.size	local_variable, .-local_variable
	.globl	function_call
	.type	function_call, @function
function_call:
.LFB30:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	QWORD PTR -24[rbp], rdi
	mov	edi, 270
	call	new_node
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -24[rbp]
	mov	rdx, QWORD PTR 8[rax]
	mov	rax, QWORD PTR -8[rbp]
	mov	QWORD PTR 40[rax], rdx
	mov	eax, 0
	call	new_vector@PLT
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	QWORD PTR 152[rax], rdx
	mov	eax, 0
	call	int_ty@PLT
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	QWORD PTR 8[rax], rdx
	mov	edi, 41
	call	consume
	test	eax, eax
	je	.L116
	mov	rax, QWORD PTR -8[rbp]
	jmp	.L117
.L116:
	mov	eax, 0
	call	assign
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rax, QWORD PTR 152[rax]
	mov	rsi, rdx
	mov	rdi, rax
	call	vec_push@PLT
	mov	edi, 44
	call	consume
	test	eax, eax
	jne	.L116
	mov	edi, 41
	call	expect
	mov	rax, QWORD PTR -8[rbp]
.L117:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE30:
	.size	function_call, .-function_call
	.section	.rodata
.LC10:
	.string	"__builtin_va_end"
	.align 8
.LC11:
	.string	"va_start requires array of struct"
.LC12:
	.string	"gp_offset"
.LC13:
	.string	"fp_offset"
.LC14:
	.string	"overflow_arg_area"
.LC15:
	.string	"reg_save_area"
	.text
	.globl	variadic_function
	.type	variadic_function, @function
variadic_function:
.LFB31:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 80
	mov	QWORD PTR -72[rbp], rdi
	mov	rax, QWORD PTR -72[rbp]
	mov	rax, QWORD PTR 8[rax]
	lea	rsi, .LC10[rip]
	mov	rdi, rax
	call	strcmp@PLT
	test	eax, eax
	jne	.L119
	mov	edi, 259
	call	consume
	mov	edi, 41
	call	consume
	mov	edi, 0
	call	new_node_num
	jmp	.L120
.L119:
	mov	edi, 289
	call	new_node
	mov	QWORD PTR -56[rbp], rax
	mov	eax, 0
	call	new_vector@PLT
	mov	rdx, rax
	mov	rax, QWORD PTR -56[rbp]
	mov	QWORD PTR 144[rax], rdx
	mov	rdx, QWORD PTR cur_fn[rip]
	mov	rax, QWORD PTR -56[rbp]
	mov	QWORD PTR 168[rax], rdx
	mov	eax, 0
	call	ident
	mov	QWORD PTR -48[rbp], rax
	mov	rax, QWORD PTR -48[rbp]
	mov	rdi, rax
	call	find_var
	mov	QWORD PTR -40[rbp], rax
	mov	rax, QWORD PTR -40[rbp]
	mov	rax, QWORD PTR [rax]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 4
	jne	.L121
	mov	rax, QWORD PTR -40[rbp]
	mov	rax, QWORD PTR [rax]
	mov	rax, QWORD PTR 24[rax]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 5
	je	.L122
.L121:
	lea	rdi, .LC11[rip]
	mov	eax, 0
	call	error@PLT
.L122:
	mov	rax, QWORD PTR -40[rbp]
	mov	rdi, rax
	call	new_node_varref
	mov	rsi, rax
	mov	edi, 273
	call	new_node_expr
	mov	rsi, rax
	mov	edi, 285
	call	new_node_expr
	mov	QWORD PTR -32[rbp], rax
	mov	rax, QWORD PTR -32[rbp]
	lea	rdx, .LC12[rip]
	mov	QWORD PTR 40[rax], rdx
	mov	rax, QWORD PTR cur_fn[rip]
	mov	eax, DWORD PTR 32[rax]
	sal	eax, 3
	mov	edi, eax
	call	new_node_num
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	rsi, rax
	mov	edi, 61
	call	new_node_binop
	mov	QWORD PTR -32[rbp], rax
	mov	rax, QWORD PTR -56[rbp]
	mov	rax, QWORD PTR 144[rax]
	mov	rdx, QWORD PTR -32[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	vec_push@PLT
	mov	rax, QWORD PTR -40[rbp]
	mov	rdi, rax
	call	new_node_varref
	mov	rsi, rax
	mov	edi, 273
	call	new_node_expr
	mov	rsi, rax
	mov	edi, 285
	call	new_node_expr
	mov	QWORD PTR -24[rbp], rax
	mov	rax, QWORD PTR -24[rbp]
	lea	rdx, .LC13[rip]
	mov	QWORD PTR 40[rax], rdx
	mov	edi, 48
	call	new_node_num
	mov	rdx, rax
	mov	rax, QWORD PTR -24[rbp]
	mov	rsi, rax
	mov	edi, 61
	call	new_node_binop
	mov	QWORD PTR -24[rbp], rax
	mov	rax, QWORD PTR -56[rbp]
	mov	rax, QWORD PTR 144[rax]
	mov	rdx, QWORD PTR -24[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	vec_push@PLT
	mov	rax, QWORD PTR -40[rbp]
	mov	rdi, rax
	call	new_node_varref
	mov	rsi, rax
	mov	edi, 273
	call	new_node_expr
	mov	rsi, rax
	mov	edi, 285
	call	new_node_expr
	mov	QWORD PTR -16[rbp], rax
	mov	rax, QWORD PTR -16[rbp]
	lea	rdx, .LC14[rip]
	mov	QWORD PTR 40[rax], rdx
	mov	rax, QWORD PTR -16[rbp]
	mov	rsi, rax
	mov	edi, 272
	call	new_node_expr
	mov	QWORD PTR -16[rbp], rax
	mov	rax, QWORD PTR -56[rbp]
	mov	rax, QWORD PTR 144[rax]
	mov	rdx, QWORD PTR -16[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	vec_push@PLT
	mov	rax, QWORD PTR -40[rbp]
	mov	rdi, rax
	call	new_node_varref
	mov	rsi, rax
	mov	edi, 273
	call	new_node_expr
	mov	rsi, rax
	mov	edi, 285
	call	new_node_expr
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	lea	rdx, .LC15[rip]
	mov	QWORD PTR 40[rax], rdx
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 272
	call	new_node_expr
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -56[rbp]
	mov	rax, QWORD PTR 144[rax]
	mov	rdx, QWORD PTR -8[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	vec_push@PLT
	mov	edi, 44
	call	consume
	mov	edi, 259
	call	consume
	mov	edi, 41
	call	expect
	mov	rax, QWORD PTR -56[rbp]
.L120:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE31:
	.size	variadic_function, .-variadic_function
	.section	.rodata
.LC16:
	.string	"#if-then statement"
	.text
	.globl	stmt
	.type	stmt, @function
stmt:
.LFB32:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 48
	mov	edi, 262
	call	consume
	test	eax, eax
	je	.L124
	mov	edi, 260
	call	new_node
	mov	QWORD PTR -32[rbp], rax
	mov	edi, 59
	call	consume
	test	eax, eax
	jne	.L125
	mov	eax, 0
	call	expr
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 48[rax], rdx
	mov	edi, 59
	call	expect
.L125:
	mov	rax, QWORD PTR -32[rbp]
	jmp	.L126
.L124:
	mov	edi, 261
	call	consume
	test	eax, eax
	je	.L127
	mov	eax, 0
	call	type_specifier
	mov	QWORD PTR -40[rbp], rax
	jmp	.L128
.L129:
	mov	rax, QWORD PTR -40[rbp]
	mov	rdi, rax
	call	ptr_to
	mov	QWORD PTR -40[rbp], rax
.L128:
	mov	edi, 42
	call	consume
	test	eax, eax
	jne	.L129
	mov	eax, 0
	call	ident
	mov	QWORD PTR -24[rbp], rax
	mov	rax, QWORD PTR -40[rbp]
	mov	rdi, rax
	call	arr_of
	mov	QWORD PTR -40[rbp], rax
	mov	rax, QWORD PTR env[rip]
	mov	rax, QWORD PTR 16[rax]
	mov	rdx, QWORD PTR -40[rbp]
	mov	rcx, QWORD PTR -24[rbp]
	mov	rsi, rcx
	mov	rdi, rax
	call	map_put@PLT
	mov	edi, 59
	call	expect
	mov	edi, 0
	call	new_node_num
	jmp	.L126
.L127:
	mov	edi, 263
	call	consume
	test	eax, eax
	je	.L130
	mov	edi, 261
	call	new_node
	mov	QWORD PTR -32[rbp], rax
	mov	edi, 40
	call	expect
	mov	eax, 0
	call	expr
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 64[rax], rdx
	mov	edi, 41
	call	expect
	lea	rdi, .LC16[rip]
	call	puts@PLT
	mov	eax, 0
	call	stmt
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 72[rax], rdx
	mov	edi, 264
	call	consume
	test	eax, eax
	je	.L131
	mov	eax, 0
	call	stmt
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 80[rax], rdx
.L131:
	mov	rax, QWORD PTR -32[rbp]
	jmp	.L126
.L130:
	mov	edi, 265
	call	consume
	test	eax, eax
	je	.L132
	mov	edi, 263
	call	new_node
	mov	QWORD PTR -32[rbp], rax
	mov	rax, QWORD PTR breaks[rip]
	mov	rdx, QWORD PTR -32[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	vec_push@PLT
	mov	rax, QWORD PTR continues[rip]
	mov	rdx, QWORD PTR -32[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	vec_push@PLT
	mov	edi, 40
	call	expect
	mov	eax, 0
	call	expr
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 64[rax], rdx
	mov	edi, 41
	call	expect
	mov	eax, 0
	call	stmt
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 104[rax], rdx
	mov	rax, QWORD PTR breaks[rip]
	mov	rdi, rax
	call	vec_pop@PLT
	mov	rax, QWORD PTR continues[rip]
	mov	rdi, rax
	call	vec_pop@PLT
	mov	rax, QWORD PTR -32[rbp]
	jmp	.L126
.L132:
	mov	edi, 266
	call	consume
	test	eax, eax
	je	.L133
	mov	edi, 262
	call	new_node
	mov	QWORD PTR -32[rbp], rax
	mov	rax, QWORD PTR breaks[rip]
	mov	rdx, QWORD PTR -32[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	vec_push@PLT
	mov	rax, QWORD PTR continues[rip]
	mov	rdx, QWORD PTR -32[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	vec_push@PLT
	mov	eax, 0
	call	stmt
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 104[rax], rdx
	mov	edi, 265
	call	expect
	mov	edi, 40
	call	expect
	mov	eax, 0
	call	expr
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 64[rax], rdx
	mov	edi, 41
	call	expect
	mov	edi, 59
	call	expect
	mov	rax, QWORD PTR breaks[rip]
	mov	rdi, rax
	call	vec_pop@PLT
	mov	rax, QWORD PTR continues[rip]
	mov	rdi, rax
	call	vec_pop@PLT
	mov	rax, QWORD PTR -32[rbp]
	jmp	.L126
.L133:
	mov	edi, 267
	call	consume
	test	eax, eax
	je	.L134
	mov	edi, 263
	call	new_node
	mov	QWORD PTR -32[rbp], rax
	mov	rax, QWORD PTR breaks[rip]
	mov	rdx, QWORD PTR -32[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	vec_push@PLT
	mov	rax, QWORD PTR continues[rip]
	mov	rdx, QWORD PTR -32[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	vec_push@PLT
	mov	edi, 40
	call	expect
	mov	eax, 0
	call	is_typename
	test	eax, eax
	je	.L135
	mov	eax, 0
	call	declaration
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 88[rax], rdx
	jmp	.L136
.L135:
	mov	edi, 59
	call	consume
	test	eax, eax
	jne	.L136
	mov	eax, 0
	call	expr
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 88[rax], rdx
	mov	edi, 59
	call	expect
.L136:
	mov	edi, 59
	call	consume
	test	eax, eax
	jne	.L137
	mov	eax, 0
	call	expr
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 64[rax], rdx
	mov	edi, 59
	call	expect
.L137:
	mov	edi, 41
	call	consume
	test	eax, eax
	jne	.L138
	mov	eax, 0
	call	expr
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 96[rax], rdx
	mov	edi, 41
	call	expect
.L138:
	mov	eax, 0
	call	stmt
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 104[rax], rdx
	mov	rax, QWORD PTR breaks[rip]
	mov	rdi, rax
	call	vec_pop@PLT
	mov	rax, QWORD PTR continues[rip]
	mov	rdi, rax
	call	vec_pop@PLT
	mov	rax, QWORD PTR -32[rbp]
	jmp	.L126
.L134:
	mov	edi, 268
	call	consume
	test	eax, eax
	je	.L139
	mov	edi, 264
	call	new_node
	mov	QWORD PTR -32[rbp], rax
	mov	rax, QWORD PTR switches[rip]
	mov	rdx, QWORD PTR -32[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	vec_push@PLT
	mov	rax, QWORD PTR breaks[rip]
	mov	rdx, QWORD PTR -32[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	vec_push@PLT
	mov	eax, 0
	call	new_vector@PLT
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 112[rax], rdx
	mov	edi, 40
	call	expect
	mov	eax, 0
	call	expr
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 64[rax], rdx
	mov	edi, 41
	call	expect
	mov	eax, 0
	call	stmt
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 104[rax], rdx
	mov	rax, QWORD PTR switches[rip]
	mov	rdi, rax
	call	vec_pop@PLT
	mov	rax, QWORD PTR breaks[rip]
	mov	rdi, rax
	call	vec_pop@PLT
	mov	rax, QWORD PTR -32[rbp]
	jmp	.L126
.L139:
	mov	edi, 269
	call	consume
	test	eax, eax
	je	.L140
	mov	edi, 265
	call	new_node
	mov	QWORD PTR -32[rbp], rax
	mov	eax, 0
	call	const_expr
	mov	edx, eax
	mov	rax, QWORD PTR -32[rbp]
	mov	DWORD PTR 32[rax], edx
	mov	edi, 58
	call	expect
	mov	eax, 0
	call	stmt
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 104[rax], rdx
	mov	rax, QWORD PTR switches[rip]
	mov	rdi, rax
	call	vec_top@PLT
	mov	QWORD PTR -16[rbp], rax
	mov	rax, QWORD PTR -16[rbp]
	mov	rax, QWORD PTR 112[rax]
	mov	rdx, QWORD PTR -32[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	vec_push@PLT
	mov	rax, QWORD PTR -32[rbp]
	jmp	.L126
.L140:
	mov	edi, 270
	call	consume
	test	eax, eax
	je	.L141
	mov	edi, 266
	call	new_node
	mov	QWORD PTR -32[rbp], rax
	mov	edi, 58
	call	expect
	mov	eax, 0
	call	stmt
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 104[rax], rdx
	mov	rax, QWORD PTR switches[rip]
	mov	rdi, rax
	call	vec_top@PLT
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rax, QWORD PTR 112[rax]
	mov	rdx, QWORD PTR -32[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	vec_push@PLT
	mov	rax, QWORD PTR -32[rbp]
	jmp	.L126
.L141:
	mov	edi, 123
	call	consume
	test	eax, eax
	je	.L142
	mov	edi, 269
	call	new_node
	mov	QWORD PTR -32[rbp], rax
	mov	eax, 0
	call	new_vector@PLT
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 144[rax], rdx
	mov	rax, QWORD PTR env[rip]
	mov	rdi, rax
	call	new_env
	mov	QWORD PTR env[rip], rax
	jmp	.L143
.L144:
	mov	eax, 0
	call	stmt
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	rax, QWORD PTR 144[rax]
	mov	rsi, rdx
	mov	rdi, rax
	call	vec_push@PLT
.L143:
	mov	edi, 125
	call	consume
	test	eax, eax
	je	.L144
	mov	rax, QWORD PTR env[rip]
	mov	rax, QWORD PTR 32[rax]
	mov	QWORD PTR env[rip], rax
	mov	rax, QWORD PTR -32[rbp]
	jmp	.L126
.L142:
	mov	edi, 271
	call	consume
	test	eax, eax
	je	.L145
	mov	edi, 267
	call	new_node
	mov	QWORD PTR -32[rbp], rax
	mov	rax, QWORD PTR breaks[rip]
	mov	rdi, rax
	call	vec_top@PLT
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 128[rax], rdx
	mov	edi, 59
	call	expect
	mov	rax, QWORD PTR -32[rbp]
	jmp	.L126
.L145:
	mov	edi, 272
	call	consume
	test	eax, eax
	je	.L146
	mov	edi, 268
	call	new_node
	mov	QWORD PTR -32[rbp], rax
	mov	rax, QWORD PTR continues[rip]
	mov	rdi, rax
	call	vec_top@PLT
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 128[rax], rdx
	mov	edi, 59
	call	expect
	mov	rax, QWORD PTR -32[rbp]
	jmp	.L126
.L146:
	mov	eax, 0
	call	is_typename
	test	eax, eax
	je	.L147
	mov	eax, 0
	call	declaration
	mov	QWORD PTR -32[rbp], rax
	mov	rax, QWORD PTR -32[rbp]
	jmp	.L126
.L147:
	mov	edi, 274
	call	consume
	test	eax, eax
	jne	.L148
	mov	edi, 275
	call	consume
	test	eax, eax
	je	.L149
.L148:
	mov	eax, 0
	call	stmt
	jmp	.L126
.L149:
	mov	eax, 0
	call	expr
	mov	QWORD PTR -32[rbp], rax
	mov	edi, 59
	call	expect
	mov	rax, QWORD PTR -32[rbp]
.L126:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE32:
	.size	stmt, .-stmt
	.globl	expr
	.type	expr, @function
expr:
.LFB33:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	eax, 0
	call	assign
	mov	QWORD PTR -8[rbp], rax
	jmp	.L151
.L152:
	mov	eax, 0
	call	assign
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 44
	call	new_node_binop
	mov	QWORD PTR -8[rbp], rax
.L151:
	mov	edi, 44
	call	consume
	test	eax, eax
	jne	.L152
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE33:
	.size	expr, .-expr
	.globl	assign
	.type	assign, @function
assign:
.LFB34:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	eax, 0
	call	ternary
	mov	QWORD PTR -8[rbp], rax
	mov	edi, 61
	call	consume
	test	eax, eax
	je	.L155
	mov	eax, 0
	call	assign
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 61
	call	new_node_binop
	mov	QWORD PTR -8[rbp], rax
	jmp	.L156
.L155:
	mov	edi, 286
	call	consume
	test	eax, eax
	je	.L157
	mov	eax, 0
	call	assign
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 43
	call	new_node_assign_eq
	mov	QWORD PTR -8[rbp], rax
	jmp	.L156
.L157:
	mov	edi, 287
	call	consume
	test	eax, eax
	je	.L158
	mov	eax, 0
	call	assign
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 45
	call	new_node_assign_eq
	mov	QWORD PTR -8[rbp], rax
	jmp	.L156
.L158:
	mov	edi, 288
	call	consume
	test	eax, eax
	je	.L159
	mov	eax, 0
	call	assign
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 42
	call	new_node_assign_eq
	mov	QWORD PTR -8[rbp], rax
	jmp	.L156
.L159:
	mov	edi, 289
	call	consume
	test	eax, eax
	je	.L160
	mov	eax, 0
	call	assign
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 47
	call	new_node_assign_eq
	mov	QWORD PTR -8[rbp], rax
	jmp	.L156
.L160:
	mov	edi, 290
	call	consume
	test	eax, eax
	je	.L156
	mov	eax, 0
	call	assign
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 37
	call	new_node_assign_eq
	mov	QWORD PTR -8[rbp], rax
.L156:
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE34:
	.size	assign, .-assign
	.globl	ternary
	.type	ternary, @function
ternary:
.LFB35:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	eax, 0
	call	log_or
	mov	QWORD PTR -16[rbp], rax
	mov	edi, 63
	call	consume
	test	eax, eax
	je	.L163
	mov	edi, 261
	call	new_node
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rdx, QWORD PTR -16[rbp]
	mov	QWORD PTR 64[rax], rdx
	mov	eax, 0
	call	log_or
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	QWORD PTR 72[rax], rdx
	mov	edi, 58
	call	expect
	mov	eax, 0
	call	log_or
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	QWORD PTR 80[rax], rdx
	mov	rax, QWORD PTR -8[rbp]
	jmp	.L164
.L163:
	mov	rax, QWORD PTR -16[rbp]
.L164:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE35:
	.size	ternary, .-ternary
	.globl	log_or
	.type	log_or, @function
log_or:
.LFB36:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	eax, 0
	call	log_and
	mov	QWORD PTR -8[rbp], rax
.L169:
	mov	edi, 294
	call	consume
	test	eax, eax
	je	.L166
	mov	eax, 0
	call	log_or
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 283
	call	new_node_binop
	mov	QWORD PTR -8[rbp], rax
	jmp	.L169
.L166:
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE36:
	.size	log_or, .-log_or
	.globl	log_and
	.type	log_and, @function
log_and:
.LFB37:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	eax, 0
	call	bit_or
	mov	QWORD PTR -8[rbp], rax
.L176:
	mov	edi, 293
	call	consume
	test	eax, eax
	je	.L173
	mov	eax, 0
	call	log_and
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 282
	call	new_node_binop
	mov	QWORD PTR -8[rbp], rax
	jmp	.L176
.L173:
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE37:
	.size	log_and, .-log_and
	.globl	bit_or
	.type	bit_or, @function
bit_or:
.LFB38:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	eax, 0
	call	bit_xor
	mov	QWORD PTR -8[rbp], rax
.L183:
	mov	edi, 124
	call	consume
	test	eax, eax
	je	.L180
	mov	eax, 0
	call	bit_or
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 124
	call	new_node_binop
	mov	QWORD PTR -8[rbp], rax
	jmp	.L183
.L180:
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE38:
	.size	bit_or, .-bit_or
	.globl	bit_xor
	.type	bit_xor, @function
bit_xor:
.LFB39:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	eax, 0
	call	bit_and
	mov	QWORD PTR -8[rbp], rax
.L190:
	mov	edi, 94
	call	consume
	test	eax, eax
	je	.L187
	mov	eax, 0
	call	bit_xor
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 94
	call	new_node_binop
	mov	QWORD PTR -8[rbp], rax
	jmp	.L190
.L187:
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE39:
	.size	bit_xor, .-bit_xor
	.globl	bit_and
	.type	bit_and, @function
bit_and:
.LFB40:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	eax, 0
	call	equality
	mov	QWORD PTR -8[rbp], rax
.L197:
	mov	edi, 38
	call	consume
	test	eax, eax
	je	.L194
	mov	eax, 0
	call	equality
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 38
	call	new_node_binop
	mov	QWORD PTR -8[rbp], rax
	jmp	.L197
.L194:
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE40:
	.size	bit_and, .-bit_and
	.globl	equality
	.type	equality, @function
equality:
.LFB41:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	eax, 0
	call	relational
	mov	QWORD PTR -8[rbp], rax
.L205:
	mov	edi, 282
	call	consume
	test	eax, eax
	je	.L201
	mov	eax, 0
	call	relational
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 274
	call	new_node_binop
	mov	QWORD PTR -8[rbp], rax
	jmp	.L205
.L201:
	mov	edi, 283
	call	consume
	test	eax, eax
	je	.L203
	mov	eax, 0
	call	relational
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 275
	call	new_node_binop
	mov	QWORD PTR -8[rbp], rax
	jmp	.L205
.L203:
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE41:
	.size	equality, .-equality
	.globl	relational
	.type	relational, @function
relational:
.LFB42:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	eax, 0
	call	shift
	mov	QWORD PTR -8[rbp], rax
.L214:
	mov	edi, 284
	call	consume
	test	eax, eax
	je	.L208
	mov	eax, 0
	call	shift
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 276
	call	new_node_binop
	mov	QWORD PTR -8[rbp], rax
	jmp	.L214
.L208:
	mov	edi, 285
	call	consume
	test	eax, eax
	je	.L210
	mov	eax, 0
	call	shift
	mov	rcx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rdx, rax
	mov	rsi, rcx
	mov	edi, 276
	call	new_node_binop
	mov	QWORD PTR -8[rbp], rax
	jmp	.L214
.L210:
	mov	edi, 60
	call	consume
	test	eax, eax
	je	.L211
	mov	eax, 0
	call	shift
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 60
	call	new_node_binop
	mov	QWORD PTR -8[rbp], rax
	jmp	.L214
.L211:
	mov	edi, 62
	call	consume
	test	eax, eax
	je	.L212
	mov	eax, 0
	call	shift
	mov	rcx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rdx, rax
	mov	rsi, rcx
	mov	edi, 60
	call	new_node_binop
	mov	QWORD PTR -8[rbp], rax
	jmp	.L214
.L212:
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE42:
	.size	relational, .-relational
	.globl	shift
	.type	shift, @function
shift:
.LFB43:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	eax, 0
	call	add
	mov	QWORD PTR -8[rbp], rax
.L221:
	mov	edi, 295
	call	consume
	test	eax, eax
	je	.L217
	mov	eax, 0
	call	add
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 286
	call	new_node_binop
	mov	QWORD PTR -8[rbp], rax
	jmp	.L221
.L217:
	mov	edi, 296
	call	consume
	test	eax, eax
	je	.L219
	mov	eax, 0
	call	add
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 287
	call	new_node_binop
	mov	QWORD PTR -8[rbp], rax
	jmp	.L221
.L219:
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE43:
	.size	shift, .-shift
	.globl	add
	.type	add, @function
add:
.LFB44:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	eax, 0
	call	mul
	mov	QWORD PTR -8[rbp], rax
.L228:
	mov	edi, 43
	call	consume
	test	eax, eax
	je	.L224
	mov	eax, 0
	call	mul
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 43
	call	new_node_binop
	mov	QWORD PTR -8[rbp], rax
	jmp	.L228
.L224:
	mov	edi, 45
	call	consume
	test	eax, eax
	je	.L226
	mov	eax, 0
	call	mul
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 45
	call	new_node_binop
	mov	QWORD PTR -8[rbp], rax
	jmp	.L228
.L226:
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE44:
	.size	add, .-add
	.globl	mul
	.type	mul, @function
mul:
.LFB45:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	eax, 0
	call	unary
	mov	QWORD PTR -8[rbp], rax
.L236:
	mov	edi, 42
	call	consume
	test	eax, eax
	je	.L231
	mov	eax, 0
	call	unary
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 42
	call	new_node_binop
	mov	QWORD PTR -8[rbp], rax
	jmp	.L236
.L231:
	mov	edi, 47
	call	consume
	test	eax, eax
	je	.L233
	mov	eax, 0
	call	unary
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 47
	call	new_node_binop
	mov	QWORD PTR -8[rbp], rax
	jmp	.L236
.L233:
	mov	edi, 37
	call	consume
	test	eax, eax
	je	.L234
	mov	eax, 0
	call	unary
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rax
	mov	edi, 37
	call	new_node_binop
	mov	QWORD PTR -8[rbp], rax
	jmp	.L236
.L234:
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE45:
	.size	mul, .-mul
	.globl	unary
	.type	unary, @function
unary:
.LFB46:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	push	rbx
	sub	rsp, 72
	.cfi_offset 3, -24
	mov	edi, 42
	call	consume
	test	eax, eax
	je	.L239
	mov	edi, 273
	call	new_node
	mov	QWORD PTR -64[rbp], rax
	mov	eax, 0
	call	unary
	mov	rdx, rax
	mov	rax, QWORD PTR -64[rbp]
	mov	QWORD PTR 48[rax], rdx
	mov	rax, QWORD PTR -64[rbp]
	jmp	.L240
.L239:
	mov	edi, 38
	call	consume
	test	eax, eax
	je	.L241
	mov	edi, 272
	call	new_node
	mov	QWORD PTR -56[rbp], rax
	mov	eax, 0
	call	unary
	mov	rdx, rax
	mov	rax, QWORD PTR -56[rbp]
	mov	QWORD PTR 48[rax], rdx
	mov	rax, QWORD PTR -56[rbp]
	jmp	.L240
.L241:
	mov	edi, 33
	call	consume
	test	eax, eax
	je	.L242
	mov	edi, 284
	call	new_node
	mov	QWORD PTR -48[rbp], rax
	mov	eax, 0
	call	unary
	mov	rdx, rax
	mov	rax, QWORD PTR -48[rbp]
	mov	QWORD PTR 48[rax], rdx
	mov	rax, QWORD PTR -48[rbp]
	jmp	.L240
.L242:
	mov	edi, 126
	call	consume
	test	eax, eax
	je	.L243
	mov	edi, 126
	call	new_node
	mov	QWORD PTR -40[rbp], rax
	mov	eax, 0
	call	unary
	mov	rdx, rax
	mov	rax, QWORD PTR -40[rbp]
	mov	QWORD PTR 48[rax], rdx
	mov	rax, QWORD PTR -40[rbp]
	jmp	.L240
.L243:
	mov	edi, 260
	call	consume
	test	eax, eax
	je	.L244
	mov	edi, 259
	call	new_node
	mov	QWORD PTR -32[rbp], rax
	mov	edi, 40
	call	consume
	test	eax, eax
	je	.L245
	mov	eax, 0
	call	is_typename
	test	eax, eax
	je	.L246
	mov	eax, 0
	call	type_specifier
	mov	QWORD PTR -80[rbp], rax
	jmp	.L247
.L248:
	mov	rax, QWORD PTR -80[rbp]
	mov	rdi, rax
	call	ptr_to
	mov	QWORD PTR -80[rbp], rax
.L247:
	mov	edi, 42
	call	consume
	test	eax, eax
	jne	.L248
	mov	edi, 278
	call	new_node
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 48[rax], rdx
	mov	rax, QWORD PTR -32[rbp]
	mov	rax, QWORD PTR 48[rax]
	mov	rdx, QWORD PTR -80[rbp]
	mov	edx, DWORD PTR 4[rdx]
	mov	DWORD PTR 32[rax], edx
	mov	edi, 41
	call	expect
	mov	rax, QWORD PTR -32[rbp]
	jmp	.L240
.L246:
	mov	eax, DWORD PTR pos[rip]
	sub	eax, 1
	mov	DWORD PTR pos[rip], eax
.L245:
	mov	eax, 0
	call	unary
	mov	rdx, rax
	mov	rax, QWORD PTR -32[rbp]
	mov	QWORD PTR 48[rax], rdx
	mov	rax, QWORD PTR -32[rbp]
	jmp	.L240
.L244:
	mov	edi, 40
	call	consume
	test	eax, eax
	je	.L249
	mov	eax, 0
	call	is_typename
	test	eax, eax
	jne	.L250
	mov	eax, DWORD PTR pos[rip]
	sub	eax, 1
	mov	DWORD PTR pos[rip], eax
	mov	eax, 0
	call	postfix
	jmp	.L240
.L250:
	mov	eax, 0
	call	type_specifier
	mov	QWORD PTR -72[rbp], rax
	jmp	.L251
.L252:
	mov	rax, QWORD PTR -72[rbp]
	mov	rdi, rax
	call	ptr_to
	mov	QWORD PTR -72[rbp], rax
.L251:
	mov	edi, 42
	call	consume
	test	eax, eax
	jne	.L252
	mov	edi, 41
	call	expect
	mov	edi, 279
	call	new_node
	mov	QWORD PTR -24[rbp], rax
	mov	rax, QWORD PTR -24[rbp]
	mov	rdx, QWORD PTR -72[rbp]
	mov	QWORD PTR 8[rax], rdx
	mov	eax, 0
	call	unary
	mov	rdx, rax
	mov	rax, QWORD PTR -24[rbp]
	mov	QWORD PTR 48[rax], rdx
	mov	rax, QWORD PTR -24[rbp]
	jmp	.L240
.L249:
	mov	edi, 43
	call	consume
	test	eax, eax
	je	.L253
	mov	eax, 0
	call	postfix
	jmp	.L240
.L253:
	mov	edi, 45
	call	consume
	test	eax, eax
	je	.L254
	mov	eax, 0
	call	postfix
	mov	rbx, rax
	mov	edi, 0
	call	new_node_num
	mov	rdx, rbx
	mov	rsi, rax
	mov	edi, 45
	call	new_node_binop
	jmp	.L240
.L254:
	mov	edi, 291
	call	consume
	test	eax, eax
	je	.L255
	mov	edi, 1
	call	new_node_num
	mov	rbx, rax
	mov	eax, 0
	call	postfix
	mov	rdx, rbx
	mov	rsi, rax
	mov	edi, 43
	call	new_node_assign_eq
	jmp	.L240
.L255:
	mov	edi, 292
	call	consume
	test	eax, eax
	je	.L256
	mov	edi, 1
	call	new_node_num
	mov	rbx, rax
	mov	eax, 0
	call	postfix
	mov	rdx, rbx
	mov	rsi, rax
	mov	edi, 45
	call	new_node_assign_eq
	jmp	.L240
.L256:
	mov	eax, 0
	call	postfix
.L240:
	add	rsp, 72
	pop	rbx
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE46:
	.size	unary, .-unary
	.globl	postfix
	.type	postfix, @function
postfix:
.LFB47:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	eax, 0
	call	term
	mov	QWORD PTR -24[rbp], rax
.L265:
	mov	rax, QWORD PTR tokens[rip]
	mov	rax, QWORD PTR [rax]
	mov	edx, DWORD PTR pos[rip]
	movsx	rdx, edx
	sal	rdx, 3
	add	rax, rdx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR -16[rbp], rax
	mov	edi, 291
	call	consume
	test	eax, eax
	je	.L258
	mov	rax, QWORD PTR -24[rbp]
	mov	rsi, rax
	mov	edi, 280
	call	new_node_expr
	mov	QWORD PTR -24[rbp], rax
	jmp	.L259
.L258:
	mov	edi, 292
	call	consume
	test	eax, eax
	je	.L260
	mov	rax, QWORD PTR -24[rbp]
	mov	rsi, rax
	mov	edi, 281
	call	new_node_expr
	mov	QWORD PTR -24[rbp], rax
	jmp	.L259
.L260:
	mov	edi, 46
	call	consume
	test	eax, eax
	je	.L261
	mov	rax, QWORD PTR -24[rbp]
	mov	rsi, rax
	mov	edi, 285
	call	new_node_expr
	mov	QWORD PTR -24[rbp], rax
	mov	rax, QWORD PTR tokens[rip]
	mov	rcx, QWORD PTR [rax]
	mov	eax, DWORD PTR pos[rip]
	lea	edx, 1[rax]
	mov	DWORD PTR pos[rip], edx
	cdqe
	sal	rax, 3
	add	rax, rcx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR -16[rbp], rax
	mov	rax, QWORD PTR -16[rbp]
	mov	rdx, QWORD PTR 8[rax]
	mov	rax, QWORD PTR -24[rbp]
	mov	QWORD PTR 40[rax], rdx
	jmp	.L259
.L261:
	mov	edi, 281
	call	consume
	test	eax, eax
	je	.L262
	mov	rax, QWORD PTR -24[rbp]
	mov	rsi, rax
	mov	edi, 273
	call	new_node_expr
	mov	rsi, rax
	mov	edi, 285
	call	new_node_expr
	mov	QWORD PTR -24[rbp], rax
	mov	rax, QWORD PTR tokens[rip]
	mov	rcx, QWORD PTR [rax]
	mov	eax, DWORD PTR pos[rip]
	lea	edx, 1[rax]
	mov	DWORD PTR pos[rip], edx
	cdqe
	sal	rax, 3
	add	rax, rcx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR -16[rbp], rax
	mov	rax, QWORD PTR -16[rbp]
	mov	rdx, QWORD PTR 8[rax]
	mov	rax, QWORD PTR -24[rbp]
	mov	QWORD PTR 40[rax], rdx
	jmp	.L259
.L262:
	mov	edi, 91
	call	consume
	test	eax, eax
	je	.L263
	mov	rax, QWORD PTR -24[rbp]
	mov	rax, QWORD PTR 40[rax]
	mov	QWORD PTR -8[rbp], rax
	mov	eax, 0
	call	add
	mov	rdx, rax
	mov	rax, QWORD PTR -24[rbp]
	mov	rsi, rax
	mov	edi, 43
	call	new_node_binop
	mov	rsi, rax
	mov	edi, 273
	call	new_node_expr
	mov	QWORD PTR -24[rbp], rax
	mov	rax, QWORD PTR -24[rbp]
	mov	rdx, QWORD PTR -8[rbp]
	mov	QWORD PTR 40[rax], rdx
	mov	edi, 93
	call	expect
	jmp	.L259
.L263:
	mov	rax, QWORD PTR -24[rbp]
	jmp	.L266
.L259:
	jmp	.L265
.L266:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE47:
	.size	postfix, .-postfix
	.section	.rodata
.LC17:
	.string	"#term here!!"
.LC18:
	.string	"#string here!!"
.LC19:
	.string	"__builtin_va_start"
	.align 8
.LC20:
	.string	"non-number or opening parentheses Token found"
	.text
	.globl	term
	.type	term, @function
term:
.LFB48:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	lea	rdi, .LC17[rip]
	call	puts@PLT
	mov	rax, QWORD PTR tokens[rip]
	mov	rax, QWORD PTR [rax]
	mov	edx, DWORD PTR pos[rip]
	movsx	rdx, edx
	sal	rdx, 3
	add	rax, rdx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR -16[rbp], rax
	mov	rax, QWORD PTR -16[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 40
	jne	.L268
	mov	eax, DWORD PTR pos[rip]
	add	eax, 1
	mov	DWORD PTR pos[rip], eax
	mov	edi, 123
	call	peek
	test	eax, eax
	je	.L269
	mov	eax, 0
	call	stmt
	mov	QWORD PTR -24[rbp], rax
	jmp	.L270
.L269:
	mov	eax, 0
	call	expr
	mov	QWORD PTR -24[rbp], rax
.L270:
	mov	edi, 41
	call	expect
	mov	rax, QWORD PTR -24[rbp]
	jmp	.L271
.L268:
	mov	rax, QWORD PTR -16[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 256
	jne	.L272
	mov	eax, DWORD PTR pos[rip]
	add	eax, 1
	mov	DWORD PTR pos[rip], eax
	mov	rax, QWORD PTR -16[rbp]
	mov	eax, DWORD PTR 4[rax]
	mov	edi, eax
	call	new_node_num
	jmp	.L271
.L272:
	mov	rax, QWORD PTR -16[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 258
	jne	.L273
	mov	eax, DWORD PTR pos[rip]
	add	eax, 1
	mov	DWORD PTR pos[rip], eax
	mov	rax, QWORD PTR -16[rbp]
	mov	eax, DWORD PTR 4[rax]
	mov	edi, eax
	call	new_node_num
	mov	QWORD PTR -8[rbp], rax
	mov	eax, 0
	call	char_ty@PLT
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	QWORD PTR 8[rax], rdx
	mov	rax, QWORD PTR -8[rbp]
	jmp	.L271
.L273:
	mov	rax, QWORD PTR -16[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 257
	jne	.L274
	mov	eax, DWORD PTR pos[rip]
	add	eax, 1
	mov	DWORD PTR pos[rip], eax
	lea	rdi, .LC18[rip]
	call	puts@PLT
	mov	rax, QWORD PTR -16[rbp]
	mov	rdi, rax
	call	string_literal
	jmp	.L271
.L274:
	mov	rax, QWORD PTR -16[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 259
	jne	.L275
	mov	eax, DWORD PTR pos[rip]
	add	eax, 1
	mov	DWORD PTR pos[rip], eax
	mov	edi, 40
	call	consume
	test	eax, eax
	jne	.L276
	mov	rax, QWORD PTR -16[rbp]
	mov	rdi, rax
	call	local_variable
	jmp	.L271
.L276:
	mov	rax, QWORD PTR -16[rbp]
	mov	rax, QWORD PTR 8[rax]
	lea	rsi, .LC19[rip]
	mov	rdi, rax
	call	strcmp@PLT
	test	eax, eax
	je	.L277
	mov	rax, QWORD PTR -16[rbp]
	mov	rax, QWORD PTR 8[rax]
	lea	rsi, .LC10[rip]
	mov	rdi, rax
	call	strcmp@PLT
	test	eax, eax
	jne	.L278
.L277:
	mov	rax, QWORD PTR -16[rbp]
	mov	rdi, rax
	call	variadic_function
	jmp	.L271
.L278:
	mov	rax, QWORD PTR -16[rbp]
	mov	rdi, rax
	call	function_call
	jmp	.L271
.L275:
	mov	rax, QWORD PTR -16[rbp]
	mov	rax, QWORD PTR 24[rax]
	lea	rsi, .LC20[rip]
	mov	rdi, rax
	call	error_at@PLT
	mov	eax, 0
.L271:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE48:
	.size	term, .-term
	.section	.rodata
.LC21:
	.string	"not variable declaration"
	.text
	.globl	declaration
	.type	declaration, @function
declaration:
.LFB49:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	eax, 0
	call	type_specifier
	mov	QWORD PTR -32[rbp], rax
	jmp	.L280
.L281:
	mov	rax, QWORD PTR -32[rbp]
	mov	rdi, rax
	call	ptr_to
	mov	QWORD PTR -32[rbp], rax
.L280:
	mov	edi, 42
	call	consume
	test	eax, eax
	jne	.L281
	mov	rax, QWORD PTR tokens[rip]
	mov	rax, QWORD PTR [rax]
	mov	edx, DWORD PTR pos[rip]
	movsx	rdx, edx
	sal	rdx, 3
	add	rax, rdx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR -24[rbp], rax
	mov	edi, 59
	call	consume
	test	eax, eax
	je	.L282
	mov	edi, 0
	call	new_node_num
	jmp	.L283
.L282:
	mov	edi, 259
	call	consume
	test	eax, eax
	jne	.L284
	mov	rax, QWORD PTR -24[rbp]
	mov	rax, QWORD PTR 24[rax]
	lea	rsi, .LC21[rip]
	mov	rdi, rax
	call	error_at@PLT
.L284:
	mov	rax, QWORD PTR -32[rbp]
	mov	rdi, rax
	call	arr_of
	mov	QWORD PTR -32[rbp], rax
	mov	rax, QWORD PTR -24[rbp]
	mov	rdx, QWORD PTR 8[rax]
	mov	rax, QWORD PTR -32[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	add_lvar
	mov	QWORD PTR -16[rbp], rax
	mov	rax, QWORD PTR -16[rbp]
	mov	rdi, rax
	call	new_node_vardef
	mov	QWORD PTR -8[rbp], rax
	mov	edi, 61
	call	consume
	test	eax, eax
	je	.L285
	mov	eax, 0
	call	expr
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	QWORD PTR 88[rax], rdx
.L285:
	mov	edi, 59
	call	expect
	mov	rax, QWORD PTR -8[rbp]
.L283:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE49:
	.size	declaration, .-declaration
	.globl	declaration_type
	.type	declaration_type, @function
declaration_type:
.LFB50:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	eax, 0
	call	type_specifier
	mov	QWORD PTR -24[rbp], rax
	jmp	.L287
.L288:
	mov	rax, QWORD PTR -24[rbp]
	mov	rdi, rax
	call	ptr_to
	mov	QWORD PTR -24[rbp], rax
.L287:
	mov	edi, 42
	call	consume
	test	eax, eax
	jne	.L288
	mov	rax, QWORD PTR tokens[rip]
	mov	rcx, QWORD PTR [rax]
	mov	eax, DWORD PTR pos[rip]
	lea	edx, 1[rax]
	mov	DWORD PTR pos[rip], edx
	cdqe
	sal	rax, 3
	add	rax, rcx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR -16[rbp], rax
	mov	rax, QWORD PTR -24[rbp]
	mov	rdi, rax
	call	arr_of
	mov	QWORD PTR -24[rbp], rax
	mov	edi, 257
	call	new_node
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rdx, QWORD PTR -24[rbp]
	mov	QWORD PTR 8[rax], rdx
	mov	rax, QWORD PTR -16[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 259
	jne	.L289
	mov	rax, QWORD PTR -16[rbp]
	mov	rdx, QWORD PTR 8[rax]
	mov	rax, QWORD PTR -8[rbp]
	mov	QWORD PTR 40[rax], rdx
.L289:
	mov	edi, 59
	call	expect
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE50:
	.size	declaration_type, .-declaration_type
	.globl	param
	.type	param, @function
param:
.LFB51:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	edi, 297
	call	consume
	test	eax, eax
	je	.L292
	mov	rax, QWORD PTR cur_fn[rip]
	mov	DWORD PTR 36[rax], 1
	mov	edi, 288
	call	new_node
	jmp	.L293
.L292:
	mov	edi, 275
	call	consume
	mov	eax, 0
	call	type_specifier
	mov	QWORD PTR -32[rbp], rax
	jmp	.L294
.L295:
	mov	rax, QWORD PTR -32[rbp]
	mov	rdi, rax
	call	ptr_to
	mov	QWORD PTR -32[rbp], rax
.L294:
	mov	edi, 42
	call	consume
	test	eax, eax
	jne	.L295
	mov	rax, QWORD PTR tokens[rip]
	mov	rax, QWORD PTR [rax]
	mov	edx, DWORD PTR pos[rip]
	movsx	rdx, edx
	sal	rdx, 3
	add	rax, rdx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR -24[rbp], rax
	mov	edi, 259
	call	consume
	test	eax, eax
	jne	.L296
	mov	rax, QWORD PTR -24[rbp]
	mov	rax, QWORD PTR 24[rax]
	lea	rsi, .LC21[rip]
	mov	rdi, rax
	call	error_at@PLT
.L296:
	mov	rax, QWORD PTR -32[rbp]
	mov	rdi, rax
	call	arr_of
	mov	QWORD PTR -32[rbp], rax
	mov	rax, QWORD PTR -32[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 4
	jne	.L297
	mov	rax, QWORD PTR -32[rbp]
	mov	rax, QWORD PTR 24[rax]
	mov	rdi, rax
	call	ptr_to
	mov	QWORD PTR -32[rbp], rax
.L297:
	mov	rax, QWORD PTR -24[rbp]
	mov	rdx, QWORD PTR 8[rax]
	mov	rax, QWORD PTR -32[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	add_lvar
	mov	QWORD PTR -16[rbp], rax
	mov	rax, QWORD PTR -16[rbp]
	mov	rdi, rax
	call	new_node_vardef
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR cur_fn[rip]
	mov	edx, DWORD PTR 32[rax]
	add	edx, 1
	mov	DWORD PTR 32[rax], edx
	mov	rax, QWORD PTR -8[rbp]
.L293:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE51:
	.size	param, .-param
	.globl	params
	.type	params, @function
params:
.LFB52:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	eax, 0
	call	new_vector@PLT
	mov	QWORD PTR -8[rbp], rax
	mov	edi, 40
	call	expect
	mov	edi, 41
	call	consume
	test	eax, eax
	jne	.L299
	mov	eax, 0
	call	param
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	vec_push@PLT
	jmp	.L300
.L301:
	mov	eax, 0
	call	param
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	vec_push@PLT
.L300:
	mov	edi, 44
	call	consume
	test	eax, eax
	jne	.L301
	mov	edi, 41
	call	expect
.L299:
	mov	rax, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE52:
	.size	params, .-params
	.globl	function
	.type	function, @function
function:
.LFB53:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	QWORD PTR -24[rbp], rdi
	mov	QWORD PTR -32[rbp], rsi
	mov	edi, 271
	call	new_node
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	rdx, QWORD PTR -24[rbp]
	mov	QWORD PTR 8[rax], rdx
	mov	rax, QWORD PTR -8[rbp]
	mov	rdx, QWORD PTR -32[rbp]
	mov	QWORD PTR 40[rax], rdx
	mov	esi, 48
	mov	edi, 1
	call	calloc@PLT
	mov	QWORD PTR -16[rbp], rax
	mov	rax, QWORD PTR -16[rbp]
	mov	QWORD PTR cur_fn[rip], rax
	mov	rax, QWORD PTR -16[rbp]
	mov	rdx, QWORD PTR -8[rbp]
	mov	QWORD PTR 16[rax], rdx
	mov	rax, QWORD PTR -16[rbp]
	mov	rdx, QWORD PTR -24[rbp]
	mov	QWORD PTR [rax], rdx
	mov	eax, 0
	call	new_vector@PLT
	mov	rdx, rax
	mov	rax, QWORD PTR -16[rbp]
	mov	QWORD PTR 24[rax], rdx
	mov	rax, QWORD PTR -16[rbp]
	mov	rax, QWORD PTR 24[rax]
	mov	QWORD PTR lvars[rip], rax
	mov	DWORD PTR g_offset[rip], 0
	mov	rax, QWORD PTR env[rip]
	mov	rdi, rax
	call	new_env
	mov	QWORD PTR env[rip], rax
	mov	eax, 0
	call	params
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	QWORD PTR 152[rax], rdx
	mov	edi, 59
	call	consume
	test	eax, eax
	jne	.L304
	mov	eax, 0
	call	stmt
	mov	rdx, rax
	mov	rax, QWORD PTR -8[rbp]
	mov	QWORD PTR 104[rax], rdx
	jmp	.L305
.L304:
	mov	QWORD PTR -16[rbp], 0
.L305:
	mov	rax, QWORD PTR env[rip]
	mov	rax, QWORD PTR 32[rax]
	mov	QWORD PTR env[rip], rax
	mov	rax, QWORD PTR -16[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE53:
	.size	function, .-function
	.globl	gvar_init
	.type	gvar_init, @function
gvar_init:
.LFB54:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 48
	mov	QWORD PTR -40[rbp], rdi
	mov	rax, QWORD PTR -40[rbp]
	mov	DWORD PTR 28[rax], 1
	mov	rax, QWORD PTR -40[rbp]
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR -16[rbp], rax
	mov	rax, QWORD PTR -16[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 4
	je	.L308
	mov	rax, QWORD PTR -16[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 5
	je	.L308
	mov	eax, 0
	call	numeric
	mov	edx, eax
	mov	rax, QWORD PTR -40[rbp]
	mov	DWORD PTR 40[rax], edx
	jmp	.L307
.L308:
	mov	edi, 123
	call	expect
	mov	eax, 0
	call	new_vector@PLT
	mov	rdx, rax
	mov	rax, QWORD PTR -40[rbp]
	mov	QWORD PTR 48[rax], rdx
.L312:
	mov	rax, QWORD PTR tokens[rip]
	mov	rax, QWORD PTR [rax]
	mov	edx, DWORD PTR pos[rip]
	movsx	rdx, edx
	sal	rdx, 3
	add	rax, rdx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 257
	jne	.L310
	mov	eax, DWORD PTR pos[rip]
	add	eax, 1
	mov	DWORD PTR pos[rip], eax
	mov	rax, QWORD PTR -8[rbp]
	mov	rdi, rax
	call	add_str
	mov	QWORD PTR -24[rbp], rax
	jmp	.L311
.L310:
	mov	rax, QWORD PTR -8[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 256
	jne	.L311
	mov	esi, 72
	mov	edi, 1
	call	calloc@PLT
	mov	QWORD PTR -24[rbp], rax
	mov	eax, 0
	call	numeric
	mov	edx, eax
	mov	rax, QWORD PTR -24[rbp]
	mov	DWORD PTR 40[rax], edx
	mov	rax, QWORD PTR -40[rbp]
	mov	rax, QWORD PTR [rax]
	mov	rdx, QWORD PTR 24[rax]
	mov	rax, QWORD PTR -24[rbp]
	mov	QWORD PTR [rax], rdx
.L311:
	mov	rax, QWORD PTR -40[rbp]
	mov	rax, QWORD PTR 48[rax]
	mov	rdx, QWORD PTR -24[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	vec_push@PLT
	mov	edi, 44
	call	consume
	test	eax, eax
	jne	.L312
	mov	edi, 125
	call	expect
.L307:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE54:
	.size	gvar_init, .-gvar_init
	.section	.rodata
.LC22:
	.string	"#%d\n"
.LC23:
	.string	"#%yattaze!!!d\n"
.LC24:
	.string	"type is expected"
.LC25:
	.string	"#this type is %d\n"
	.text
	.globl	toplevel
	.type	toplevel, @function
toplevel:
.LFB55:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 64
	mov	esi, 10
	lea	rdi, .LC22[rip]
	mov	eax, 0
	call	printf@PLT
	jmp	.L314
.L323:
	mov	esi, 10
	lea	rdi, .LC22[rip]
	mov	eax, 0
	call	printf@PLT
	mov	esi, 10
	lea	rdi, .LC23[rip]
	mov	eax, 0
	call	printf@PLT
	mov	edi, 273
	call	consume
	mov	DWORD PTR -56[rbp], eax
	mov	edi, 274
	call	consume
	mov	DWORD PTR -52[rbp], eax
	mov	edi, 275
	call	consume
	mov	DWORD PTR -48[rbp], eax
	mov	edi, 261
	call	consume
	mov	DWORD PTR -44[rbp], eax
	mov	rax, QWORD PTR tokens[rip]
	mov	rax, QWORD PTR [rax]
	mov	edx, DWORD PTR pos[rip]
	movsx	rdx, edx
	sal	rdx, 3
	add	rax, rdx
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR -32[rbp], rax
	mov	rax, QWORD PTR -32[rbp]
	mov	eax, DWORD PTR [rax]
	mov	esi, eax
	lea	rdi, .LC22[rip]
	mov	eax, 0
	call	printf@PLT
	mov	eax, 0
	call	type_specifier
	mov	QWORD PTR -40[rbp], rax
	cmp	QWORD PTR -40[rbp], 0
	jne	.L315
	lea	rdi, .LC24[rip]
	mov	eax, 0
	call	error@PLT
.L315:
	mov	rax, QWORD PTR -40[rbp]
	mov	eax, DWORD PTR [rax]
	mov	esi, eax
	lea	rdi, .LC25[rip]
	mov	eax, 0
	call	printf@PLT
	jmp	.L316
.L317:
	mov	rax, QWORD PTR -40[rbp]
	mov	rdi, rax
	call	ptr_to
	mov	QWORD PTR -40[rbp], rax
.L316:
	mov	edi, 42
	call	consume
	test	eax, eax
	jne	.L317
	mov	edi, 59
	call	consume
	test	eax, eax
	je	.L318
	jmp	.L314
.L318:
	mov	esi, 11
	lea	rdi, .LC22[rip]
	mov	eax, 0
	call	printf@PLT
	mov	eax, 0
	call	ident
	mov	QWORD PTR -24[rbp], rax
	mov	esi, 12
	lea	rdi, .LC22[rip]
	mov	eax, 0
	call	printf@PLT
	mov	edi, 40
	call	peek
	test	eax, eax
	je	.L319
	mov	rdx, QWORD PTR -24[rbp]
	mov	rax, QWORD PTR -40[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	function
	mov	QWORD PTR -16[rbp], rax
	cmp	QWORD PTR -16[rbp], 0
	je	.L314
	mov	rax, QWORD PTR prog[rip]
	mov	rax, QWORD PTR 8[rax]
	mov	rdx, QWORD PTR -16[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	vec_push@PLT
	jmp	.L314
.L319:
	mov	rax, QWORD PTR -40[rbp]
	mov	rdi, rax
	call	arr_of
	mov	QWORD PTR -40[rbp], rax
	cmp	DWORD PTR -44[rbp], 0
	je	.L321
	mov	rax, QWORD PTR env[rip]
	mov	rax, QWORD PTR 16[rax]
	mov	rdx, QWORD PTR -40[rbp]
	mov	rcx, QWORD PTR -24[rbp]
	mov	rsi, rcx
	mov	rdi, rax
	call	map_put@PLT
	mov	edi, 59
	call	expect
	jmp	.L314
.L321:
	mov	edx, DWORD PTR -56[rbp]
	mov	rsi, QWORD PTR -24[rbp]
	mov	rax, QWORD PTR -40[rbp]
	mov	ecx, edx
	mov	edx, 0
	mov	rdi, rax
	call	add_gvar
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	edx, DWORD PTR -52[rbp]
	mov	DWORD PTR 60[rax], edx
	mov	rax, QWORD PTR -8[rbp]
	mov	edx, DWORD PTR -48[rbp]
	mov	DWORD PTR 64[rax], edx
	mov	edi, 61
	call	consume
	test	eax, eax
	je	.L322
	mov	rax, QWORD PTR -8[rbp]
	mov	rdi, rax
	call	gvar_init
.L322:
	mov	edi, 59
	call	expect
.L314:
	mov	edi, 298
	call	peek
	test	eax, eax
	je	.L323
	mov	rax, QWORD PTR prog[rip]
	mov	rax, QWORD PTR 8[rax]
	mov	esi, 0
	mov	rdi, rax
	call	vec_push@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE55:
	.size	toplevel, .-toplevel
	.section	.rodata
.LC26:
	.string	"#%d"
	.text
	.globl	parse
	.type	parse, @function
parse:
.LFB56:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	push	rbx
	sub	rsp, 24
	.cfi_offset 3, -24
	mov	QWORD PTR -24[rbp], rdi
	mov	esi, 10
	lea	rdi, .LC26[rip]
	mov	eax, 0
	call	printf@PLT
	mov	rax, QWORD PTR -24[rbp]
	mov	QWORD PTR tokens[rip], rax
	mov	DWORD PTR pos[rip], 0
	mov	eax, 0
	call	new_vector@PLT
	mov	QWORD PTR switches[rip], rax
	mov	eax, 0
	call	new_vector@PLT
	mov	QWORD PTR breaks[rip], rax
	mov	eax, 0
	call	new_vector@PLT
	mov	QWORD PTR continues[rip], rax
	mov	esi, 16
	mov	edi, 1
	call	calloc@PLT
	mov	QWORD PTR prog[rip], rax
	mov	rbx, QWORD PTR prog[rip]
	mov	eax, 0
	call	new_map@PLT
	mov	QWORD PTR [rbx], rax
	mov	edi, 0
	call	new_env
	mov	QWORD PTR env[rip], rax
	mov	rdx, QWORD PTR prog[rip]
	mov	rax, QWORD PTR env[rip]
	mov	rdx, QWORD PTR [rdx]
	mov	QWORD PTR [rax], rdx
	mov	rbx, QWORD PTR prog[rip]
	mov	eax, 0
	call	new_vector@PLT
	mov	QWORD PTR 8[rbx], rax
	mov	eax, 0
	call	toplevel
	mov	rax, QWORD PTR prog[rip]
	add	rsp, 24
	pop	rbx
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE56:
	.size	parse, .-parse
	.ident	"GCC: (Ubuntu 8.3.0-6ubuntu1) 8.3.0"
	.section	.note.GNU-stack,"",@progbits
