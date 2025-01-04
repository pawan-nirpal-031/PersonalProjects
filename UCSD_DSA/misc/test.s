	.text
	.file	"test.cpp"
	.section	.text.startup,"ax",@progbits
	.p2align	4, 0x90                         # -- Begin function __cxx_global_var_init
	.type	__cxx_global_var_init,@function
__cxx_global_var_init:                  # @__cxx_global_var_init
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	leaq	_ZStL8__ioinit(%rip), %rdi
	callq	_ZNSt8ios_base4InitC1Ev@PLT
	movq	_ZNSt8ios_base4InitD1Ev@GOTPCREL(%rip), %rdi
	leaq	_ZStL8__ioinit(%rip), %rsi
	leaq	__dso_handle(%rip), %rdx
	callq	__cxa_atexit@PLT
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end0:
	.size	__cxx_global_var_init, .Lfunc_end0-__cxx_global_var_init
	.cfi_endproc
                                        # -- End function
	.text
	.globl	_Z3fooPii                       # -- Begin function _Z3fooPii
	.p2align	4, 0x90
	.type	_Z3fooPii,@function
_Z3fooPii:                              # @_Z3fooPii
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movl	$0, -16(%rbp)
	movl	$0, -20(%rbp)
.LBB1_1:                                # =>This Inner Loop Header: Depth=1
	movl	-20(%rbp), %eax
	cmpl	-12(%rbp), %eax
	jge	.LBB1_4
# %bb.2:                                #   in Loop: Header=BB1_1 Depth=1
	movq	-8(%rbp), %rax
	movslq	-20(%rbp), %rcx
	movl	(%rax,%rcx,4), %eax
	addl	-16(%rbp), %eax
	movl	%eax, -16(%rbp)
# %bb.3:                                #   in Loop: Header=BB1_1 Depth=1
	movl	-20(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -20(%rbp)
	jmp	.LBB1_1
.LBB1_4:
	movl	-16(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end1:
	.size	_Z3fooPii, .Lfunc_end1-_Z3fooPii
	.cfi_endproc
                                        # -- End function
	.globl	_Z4purri                        # -- Begin function _Z4purri
	.p2align	4, 0x90
	.type	_Z4purri,@function
_Z4purri:                               # @_Z4purri
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movl	%edi, -4(%rbp)
	movl	$0, -8(%rbp)
	movl	$0, -12(%rbp)
.LBB2_1:                                # =>This Inner Loop Header: Depth=1
	movl	-12(%rbp), %eax
	cmpl	-4(%rbp), %eax
	jge	.LBB2_4
# %bb.2:                                #   in Loop: Header=BB2_1 Depth=1
	movl	-12(%rbp), %eax
	addl	-8(%rbp), %eax
	movl	%eax, -8(%rbp)
# %bb.3:                                #   in Loop: Header=BB2_1 Depth=1
	movl	-12(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -12(%rbp)
	jmp	.LBB2_1
.LBB2_4:
	movl	-8(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end2:
	.size	_Z4purri, .Lfunc_end2-_Z4purri
	.cfi_endproc
                                        # -- End function
	.globl	_Z3boov                         # -- Begin function _Z3boov
	.p2align	4, 0x90
	.type	_Z3boov,@function
_Z3boov:                                # @_Z3boov
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movl	$0, -4(%rbp)
	movl	$0, -8(%rbp)
.LBB3_1:                                # =>This Inner Loop Header: Depth=1
	cmpl	$100, -8(%rbp)
	jge	.LBB3_4
# %bb.2:                                #   in Loop: Header=BB3_1 Depth=1
	movl	-8(%rbp), %eax
	addl	-4(%rbp), %eax
	movl	%eax, -4(%rbp)
# %bb.3:                                #   in Loop: Header=BB3_1 Depth=1
	movl	-8(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -8(%rbp)
	jmp	.LBB3_1
.LBB3_4:
	movl	-4(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end3:
	.size	_Z3boov, .Lfunc_end3-_Z3boov
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	$0, -4(%rbp)
	callq	_Z3boov
	movl	%eax, -8(%rbp)
	movl	-8(%rbp), %eax
	addl	$100, %eax
	movl	%eax, -8(%rbp)
	cmpl	$100, -8(%rbp)
	jle	.LBB4_8
# %bb.1:
	movl	$0, -12(%rbp)
	movslq	-8(%rbp), %rax
	movl	$4, %ecx
	mulq	%rcx
	movq	%rax, %rdi
	seto	%al
	movq	$-1, %rax
	cmovoq	%rax, %rdi
	callq	_Znam@PLT
	movq	%rax, -24(%rbp)
	movl	$0, -28(%rbp)
.LBB4_2:                                # =>This Inner Loop Header: Depth=1
	movl	-28(%rbp), %eax
	cmpl	-8(%rbp), %eax
	jge	.LBB4_5
# %bb.3:                                #   in Loop: Header=BB4_2 Depth=1
	movl	-28(%rbp), %edi
	imull	-8(%rbp), %edi
	callq	_Z4purri
	addl	-12(%rbp), %eax
	movl	%eax, -12(%rbp)
	movl	-12(%rbp), %edx
	movq	-24(%rbp), %rax
	movslq	-28(%rbp), %rcx
	movl	%edx, (%rax,%rcx,4)
# %bb.4:                                #   in Loop: Header=BB4_2 Depth=1
	movl	-28(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -28(%rbp)
	jmp	.LBB4_2
.LBB4_5:
	movq	-24(%rbp), %rdi
	movl	-32(%rbp), %esi
	callq	_Z3fooPii
	movl	%eax, -32(%rbp)
	cmpl	$100, -32(%rbp)
	jge	.LBB4_7
# %bb.6:
	movl	$1, -4(%rbp)
	jmp	.LBB4_9
.LBB4_7:
	jmp	.LBB4_8
.LBB4_8:
	movl	$0, -4(%rbp)
.LBB4_9:
	movl	-4(%rbp), %eax
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end4:
	.size	main, .Lfunc_end4-main
	.cfi_endproc
                                        # -- End function
	.section	.text.startup,"ax",@progbits
	.p2align	4, 0x90                         # -- Begin function _GLOBAL__sub_I_test.cpp
	.type	_GLOBAL__sub_I_test.cpp,@function
_GLOBAL__sub_I_test.cpp:                # @_GLOBAL__sub_I_test.cpp
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	callq	__cxx_global_var_init
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end5:
	.size	_GLOBAL__sub_I_test.cpp, .Lfunc_end5-_GLOBAL__sub_I_test.cpp
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
	.addrsig_sym __cxx_global_var_init
	.addrsig_sym __cxa_atexit
	.addrsig_sym _Z3fooPii
	.addrsig_sym _Z4purri
	.addrsig_sym _Z3boov
	.addrsig_sym _Znam
	.addrsig_sym _GLOBAL__sub_I_test.cpp
	.addrsig_sym _ZStL8__ioinit
	.addrsig_sym __dso_handle
