.global setjmp
.type setjmp, @function
setjmp:
    movl 4(%esp), %ecx
    movl %ebx, (%ecx)
    movl %esi, 4(%ecx)
    movl %edi, 8(%ecx)
    movl %ebp, 12(%ecx)
    movl %esp, 16(%ecx)
    movl (%esp), %eax
    movl %eax, 20(%ecx)
    xorl %eax, %eax
    ret

.global longjmp
.type longjmp, @function
longjmp:
    movl 4(%esp), %ecx
    movl 8(%esp), %eax
    movl (%ecx), %ebx
    movl 4(%ecx), %esi
    movl 8(%ecx), %edi
    movl 12(%ecx), %ebp
    movl 16(%ecx), %esp
    movl 20(%ecx), %edx
    jmp *%edx
