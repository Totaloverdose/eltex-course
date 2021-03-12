	.file	"float.c"
	.text
	.section	.rodata
.LC1:
	.string	"%f\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movss	.LC0(%rip), %xmm0
	movss	%xmm0, -12(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L2
.L3:
	pxor	%xmm2, %xmm2
	cvtss2sd	-12(%rbp), %xmm2
	movq	%xmm2, %rax
	movq	%rax, %xmm0
	leaq	.LC1(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	movss	-12(%rbp), %xmm1
	movss	.LC0(%rip), %xmm0
	addss	%xmm1, %xmm0
	movss	%xmm0, -12(%rbp)
	addq	$1, -8(%rbp)
.L2:
	movabsq	$9223372036854775807, %rax
	cmpq	%rax, -8(%rbp)
	jne	.L3
	movl	$0, %edi
	call	exit@PLT
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.section	.rodata
	.align 4
.LC0:
	.long	1065353216
	.ident	"GCC: (GNU) 10.2.0"
	.section	.note.GNU-stack,"",@progbits
