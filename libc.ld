/*
 * Линкер-скрипт для libc.so — ET_DYN с фиксированной базой 0x10000000.
 *
 * Главные PIE-бинари (cactsole/cgoct) живут на 0x08000000 — выбран запас в
 * ~128 МБ между секциями кода, чтобы не пересекаться по PT_LOAD при mmap'е
 * в kernel dynlinker'е. Если libc вырастет — поднимаем сюда же.
 */

SECTIONS {
    . = 0x10000000;

    .dynamic   : { *(.dynamic) }
    .hash      : { *(.hash) }
    .gnu.hash  : { *(.gnu.hash) }
    .dynsym    : { *(.dynsym) }
    .dynstr    : { *(.dynstr) }
    .rel.dyn   : { *(.rel.dyn) }
    .rel.plt   : { *(.rel.plt) }

    .init      : { *(.init) }
    .plt       : { *(.plt) }
    .text      : { *(.text .text.*) }
    .fini      : { *(.fini) }

    .rodata    : { *(.rodata .rodata.*) }
    .eh_frame  : { *(.eh_frame) }

    . = ALIGN(0x1000);
    .data      : { *(.data .data.*) }
    .got       : { *(.got) }
    .got.plt   : { *(.got.plt) }
    .bss       : { *(.bss .bss.*) *(COMMON) }
}
