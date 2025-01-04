	.text
	.file	"test.cpp"
	.globl	_Z3fooPii                       # -- Begin function _Z3fooPii
	.p2align	4, 0x90
	.type	_Z3fooPii,@function
_Z3fooPii:                              # @_Z3fooPii
	.cfi_startproc
# %bb.0:
	testl	%esi, %esi
	jle	.LBB0_1
# %bb.2:
	movl	%esi, %ecx
	cmpl	$8, %esi
	jae	.LBB0_4
# %bb.3:
	xorl	%edx, %edx
	xorl	%eax, %eax
	jmp	.LBB0_12
.LBB0_1:
	xorl	%eax, %eax
	retq
.LBB0_4:
	movl	%ecx, %edx
	andl	$-8, %edx
	leaq	-8(%rdx), %rsi
	movq	%rsi, %rax
	shrq	$3, %rax
	addq	$1, %rax
	movl	%eax, %r8d
	andl	$3, %r8d
	cmpq	$24, %rsi
	jae	.LBB0_6
# %bb.5:
	pxor	%xmm0, %xmm0
	xorl	%esi, %esi
	pxor	%xmm1, %xmm1
	jmp	.LBB0_8
.LBB0_6:
	andq	$-4, %rax
	pxor	%xmm0, %xmm0
	xorl	%esi, %esi
	pxor	%xmm1, %xmm1
	.p2align	4, 0x90
.LBB0_7:                                # =>This Inner Loop Header: Depth=1
	movdqu	(%rdi,%rsi,4), %xmm2
	paddd	%xmm0, %xmm2
	movdqu	16(%rdi,%rsi,4), %xmm0
	paddd	%xmm1, %xmm0
	movdqu	32(%rdi,%rsi,4), %xmm1
	movdqu	48(%rdi,%rsi,4), %xmm3
	movdqu	64(%rdi,%rsi,4), %xmm4
	paddd	%xmm1, %xmm4
	paddd	%xmm2, %xmm4
	movdqu	80(%rdi,%rsi,4), %xmm2
	paddd	%xmm3, %xmm2
	paddd	%xmm0, %xmm2
	movdqu	96(%rdi,%rsi,4), %xmm0
	paddd	%xmm4, %xmm0
	movdqu	112(%rdi,%rsi,4), %xmm1
	paddd	%xmm2, %xmm1
	addq	$32, %rsi
	addq	$-4, %rax
	jne	.LBB0_7
.LBB0_8:
	testq	%r8, %r8
	je	.LBB0_11
# %bb.9:
	leaq	(%rdi,%rsi,4), %rax
	addq	$16, %rax
	shlq	$5, %r8
	xorl	%esi, %esi
	.p2align	4, 0x90
.LBB0_10:                               # =>This Inner Loop Header: Depth=1
	movdqu	-16(%rax,%rsi), %xmm2
	paddd	%xmm2, %xmm0
	movdqu	(%rax,%rsi), %xmm2
	paddd	%xmm2, %xmm1
	addq	$32, %rsi
	cmpq	%rsi, %r8
	jne	.LBB0_10
.LBB0_11:
	paddd	%xmm1, %xmm0
	pshufd	$238, %xmm0, %xmm1              # xmm1 = xmm0[2,3,2,3]
	paddd	%xmm0, %xmm1
	pshufd	$85, %xmm1, %xmm0               # xmm0 = xmm1[1,1,1,1]
	paddd	%xmm1, %xmm0
	movd	%xmm0, %eax
	cmpq	%rcx, %rdx
	je	.LBB0_13
	.p2align	4, 0x90
.LBB0_12:                               # =>This Inner Loop Header: Depth=1
	addl	(%rdi,%rdx,4), %eax
	addq	$1, %rdx
	cmpq	%rdx, %rcx
	jne	.LBB0_12
.LBB0_13:
	retq
.Lfunc_end0:
	.size	_Z3fooPii, .Lfunc_end0-_Z3fooPii
	.cfi_endproc
                                        # -- End function
	.globl	_Z4purri                        # -- Begin function _Z4purri
	.p2align	4, 0x90
	.type	_Z4purri,@function
_Z4purri:                               # @_Z4purri
	.cfi_startproc
# %bb.0:
                                        # kill: def $edi killed $edi def $rdi
	testl	%edi, %edi
	jle	.LBB1_1
# %bb.2:
	leal	-1(%rdi), %eax
	leal	-2(%rdi), %ecx
	imulq	%rax, %rcx
	shrq	%rcx
	leal	(%rcx,%rdi), %eax
	addl	$-1, %eax
	retq
.LBB1_1:
	xorl	%eax, %eax
	retq
.Lfunc_end1:
	.size	_Z4purri, .Lfunc_end1-_Z4purri
	.cfi_endproc
                                        # -- End function
	.globl	_Z3boov                         # -- Begin function _Z3boov
	.p2align	4, 0x90
	.type	_Z3boov,@function
_Z3boov:                                # @_Z3boov
	.cfi_startproc
# %bb.0:
	movl	$4950, %eax                     # imm = 0x1356
	retq
.Lfunc_end2:
	.size	_Z3boov, .Lfunc_end2-_Z3boov
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	movl	$1, %eax
	retq
.Lfunc_end3:
	.size	main, .Lfunc_end3-main
	.cfi_endproc
                                        # -- End function
	.section	.text.startup,"ax",@progbits
	.p2align	4, 0x90                         # -- Begin function _GLOBAL__sub_I_test.cpp
	.type	_GLOBAL__sub_I_test.cpp,@function
_GLOBAL__sub_I_test.cpp:                # @_GLOBAL__sub_I_test.cpp
	.cfi_startproc
# %bb.0:
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset %rbx, -16
	leaq	_ZStL8__ioinit(%rip), %rbx
	movq	%rbx, %rdi
	callq	_ZNSt8ios_base4InitC1Ev@PLT
	movq	_ZNSt8ios_base4InitD1Ev@GOTPCREL(%rip), %rdi
	leaq	__dso_handle(%rip), %rdx
	movq	%rbx, %rsi
	popq	%rbx
	.cfi_def_cfa_offset 8
	jmp	__cxa_atexit@PLT                # TAILCALL
.Lfunc_end4:
	.size	_GLOBAL__sub_I_test.cpp, .Lfunc_end4-_GLOBAL__sub_I_test.cpp
	.cfi_endproc
                                        # -- End function
	.type	_ZStL8__ioinit,@object          # @_ZStL8__ioinit
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.hidden	__dso_handle
	.section	.init_array,"aw",@init_array
	.p2align	3
	.quad	_GLOBAL__sub_I_test.cpp
	.ident	"Ubuntu clang version 14.0.0-1ubuntu1.1"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym _GLOBAL__sub_I_test.cpp
	.addrsig_sym _ZStL8__ioinit
	.addrsig_sym __dso_handle
