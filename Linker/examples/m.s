
m.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <main>:
   0:	f3 0f 1e fa          	endbr64 
   4:	55                   	push   %rbp
   5:	48 89 e5             	mov    %rsp,%rbp
   8:	48 83 ec 10          	sub    $0x10,%rsp
   c:	89 7d fc             	mov    %edi,-0x4(%rbp)
   f:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  13:	48 8d 3d 00 00 00 00 	lea    0x0(%rip),%rdi        # 1a <main+0x1a>
  1a:	e8 00 00 00 00       	callq  1f <main+0x1f>
  1f:	b8 00 00 00 00       	mov    $0x0,%eax
  24:	c9                   	leaveq 
  25:	c3                   	retq   

Disassembly of section .data:

0000000000000000 <string.1916>:
   0:	48                   	rex.W
   1:	65 6c                	gs insb (%dx),%es:(%rdi)
   3:	6c                   	insb   (%dx),%es:(%rdi)
   4:	6f                   	outsl  %ds:(%rsi),(%dx)
   5:	2c 20                	sub    $0x20,%al
   7:	77 6f                	ja     78 <string.1916+0x78>
   9:	72 6c                	jb     77 <string.1916+0x77>
   b:	64 21 0a             	and    %ecx,%fs:(%rdx)
	...

Disassembly of section .comment:

0000000000000000 <.comment>:
   0:	00 47 43             	add    %al,0x43(%rdi)
   3:	43 3a 20             	rex.XB cmp (%r8),%spl
   6:	28 55 62             	sub    %dl,0x62(%rbp)
   9:	75 6e                	jne    79 <main+0x79>
   b:	74 75                	je     82 <main+0x82>
   d:	20 39                	and    %bh,(%rcx)
   f:	2e 34 2e             	cs xor $0x2e,%al
  12:	30 2d 31 75 62 75    	xor    %ch,0x75627531(%rip)        # 75627549 <main+0x75627549>
  18:	6e                   	outsb  %ds:(%rsi),(%dx)
  19:	74 75                	je     90 <main+0x90>
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
  24:	26 00 00             	add    %al,%es:(%rax)
  27:	00 00                	add    %al,(%rax)
  29:	45 0e                	rex.RB (bad) 
  2b:	10 86 02 43 0d 06    	adc    %al,0x60d4302(%rsi)
  31:	5d                   	pop    %rbp
  32:	0c 07                	or     $0x7,%al
  34:	08 00                	or     %al,(%rax)
	...
