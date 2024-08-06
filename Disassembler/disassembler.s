	.file	"disassembler.c"
	.text
	.section	.rodata
.LC0:
	.string	"Failed opening %s, aborting\n"
.LC1:
	.string	"Line %i: %02hhx%02hhx  "
.LC2:
	.string	"CLEAR_DISPLAY"
.LC3:
	.string	"RETURN"
.LC4:
	.string	"GOTO %03x"
.LC5:
	.string	"CALL %03x"
.LC6:
	.string	"SKIP NEXT OP IF V%x == %x"
.LC7:
	.string	"SKIP NEXT OP IF V%x != %x"
.LC8:
	.string	"SKIP NEXT OP IF V%x == V%x"
.LC9:
	.string	"ASSIGN V%x = %02x"
.LC10:
	.string	"ASSIGN V%x += %02x"
.LC11:
	.string	"ASSIGN V%x = V%x"
.LC12:
	.string	"ASSIGN V%x |= V%x"
.LC13:
	.string	"ASSIGN V%x &= V%x"
.LC14:
	.string	"ASSIGN V%x ^= V%x"
	.align 8
.LC15:
	.string	"ASSIGN V%x += V%x, possibly set carry flag"
.LC16:
	.string	"ASSIGN V%x -= V%x"
.LC17:
	.string	"SHIFT V%x right one bit"
.LC18:
	.string	"ASSIGN V%x = V%x - V%x"
.LC19:
	.string	"SHIFT V%x left one bit"
.LC20:
	.string	"SKIP NEXT OP IF V%x != V%x"
.LC21:
	.string	"SET I = %02x"
.LC22:
	.string	"GO TO V0 + %03x"
.LC23:
	.string	"SET V%x = RANDOM & %02x"
	.align 8
.LC24:
	.string	"DRAW I at (V%x, V%x) with height %x"
	.align 8
.LC25:
	.string	"SKIP NEXT OP IF V%x is pressed"
	.align 8
.LC26:
	.string	"SKIP NEXT OP IF V%x is not pressed"
.LC27:
	.string	"ASSIGn V%x = delay_timer"
.LC28:
	.string	"ASSIGN V%x = get_key"
.LC29:
	.string	"ASSIGN delay_timer = V%x"
.LC30:
	.string	"ASSIGN sound_timer = V%x"
.LC31:
	.string	"ASSIGN I += V%x"
	.align 8
.LC32:
	.string	"ASSIGN I = sprite_address(V%x)"
	.align 8
.LC33:
	.string	"CONVERT V%x to decimal, store in I"
	.align 8
.LC34:
	.string	"DUMP registers from V0 to V%x starting at I"
	.align 8
