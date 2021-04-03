	.file	"test.c"
	.text
	.globl	test
	.type	test, @function
test:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	%rcx, -48(%rbp)
	movq	%r8, -56(%rbp)
	movq	%r9, -64(%rbp)
	movq	$0, -8(%rbp)
	movq	-24(%rbp), %rdx
	movq	-32(%rbp), %rax
	addq	%rax, %rdx
	movq	-40(%rbp), %rax
	addq	%rax, %rdx
	movq	-48(%rbp), %rax
	addq	%rax, %rdx
	movq	-56(%rbp), %rax
	addq	%rax, %rdx
	movq	-64(%rbp), %rax
	addq	%rax, %rdx
	movq	16(%rbp), %rax
	addq	%rax, %rdx
	movq	24(%rbp), %rax
	addq	%rax, %rdx
	movq	32(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	test, .-test
	.section	.rodata
.LC0:
	.string	"main=%p test=%p\n"
.LC1:
	.string	"sum = %ld\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movq	$5, -80(%rbp)
	movq	$6, -72(%rbp)
	movq	$7, -64(%rbp)
	movq	$8, -56(%rbp)
	movq	$9, -48(%rbp)
	movq	$10, -40(%rbp)
	movq	$11, -32(%rbp)
	movq	$12, -24(%rbp)
	movq	$13, -16(%rbp)
	leaq	test(%rip), %rdx
	leaq	main(%rip), %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-40(%rbp), %r8
	movq	-48(%rbp), %rdi
	movq	-56(%rbp), %rcx
	movq	-64(%rbp), %rdx
	movq	-72(%rbp), %rsi
	movq	-80(%rbp), %rax
	subq	$8, %rsp
	pushq	-16(%rbp)
	pushq	-24(%rbp)
	pushq	-32(%rbp)
	movq	%r8, %r9
	movq	%rdi, %r8
	movq	%rax, %rdi
	call	test
	addq	$32, %rsp
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (GNU) 10.2.0"
	.section	.note.GNU-stack,"",@progbits
