	.file	"assign1-cmpsc311-sp20.c"
	.text
	.globl	round311
	.type	round311, @function
round311:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movss	%xmm0, -4(%rbp)
	movss	-4(%rbp), %xmm0
	pxor	%xmm1, %xmm1
	comiss	%xmm1, %xmm0
	jbe	.L6
	cvtss2sd	-4(%rbp), %xmm1
	movsd	.LC1(%rip), %xmm0
	addsd	%xmm1, %xmm0
	cvttsd2sil	%xmm0, %eax
	jmp	.L4
.L6:
	cvtss2sd	-4(%rbp), %xmm0
	movsd	.LC1(%rip), %xmm1
	subsd	%xmm1, %xmm0
	cvttsd2sil	%xmm0, %eax
.L4:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	round311, .-round311
	.section	.rodata
.LC2:
	.string	"%.2f\n"
.LC3:
	.string	"%.2f, "
	.text
	.globl	printFloatArr311
	.type	printFloatArr311, @function
printFloatArr311:
.LFB1:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L8
.L11:
	movl	-28(%rbp), %eax
	subl	$1, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L9
	movl	-4(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movss	(%rax), %xmm0
	cvtss2sd	%xmm0, %xmm0
	leaq	.LC2(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	jmp	.L7
.L9:
	movl	-4(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movss	(%rax), %xmm0
	cvtss2sd	%xmm0, %xmm0
	leaq	.LC3(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	addl	$1, -4(%rbp)
.L8:
	movl	-4(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jl	.L11
.L7:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	printFloatArr311, .-printFloatArr311
	.section	.rodata
.LC4:
	.string	"%d\n"
.LC5:
	.string	"%d, "
	.text
	.globl	printIntArr311
	.type	printIntArr311, @function
printIntArr311:
.LFB2:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L13
.L16:
	movl	-28(%rbp), %eax
	subl	$1, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L14
	movl	-4(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC4(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L12
.L14:
	movl	-4(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC5(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -4(%rbp)
.L13:
	movl	-4(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jl	.L16
.L12:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	printIntArr311, .-printIntArr311
	.globl	bubbleSort311
	.type	bubbleSort311, @function
bubbleSort311:
.LFB3:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	$0, -12(%rbp)
	jmp	.L18
.L22:
	movl	$0, -8(%rbp)
	jmp	.L19
.L21:
	movl	-8(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %edx
	movl	-8(%rbp), %eax
	cltq
	addq	$1, %rax
	leaq	0(,%rax,4), %rcx
	movq	-24(%rbp), %rax
	addq	%rcx, %rax
	movl	(%rax), %eax
	cmpl	%eax, %edx
	jle	.L20
	movl	-8(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %eax
	movl	%eax, -4(%rbp)
	movl	-8(%rbp), %eax
	cltq
	addq	$1, %rax
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movl	-8(%rbp), %edx
	movslq	%edx, %rdx
	leaq	0(,%rdx,4), %rcx
	movq	-24(%rbp), %rdx
	addq	%rcx, %rdx
	movl	(%rax), %eax
	movl	%eax, (%rdx)
	movl	-8(%rbp), %eax
	cltq
	addq	$1, %rax
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rax, %rdx
	movl	-4(%rbp), %eax
	movl	%eax, (%rdx)
.L20:
	addl	$1, -8(%rbp)
.L19:
	movl	-28(%rbp), %eax
	subl	-12(%rbp), %eax
	cmpl	%eax, -8(%rbp)
	jl	.L21
	addl	$1, -12(%rbp)
.L18:
	movl	-12(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jl	.L22
	nop
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	bubbleSort311, .-bubbleSort311
	.section	.rodata
.LC6:
	.string	"%d"
	.text
	.globl	toBinary
	.type	toBinary, @function
toBinary:
.LFB4:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, %eax
	movb	%al, -20(%rbp)
	movl	$128, -8(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L24
.L26:
	movsbl	-20(%rbp), %eax
	andl	-8(%rbp), %eax
	testl	%eax, %eax
	setg	%al
	movzbl	%al, %eax
	movl	%eax, %esi
	leaq	.LC6(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	cmpl	$3, -4(%rbp)
	jne	.L25
	movl	$32, %edi
	call	putchar@PLT
.L25:
	sarl	-8(%rbp)
	addl	$1, -4(%rbp)
.L24:
	cmpl	$7, -4(%rbp)
	jle	.L26
	movl	$10, %edi
	call	putchar@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	toBinary, .-toBinary
	.globl	countRange311
	.type	countRange311, @function
countRange311:
.LFB5:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$40, %rsp
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	%edx, -32(%rbp)
	movl	%ecx, -36(%rbp)
	movl	$0, -12(%rbp)
	movl	$0, -8(%rbp)
	jmp	.L28
.L30:
	movl	-8(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %eax
	movd	%eax, %xmm0
	call	round311
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	cmpl	-32(%rbp), %eax
	jl	.L29
	movl	-4(%rbp), %eax
	cmpl	-36(%rbp), %eax
	jg	.L29
	addl	$3, -12(%rbp)
.L29:
	addl	$1, -8(%rbp)
.L28:
	movl	-8(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jl	.L30
	movl	-12(%rbp), %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	countRange311, .-countRange311
	.section	.rodata
.LC7:
	.string	"   "
.LC8:
	.string	"%d |"
.LC9:
	.string	"%d  |"
	.align 8
.LC10:
	.string	"+----------------------------------------+"
	.align 8
.LC11:
	.string	"  00  10  20  30  40  50  60  70  80  90 "
	.text
	.globl	histogram311
	.type	histogram311, @function
histogram311:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-32(%rbp), %rax
	subq	$40, %rax
	cmpq	$3, %rax
	jbe	.L33
	movl	$-1, %eax
	jmp	.L34
.L33:
	movl	$0, -8(%rbp)
	jmp	.L35
.L89:
	cmpl	$0, -8(%rbp)
	je	.L36
	cmpl	$20, -8(%rbp)
	jle	.L37
.L36:
	leaq	.LC7(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L38
.L37:
	cmpl	$10, -8(%rbp)
	jg	.L39
	movl	$20, %eax
	subl	-8(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC8(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L38
.L39:
	movl	$20, %eax
	subl	-8(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC9(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
.L38:
	movl	$0, -4(%rbp)
	jmp	.L40
.L86:
	cmpl	$0, -8(%rbp)
	je	.L41
	cmpl	$21, -8(%rbp)
	jne	.L42
.L41:
	leaq	.LC10(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$45, -4(%rbp)
	jmp	.L43
.L42:
	movl	-8(%rbp), %ecx
	movslq	%ecx, %rax
	imulq	$1717986919, %rax, %rax
	shrq	$32, %rax
	movl	%eax, %edx
	sarl	%edx
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, %edx
	sall	$2, %edx
	addl	%eax, %edx
	movl	%ecx, %eax
	subl	%edx, %eax
	testl	%eax, %eax
	jne	.L44
	cmpl	$1, -4(%rbp)
	je	.L45
	cmpl	$2, -4(%rbp)
	jne	.L46
.L45:
	movq	-24(%rbp), %rax
	movl	(%rax), %eax
	movslq	%eax, %rdx
	imulq	$1431655766, %rdx, %rdx
	shrq	$32, %rdx
	sarl	$31, %eax
	subl	%eax, %edx
	movl	$20, %eax
	subl	-8(%rbp), %eax
	cmpl	%eax, %edx
	jl	.L46
	movl	$120, %edi
	call	putchar@PLT
	jmp	.L43
.L46:
	cmpl	$5, -4(%rbp)
	je	.L48
	cmpl	$6, -4(%rbp)
	jne	.L49
.L48:
	movq	-24(%rbp), %rax
	addq	$4, %rax
	movl	(%rax), %eax
	movslq	%eax, %rdx
	imulq	$1431655766, %rdx, %rdx
	shrq	$32, %rdx
	sarl	$31, %eax
	subl	%eax, %edx
	movl	$20, %eax
	subl	-8(%rbp), %eax
	cmpl	%eax, %edx
	jl	.L49
	movl	$120, %edi
	call	putchar@PLT
	jmp	.L43
.L49:
	cmpl	$9, -4(%rbp)
	je	.L50
	cmpl	$10, -4(%rbp)
	jne	.L51
.L50:
	movq	-24(%rbp), %rax
	addq	$8, %rax
	movl	(%rax), %eax
	movslq	%eax, %rdx
	imulq	$1431655766, %rdx, %rdx
	shrq	$32, %rdx
	sarl	$31, %eax
	subl	%eax, %edx
	movl	$20, %eax
	subl	-8(%rbp), %eax
	cmpl	%eax, %edx
	jl	.L51
	movl	$120, %edi
	call	putchar@PLT
	jmp	.L43
.L51:
	cmpl	$13, -4(%rbp)
	je	.L52
	cmpl	$14, -4(%rbp)
	jne	.L53
.L52:
	movq	-24(%rbp), %rax
	addq	$12, %rax
	movl	(%rax), %eax
	movslq	%eax, %rdx
	imulq	$1431655766, %rdx, %rdx
	shrq	$32, %rdx
	sarl	$31, %eax
	subl	%eax, %edx
	movl	$20, %eax
	subl	-8(%rbp), %eax
	cmpl	%eax, %edx
	jl	.L53
	movl	$120, %edi
	call	putchar@PLT
	jmp	.L43
.L53:
	cmpl	$17, -4(%rbp)
	je	.L54
	cmpl	$18, -4(%rbp)
	jne	.L55
.L54:
	movq	-24(%rbp), %rax
	addq	$16, %rax
	movl	(%rax), %eax
	movslq	%eax, %rdx
	imulq	$1431655766, %rdx, %rdx
	shrq	$32, %rdx
	sarl	$31, %eax
	subl	%eax, %edx
	movl	$20, %eax
	subl	-8(%rbp), %eax
	cmpl	%eax, %edx
	jl	.L55
	movl	$120, %edi
	call	putchar@PLT
	jmp	.L43
.L55:
	cmpl	$21, -4(%rbp)
	je	.L56
	cmpl	$22, -4(%rbp)
	jne	.L57
.L56:
	movq	-24(%rbp), %rax
	addq	$20, %rax
	movl	(%rax), %eax
	movslq	%eax, %rdx
	imulq	$1431655766, %rdx, %rdx
	shrq	$32, %rdx
	sarl	$31, %eax
	subl	%eax, %edx
	movl	$20, %eax
	subl	-8(%rbp), %eax
	cmpl	%eax, %edx
	jl	.L57
	movl	$120, %edi
	call	putchar@PLT
	jmp	.L43
.L57:
	cmpl	$25, -4(%rbp)
	je	.L58
	cmpl	$26, -4(%rbp)
	jne	.L59
.L58:
	movq	-24(%rbp), %rax
	addq	$24, %rax
	movl	(%rax), %eax
	movslq	%eax, %rdx
	imulq	$1431655766, %rdx, %rdx
	shrq	$32, %rdx
	sarl	$31, %eax
	subl	%eax, %edx
	movl	$20, %eax
	subl	-8(%rbp), %eax
	cmpl	%eax, %edx
	jl	.L59
	movl	$120, %edi
	call	putchar@PLT
	jmp	.L43
.L59:
	cmpl	$29, -4(%rbp)
	je	.L60
	cmpl	$30, -4(%rbp)
	jne	.L61
.L60:
	movq	-24(%rbp), %rax
	addq	$28, %rax
	movl	(%rax), %eax
	movslq	%eax, %rdx
	imulq	$1431655766, %rdx, %rdx
	shrq	$32, %rdx
	sarl	$31, %eax
	subl	%eax, %edx
	movl	$20, %eax
	subl	-8(%rbp), %eax
	cmpl	%eax, %edx
	jl	.L61
	movl	$120, %edi
	call	putchar@PLT
	jmp	.L43
.L61:
	cmpl	$33, -4(%rbp)
	je	.L62
	cmpl	$34, -4(%rbp)
	jne	.L63
.L62:
	movq	-24(%rbp), %rax
	addq	$32, %rax
	movl	(%rax), %eax
	movslq	%eax, %rdx
	imulq	$1431655766, %rdx, %rdx
	shrq	$32, %rdx
	sarl	$31, %eax
	subl	%eax, %edx
	movl	$20, %eax
	subl	-8(%rbp), %eax
	cmpl	%eax, %edx
	jl	.L63
	movl	$120, %edi
	call	putchar@PLT
	jmp	.L43
.L63:
	cmpl	$37, -4(%rbp)
	je	.L64
	cmpl	$38, -4(%rbp)
	jne	.L65
.L64:
	movq	-24(%rbp), %rax
	addq	$36, %rax
	movl	(%rax), %eax
	movslq	%eax, %rdx
	imulq	$1431655766, %rdx, %rdx
	shrq	$32, %rdx
	sarl	$31, %eax
	subl	%eax, %edx
	movl	$20, %eax
	subl	-8(%rbp), %eax
	cmpl	%eax, %edx
	jl	.L65
	movl	$120, %edi
	call	putchar@PLT
	jmp	.L43
.L65:
	movl	$46, %edi
	call	putchar@PLT
	jmp	.L43
.L44:
	cmpl	$21, -8(%rbp)
	jg	.L43
	cmpl	$1, -4(%rbp)
	je	.L66
	cmpl	$2, -4(%rbp)
	jne	.L67
.L66:
	movq	-24(%rbp), %rax
	movl	(%rax), %eax
	movslq	%eax, %rdx
	imulq	$1431655766, %rdx, %rdx
	shrq	$32, %rdx
	sarl	$31, %eax
	subl	%eax, %edx
	movl	$20, %eax
	subl	-8(%rbp), %eax
	cmpl	%eax, %edx
	jl	.L67
	movl	$120, %edi
	call	putchar@PLT
	jmp	.L43
.L67:
	cmpl	$5, -4(%rbp)
	je	.L68
	cmpl	$6, -4(%rbp)
	jne	.L69
.L68:
	movq	-24(%rbp), %rax
	addq	$4, %rax
	movl	(%rax), %eax
	movslq	%eax, %rdx
	imulq	$1431655766, %rdx, %rdx
	shrq	$32, %rdx
	sarl	$31, %eax
	subl	%eax, %edx
	movl	$20, %eax
	subl	-8(%rbp), %eax
	cmpl	%eax, %edx
	jl	.L69
	movl	$120, %edi
	call	putchar@PLT
	jmp	.L43
.L69:
	cmpl	$9, -4(%rbp)
	je	.L70
	cmpl	$10, -4(%rbp)
	jne	.L71
.L70:
	movq	-24(%rbp), %rax
	addq	$8, %rax
	movl	(%rax), %eax
	movslq	%eax, %rdx
	imulq	$1431655766, %rdx, %rdx
	shrq	$32, %rdx
	sarl	$31, %eax
	subl	%eax, %edx
	movl	$20, %eax
	subl	-8(%rbp), %eax
	cmpl	%eax, %edx
	jl	.L71
	movl	$120, %edi
	call	putchar@PLT
	jmp	.L43
.L71:
	cmpl	$13, -4(%rbp)
	je	.L72
	cmpl	$14, -4(%rbp)
	jne	.L73
.L72:
	movq	-24(%rbp), %rax
	addq	$12, %rax
	movl	(%rax), %eax
	movslq	%eax, %rdx
	imulq	$1431655766, %rdx, %rdx
	shrq	$32, %rdx
	sarl	$31, %eax
	subl	%eax, %edx
	movl	$20, %eax
	subl	-8(%rbp), %eax
	cmpl	%eax, %edx
	jl	.L73
	movl	$120, %edi
	call	putchar@PLT
	jmp	.L43
.L73:
	cmpl	$17, -4(%rbp)
	je	.L74
	cmpl	$18, -4(%rbp)
	jne	.L75
.L74:
	movq	-24(%rbp), %rax
	addq	$16, %rax
	movl	(%rax), %eax
	movslq	%eax, %rdx
	imulq	$1431655766, %rdx, %rdx
	shrq	$32, %rdx
	sarl	$31, %eax
	subl	%eax, %edx
	movl	$20, %eax
	subl	-8(%rbp), %eax
	cmpl	%eax, %edx
	jl	.L75
	movl	$120, %edi
	call	putchar@PLT
	jmp	.L43
.L75:
	cmpl	$21, -4(%rbp)
	je	.L76
	cmpl	$22, -4(%rbp)
	jne	.L77
.L76:
	movq	-24(%rbp), %rax
	addq	$20, %rax
	movl	(%rax), %eax
	movslq	%eax, %rdx
	imulq	$1431655766, %rdx, %rdx
	shrq	$32, %rdx
	sarl	$31, %eax
	subl	%eax, %edx
	movl	$20, %eax
	subl	-8(%rbp), %eax
	cmpl	%eax, %edx
	jl	.L77
	movl	$120, %edi
	call	putchar@PLT
	jmp	.L43
.L77:
	cmpl	$25, -4(%rbp)
	je	.L78
	cmpl	$26, -4(%rbp)
	jne	.L79
.L78:
	movq	-24(%rbp), %rax
	addq	$24, %rax
	movl	(%rax), %eax
	movslq	%eax, %rdx
	imulq	$1431655766, %rdx, %rdx
	shrq	$32, %rdx
	sarl	$31, %eax
	subl	%eax, %edx
	movl	$20, %eax
	subl	-8(%rbp), %eax
	cmpl	%eax, %edx
	jl	.L79
	movl	$120, %edi
	call	putchar@PLT
	jmp	.L43
.L79:
	cmpl	$29, -4(%rbp)
	je	.L80
	cmpl	$30, -4(%rbp)
	jne	.L81
.L80:
	movq	-24(%rbp), %rax
	addq	$28, %rax
	movl	(%rax), %eax
	movslq	%eax, %rdx
	imulq	$1431655766, %rdx, %rdx
	shrq	$32, %rdx
	sarl	$31, %eax
	subl	%eax, %edx
	movl	$20, %eax
	subl	-8(%rbp), %eax
	cmpl	%eax, %edx
	jl	.L81
	movl	$120, %edi
	call	putchar@PLT
	jmp	.L43
.L81:
	cmpl	$33, -4(%rbp)
	je	.L82
	cmpl	$34, -4(%rbp)
	jne	.L83
.L82:
	movq	-24(%rbp), %rax
	addq	$32, %rax
	movl	(%rax), %eax
	movslq	%eax, %rdx
	imulq	$1431655766, %rdx, %rdx
	shrq	$32, %rdx
	sarl	$31, %eax
	subl	%eax, %edx
	movl	$20, %eax
	subl	-8(%rbp), %eax
	cmpl	%eax, %edx
	jl	.L83
	movl	$120, %edi
	call	putchar@PLT
	jmp	.L43
.L83:
	cmpl	$37, -4(%rbp)
	je	.L84
	cmpl	$38, -4(%rbp)
	jne	.L85
.L84:
	movq	-24(%rbp), %rax
	addq	$36, %rax
	movl	(%rax), %eax
	movslq	%eax, %rdx
	imulq	$1431655766, %rdx, %rdx
	shrq	$32, %rdx
	sarl	$31, %eax
	subl	%eax, %edx
	movl	$20, %eax
	subl	-8(%rbp), %eax
	cmpl	%eax, %edx
	jl	.L85
	movl	$120, %edi
	call	putchar@PLT
	jmp	.L43
.L85:
	movl	$32, %edi
	call	putchar@PLT
.L43:
	addl	$1, -4(%rbp)
.L40:
	cmpl	$39, -4(%rbp)
	jle	.L86
	cmpl	$0, -8(%rbp)
	je	.L87
	cmpl	$20, -8(%rbp)
	jg	.L87
	movl	$124, %edi
	call	putchar@PLT
.L87:
	cmpl	$22, -8(%rbp)
	jne	.L88
	leaq	.LC11(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
.L88:
	movl	$10, %edi
	call	putchar@PLT
	addl	$1, -8(%rbp)
.L35:
	cmpl	$22, -8(%rbp)
	jle	.L89
	movl	$0, %eax
.L34:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	histogram311, .-histogram311
	.section	.rodata
	.align 8
.LC12:
	.string	"CMPSC311 - Assignment #1 - Spring 2020\n"
.LC13:
	.string	"%f"
	.align 8
.LC19:
	.string	"Testing printfloatArr311 (floats): "
.LC20:
	.string	"\n"
	.align 8
.LC21:
	.string	"Testing printIntArr311 (integers): "
	.align 8
.LC22:
	.string	"Testing bubbleSort311 (integers): "
.LC23:
	.string	"Testing toBinary:"
	.align 8
.LC24:
	.string	"\n\nCMPSC311 - Assignment #1 - Spring 2020 Complete."
	.text
	.globl	main
	.type	main, @function
main:
.LFB7:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$896, %rsp
	movl	%edi, -884(%rbp)
	movq	%rsi, -896(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	.LC12(%rip), %rdi
	call	puts@PLT
	movl	$0, -868(%rbp)
	jmp	.L91
.L92:
	leaq	-816(%rbp), %rax
	movl	-868(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rdx, %rax
	movq	%rax, %rsi
	leaq	.LC13(%rip), %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
	addl	$1, -868(%rbp)
.L91:
	cmpl	$99, -868(%rbp)
	jle	.L92
	movl	$0, -868(%rbp)
	jmp	.L93
.L100:
	movl	-868(%rbp), %eax
	andl	$1, %eax
	testl	%eax, %eax
	jne	.L94
	movl	-868(%rbp), %eax
	cltq
	movss	-816(%rbp,%rax,4), %xmm0
	comiss	.LC14(%rip), %xmm0
	jbe	.L113
	movl	-868(%rbp), %eax
	cltq
	movss	-816(%rbp,%rax,4), %xmm0
	cvtss2sd	%xmm0, %xmm1
	movsd	.LC15(%rip), %xmm0
	mulsd	%xmm1, %xmm0
	cvtsd2ss	%xmm0, %xmm0
	movl	-868(%rbp), %eax
	cltq
	movss	%xmm0, -816(%rbp,%rax,4)
	jmp	.L97
.L113:
	movl	-868(%rbp), %eax
	cltq
	movss	-816(%rbp,%rax,4), %xmm0
	cvtss2sd	%xmm0, %xmm1
	movsd	.LC16(%rip), %xmm0
	mulsd	%xmm1, %xmm0
	cvtsd2ss	%xmm0, %xmm0
	movl	-868(%rbp), %eax
	cltq
	movss	%xmm0, -816(%rbp,%rax,4)
	jmp	.L97
.L94:
	movl	-868(%rbp), %eax
	cltq
	movss	-816(%rbp,%rax,4), %xmm0
	comiss	.LC14(%rip), %xmm0
	jbe	.L114
	movl	-868(%rbp), %eax
	cltq
	movss	-816(%rbp,%rax,4), %xmm0
	cvtss2sd	%xmm0, %xmm1
	movsd	.LC17(%rip), %xmm0
	mulsd	%xmm1, %xmm0
	cvtsd2ss	%xmm0, %xmm0
	movl	-868(%rbp), %eax
	cltq
	movss	%xmm0, -816(%rbp,%rax,4)
	jmp	.L97
.L114:
	movl	-868(%rbp), %eax
	cltq
	movss	-816(%rbp,%rax,4), %xmm0
	cvtss2sd	%xmm0, %xmm1
	movsd	.LC18(%rip), %xmm0
	mulsd	%xmm1, %xmm0
	cvtsd2ss	%xmm0, %xmm0
	movl	-868(%rbp), %eax
	cltq
	movss	%xmm0, -816(%rbp,%rax,4)
.L97:
	addl	$1, -868(%rbp)
.L93:
	cmpl	$99, -868(%rbp)
	jle	.L100
	movl	$0, -868(%rbp)
	jmp	.L101
.L102:
	movl	-868(%rbp), %eax
	cltq
	movl	-816(%rbp,%rax,4), %eax
	movd	%eax, %xmm0
	call	round311
	movl	-868(%rbp), %edx
	movslq	%edx, %rdx
	movl	%eax, -416(%rbp,%rdx,4)
	addl	$1, -868(%rbp)
.L101:
	cmpl	$99, -868(%rbp)
	jle	.L102
	leaq	.LC19(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-816(%rbp), %rax
	movl	$100, %esi
	movq	%rax, %rdi
	call	printFloatArr311
	leaq	.LC20(%rip), %rdi
	call	puts@PLT
	leaq	.LC21(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-416(%rbp), %rax
	movl	$100, %esi
	movq	%rax, %rdi
	call	printIntArr311
	leaq	.LC20(%rip), %rdi
	call	puts@PLT
	leaq	.LC22(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-416(%rbp), %rax
	movl	$100, %esi
	movq	%rax, %rdi
	call	bubbleSort311
	leaq	-416(%rbp), %rax
	movl	$100, %esi
	movq	%rax, %rdi
	call	printIntArr311
	leaq	.LC20(%rip), %rdi
	call	puts@PLT
	leaq	.LC23(%rip), %rdi
	call	puts@PLT
	movl	$94, -868(%rbp)
	jmp	.L103
.L104:
	movl	-868(%rbp), %eax
	cltq
	movl	-416(%rbp,%rax,4), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	toBinary
	addl	$1, -868(%rbp)
.L103:
	cmpl	$99, -868(%rbp)
	jle	.L104
	leaq	.LC20(%rip), %rdi
	call	puts@PLT
	movl	$0, -868(%rbp)
	jmp	.L105
.L106:
	movl	-868(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	leal	-1(%rax), %ecx
	movl	-868(%rbp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	movl	%eax, %edx
	leaq	-816(%rbp), %rax
	movl	$100, %esi
	movq	%rax, %rdi
	call	countRange311
	movl	-868(%rbp), %edx
	movslq	%edx, %rdx
	movl	%eax, -864(%rbp,%rdx,4)
	addl	$1, -868(%rbp)
.L105:
	cmpl	$9, -868(%rbp)
	jle	.L106
	leaq	-864(%rbp), %rax
	movl	$40, %esi
	movq	%rax, %rdi
	call	histogram311
	cmpl	$-1, %eax
	jne	.L107
	movl	$-1, %eax
	jmp	.L109
.L107:
	leaq	.LC24(%rip), %rdi
	call	puts@PLT
	movl	$0, %eax
.L109:
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L110
	call	__stack_chk_fail@PLT
.L110:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC1:
	.long	0
	.long	1071644672
	.align 4
.LC14:
	.long	1112014848
	.align 8
.LC15:
	.long	2405181686
	.long	1072231874
	.align 8
.LC16:
	.long	171798692
	.long	1072735191
	.align 8
.LC17:
	.long	3607772529
	.long	1072525475
	.align 8
.LC18:
	.long	1717986918
	.long	1072850534
	.ident	"GCC: (Ubuntu 9.2.1-9ubuntu2) 9.2.1 20191008"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
