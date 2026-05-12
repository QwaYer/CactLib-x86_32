/*
 * crt0 для CactOS. argc/argv/envp лежат на стеке (System V i386 ABI):
 *     [esp]      = argc
 *     [esp + 4]  = argv
 *     [esp + 8]  = envp
 *
 * `@PLT` на call'ах нужен для PIE-режима: внешний `exit` (в libc.so)
 * резолвится через PLT/GOT, без правок .text при загрузке (без TEXTREL).
 * Для статической линковки R_386_PLT32 эквивалентен R_386_PC32, поэтому
 * один и тот же start.o работает и в .a, и в PIE.
 */

.section .text
.global _start
.type _start, @function

_start:
    /* i386 PIC: PLT uses `jmp *disp(%ebx)`; ebx must point at _GLOBAL_OFFSET_TABLE_. */
    call    1f
1:
    popl    %ebx
    addl    $(_GLOBAL_OFFSET_TABLE_ - 1b), %ebx

    movl (%esp),  %eax       /* argc */
    movl 4(%esp), %ecx       /* argv */
    movl 8(%esp), %edx       /* envp */
    pushl %edx
    pushl %ecx
    pushl %eax
    call main@PLT
    addl $12, %esp
    pushl %eax
    call exit@PLT
    hlt

/* Помечаем стек как не-исполняемый, чтобы ld не ругался executable stack. */
.section .note.GNU-stack,"",@progbits
