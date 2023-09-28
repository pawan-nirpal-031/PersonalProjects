
a.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <a>:
   0:	f3 0f 1e fa          	endbr64 
   4:	55                   	push   %rbp
   5:	48 89 e5             	mov    %rsp,%rbp
   8:	48 83 ec 10          	sub    $0x10,%rsp
   c:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  10:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  14:	48 89 c7             	mov    %rax,%rdi
  17:	e8 00 00 00 00       	callq  1c <a+0x1c>
  1c:	48 89 c2             	mov    %rax,%rdx
  1f:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  23:	48 89 c6             	mov    %rax,%rsi
  26:	bf 01 00 00 00       	mov    $0x1,%edi
  2b:	e8 00 00 00 00       	callq  30 <a+0x30>
  30:	90                   	nop
  31:	c9                   	leaveq 
  32:	c3                   	retq   

Disassembly of section .comment:

0000000000000000 <.comment>:
   0:	00 47 43             	add    %al,0x43(%rdi)
   3:	43 3a 20             	rex.XB cmp (%r8),%spl
   6:	28 55 62             	sub    %dl,0x62(%rbp)
   9:	75 6e                	jne    79 <a+0x79>
   b:	74 75                	je     82 <a+0x82>
   d:	20 39                	and    %bh,(%rcx)
   f:	2e 34 2e             	cs xor $0x2e,%al
  12:	30 2d 31 75 62 75    	xor    %ch,0x75627531(%rip)        # 75627549 <a+0x75627549>
  18:	6e                   	outsb  %ds:(%rsi),(%dx)
  19:	74 75                	je     90 <a+0x90>
  1b:	31 7e 32             	xor    %edi,0x32(%rsi)
  1e:	30 2e                	xor    %ch,(%rsi)
  20:	30 34 2e             	xor    %dh,(%rsi,%rbp,1)
  23:	31 29                	xor    %ebp,(%rcx)
  25:	20 39                	and    %bh,(%rcx)
  27:	2e 34 2e             	cs xor $0x2e,%al
  2a:	30 00                	xor    %al,(%rax)

Disassembly of section .note.gnu.property:

0000000000000000 <.note.gnu.property>:
   0:	04 00                	add    $0x0,%al
   2:	00 00                	add    %al,(%rax)
   4:	10 00                	adc    %al,(%rax)
   6:	00 00                	add    %al,(%rax)
   8:	05 00 00 00 47       	add    $0x47000000,%eax
   d:	4e 55                	rex.WRX push %rbp
   f:	00 02                	add    %al,(%rdx)
  11:	00 00                	add    %al,(%rax)
  13:	c0 04 00 00          	rolb   $0x0,(%rax,%rax,1)
  17:	00 03                	add    %al,(%rbx)
  19:	00 00                	add    %al,(%rax)
  1b:	00 00                	add    %al,(%rax)
  1d:	00 00                	add    %al,(%rax)
	...

Disassembly of section .eh_frame:

0000000000000000 <.eh_frame>:
   0:	14 00                	adc    $0x0,%al
   2:	00 00                	add    %al,(%rax)
   4:	00 00                	add    %al,(%rax)
   6:	00 00                	add    %al,(%rax)
   8:	01 7a 52             	add    %edi,0x52(%rdx)
   b:	00 01                	add    %al,(%rcx)
   d:	78 10                	js     1f <.eh_frame+0x1f>
   f:	01 1b                	add    %ebx,(%rbx)
  11:	0c 07                	or     $0x7,%al
  13:	08 90 01 00 00 1c    	or     %dl,0x1c000001(%rax)
  19:	00 00                	add    %al,(%rax)
  1b:	00 1c 00             	add    %bl,(%rax,%rax,1)
  1e:	00 00                	add    %al,(%rax)
  20:	00 00                	add    %al,(%rax)
  22:	00 00                	add    %al,(%rax)
  24:	33 00                	xor    (%rax),%eax
  26:	00 00                	add    %al,(%rax)
  28:	00 45 0e             	add    %al,0xe(%rbp)
  2b:	10 86 02 43 0d 06    	adc    %al,0x60d4302(%rsi)
  31:	6a 0c                	pushq  $0xc
  33:	07                   	(bad)  
  34:	08 00                	or     %al,(%rax)
	...