.LC35:
	.string	"LOAD memory starting at I into registers V0 to V%x"
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
	subq	$48, %rsp
	movl	%edi, -36(%rbp)
	movq	%rsi, -48(%rbp)
	movq	-48(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	open@PLT
	movl	%eax, -12(%rbp)
	cmpl	$-1, -12(%rbp)
	jne	.L2
	movq	-48(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$1, %eax
	jmp	.L3
.L2:
	movl	$290, %edi
	call	malloc@PLT
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rcx
	movl	-12(%rbp), %eax
	movl	$290, %edx
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	read@PLT
	movl	$0, -16(%rbp)
	jmp	.L4
.L50:
	movl	-16(%rbp), %eax
	cltq
	leaq	1(%rax), %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %ecx
	movl	-16(%rbp), %eax
	movslq	%eax, %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %edx
	movl	-16(%rbp), %eax
	movl	%eax, %esi
	shrl	$31, %esi
	addl	%esi, %eax
	sarl	%eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-16(%rbp), %eax
	movslq	%eax, %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	sarb	$4, %al
	movb	%al, -24(%rbp)
	andb	$15, -24(%rbp)
	movl	-16(%rbp), %eax
	movslq	%eax, %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cbtw
	andl	$15, %eax
	movw	%ax, -20(%rbp)
	movl	-16(%rbp), %eax
	cltq
	leaq	1(%rax), %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	shrb	$4, %al
	movb	%al, -23(%rbp)
	movl	-16(%rbp), %eax
	cltq
	leaq	1(%rax), %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	andl	$15, %eax
	movb	%al, -22(%rbp)
	movl	-16(%rbp), %eax
	cltq
	leaq	1(%rax), %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movb	%al, -21(%rbp)
	movl	-16(%rbp), %eax
	cltq
	leaq	1(%rax), %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cbtw
	movzbl	%al, %edx
	movzwl	-20(%rbp), %eax
	sall	$8, %eax
	orl	%edx, %eax
	movw	%ax, -18(%rbp)
	movzbl	-24(%rbp), %eax
	cmpl	$15, %eax
	ja	.L5
	movl	%eax, %eax
	leaq	0(,%rax,4), %rdx
	leaq	.L7(%rip), %rax
	movl	(%rdx,%rax), %eax
	cltq
	leaq	.L7(%rip), %rdx
	addq	%rdx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L7:
	.long	.L22-.L7
	.long	.L21-.L7
	.long	.L20-.L7
	.long	.L19-.L7
	.long	.L18-.L7
	.long	.L17-.L7
	.long	.L16-.L7
	.long	.L15-.L7
	.long	.L14-.L7
	.long	.L13-.L7
	.long	.L12-.L7
	.long	.L11-.L7
	.long	.L10-.L7
	.long	.L9-.L7
	.long	.L8-.L7
	.long	.L6-.L7
	.text
.L22:
	cmpb	$-32, -21(%rbp)
	jne	.L23
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L51
.L23:
	cmpb	$-18, -21(%rbp)
	jne	.L51
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L51
.L21:
	movzwl	-18(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC4(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L5
.L20:
	movzwl	-18(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC5(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L5
.L19:
	movzbl	-21(%rbp), %edx
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC6(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L5
.L18:
	movzbl	-21(%rbp), %edx
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC7(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L5
.L17:
	movzbl	-23(%rbp), %edx
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC8(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L5
.L16:
	movzbl	-21(%rbp), %edx
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC9(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L5
.L15:
	movzbl	-21(%rbp), %edx
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC10(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L5
.L14:
	movzbl	-22(%rbp), %eax
	cmpl	$14, %eax
	ja	.L52
	movl	%eax, %eax
	leaq	0(,%rax,4), %rdx
	leaq	.L27(%rip), %rax
	movl	(%rdx,%rax), %eax
	cltq
	leaq	.L27(%rip), %rdx
	addq	%rdx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L27:
	.long	.L35-.L27
	.long	.L34-.L27
	.long	.L33-.L27
	.long	.L32-.L27
	.long	.L31-.L27
	.long	.L30-.L27
	.long	.L29-.L27
	.long	.L28-.L27
	.long	.L52-.L27
	.long	.L52-.L27
	.long	.L52-.L27
	.long	.L52-.L27
	.long	.L52-.L27
	.long	.L52-.L27
	.long	.L26-.L27
	.text
.L35:
	movzbl	-23(%rbp), %edx
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC11(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L25
.L34:
	movzbl	-23(%rbp), %edx
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC12(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L25
.L33:
	movzbl	-23(%rbp), %edx
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC13(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L25
.L32:
	movzbl	-23(%rbp), %edx
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC14(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L25
.L31:
	movzbl	-23(%rbp), %edx
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC15(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L25
.L30:
	movzbl	-23(%rbp), %edx
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC16(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L25
.L29:
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC17(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L25
.L28:
	movzwl	-20(%rbp), %ecx
	movzbl	-23(%rbp), %edx
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC18(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L25
.L26:
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC19(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
.L25:
	jmp	.L52
.L13:
	movzbl	-23(%rbp), %edx
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC20(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L5
.L12:
	movzwl	-18(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC21(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L5
.L11:
	movzwl	-18(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC22(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L5
.L10:
	movzbl	-21(%rbp), %edx
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC23(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L5
.L9:
	movzbl	-22(%rbp), %ecx
	movzbl	-23(%rbp), %edx
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC24(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L5
.L8:
	cmpb	$-98, -21(%rbp)
	jne	.L36
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC25(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L53
.L36:
	cmpb	$-95, -21(%rbp)
	jne	.L53
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC26(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L53
.L6:
	movzbl	-21(%rbp), %eax
	cmpl	$101, %eax
	je	.L38
	cmpl	$101, %eax
	jg	.L54
	cmpl	$51, %eax
	jg	.L40
	cmpl	$7, %eax
	jl	.L54
	subl	$7, %eax
	cmpl	$44, %eax
	ja	.L54
	movl	%eax, %eax
	leaq	0(,%rax,4), %rdx
	leaq	.L42(%rip), %rax
	movl	(%rdx,%rax), %eax
	cltq
	leaq	.L42(%rip), %rdx
	addq	%rdx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L42:
	.long	.L48-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L47-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L46-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L45-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L44-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L43-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L54-.L42
	.long	.L41-.L42
	.text
.L40:
	cmpl	$85, %eax
	je	.L49
	jmp	.L54
.L48:
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC27(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L39
.L47:
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC28(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L39
.L46:
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC29(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L39
.L45:
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC30(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L39
.L44:
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC31(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L39
.L43:
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC32(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L39
.L41:
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC33(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L39
.L49:
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC34(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L39
.L38:
	movzwl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC35(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
.L39:
	jmp	.L54
.L51:
	nop
	jmp	.L5
.L52:
	nop
	jmp	.L5
.L53:
	nop
	jmp	.L5
.L54:
	nop
.L5:
	movl	$10, %edi
	call	putchar@PLT
	addl	$2, -16(%rbp)
.L4:
	cmpl	$289, -16(%rbp)
	jle	.L50
	movl	$0, %eax
.L3:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.ident	"GCC: (GNU) 14.1.1 20240720"
	.section	.note.GNU-stack,"",@progbits
