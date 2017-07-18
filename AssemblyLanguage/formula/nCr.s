.globl nCr
	.type	nCr, @function
nCr:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$20, %esp
	movl	%ebx, -12(%ebp)
	movl	%esi, -8(%ebp)
	movl	%edi, -4(%ebp)
	movl	8(%ebp), %ebx
	movl	12(%ebp), %esi
	movl	$0, %eax
	cmpl	$12, %ebx
	jg	.L3
	movl	%ebx, (%esp)
	call	Factorial
	movl	%eax, -16(%ebp)
	movl	%esi, (%esp)
	call	Factorial
	movl	%eax, %edi
	subl	%esi, %ebx
	movl	%ebx, (%esp)
	call	Factorial
	imull	%eax, %edi
	movl	-16(%ebp), %edx
	movl	%edx, %eax
	sarl	$31, %edx
	idivl	%edi
.L3:
	movl	-12(%ebp), %ebx
	movl	-8(%ebp), %esi
	movl	-4(%ebp), %edi
	movl	%ebp, %esp
	popl	%ebp
	ret
	
	
.globl Factorial
	.type	Factorial, @function
Factorial:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %ecx
	movl	$0, %eax
	cmpl	$12, %ecx
	jg	.L1
	movl	%ecx, %edx
	movb	$1, %al
	cmpl	$1, %ecx
	jle	.L1
.L2:
	imull	%edx, %eax
	subl	$1, %edx
	cmpl	$1, %edx
	jne	.L2
.L1:
	popl	%ebp
	ret

