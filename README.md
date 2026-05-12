# 🌵 CactLib/x86_32

<p align="center">
  <img src="https://img.shields.io/badge/version-1.0.0-green.svg?style=for-the-badge" alt="Version: 1.0.0">
  <img src="https://img.shields.io/badge/license-GPLv3-blue.svg?style=for-the-badge" alt="License: GPLv3">
  <img src="https://img.shields.io/badge/arch-i686-red.svg?style=for-the-badge" alt="Arch: i686">
  <img src="https://img.shields.io/badge/language-C%2FASM-orange.svg?style=for-the-badge" alt="Language: C/ASM">
  <img src="https://img.shields.io/badge/output-libc.a%20%2B%20libc.so-green.svg?style=for-the-badge" alt="=libc.so">
  <img src="https://img.shields.io/badge/syscall-int%200x80-purple.svg?style=for-the-badge" alt="int 0x80">
  <img src="https://img.shields.io/badge/status-pre--1.0-yellow.svg?style=for-the-badge" alt="pre-1.0">
</p>

<p align="center">
  A <strong>freestanding C library</strong> for <a href="https://github.com/QwaYer/CactOS-x86_32"><strong>CactOS</strong></a> user space on <strong>i686</strong>.<br>
  No host libc — every OS entry goes through <code>int 0x80</code> with numbers from <a href="include/syscall.h"><code>include/syscall.h</code></a>, which must match <a href="https://github.com/QwaYer/CactKernel-x86_32"><strong>CactKernel-x86_32</strong></a> <code>syscalls.h</code>.
</p>

---

## 📦 Overview

| | |
|---|---|
| **Sources** | 15 × `src/*.c` + `start.S` |
| **Public headers** | 18 files under `include/` (including `sys/mman.h`) |
| **Syscall IDs** | **95** — `SYS_SYSCALL_COUNT` mirrors the kernel enum |
| **Runtime deps** | None (only the cross `gcc`/`ar`/`ld` toolchain) |
| **Artifacts** | **`libc.a`** (static), **`libc.so`** (shared ET_DYN for PIE), **`build/pic/start.o`** (PIC `_start`) |

CactLib is the **contract surface** between user ELF binaries and the kernel. If you add or renumber a syscall in the kernel, you **must**:

1. Update **`include/syscall.h`** here to match **`Cact/kernel/core/syscalls/syscalls.h`**.
2. Rebuild **`libc.a`** / **`libc.so`** (`make`).
3. **Re-link every user program** (init, shell, demos, drivers’ staged ELFs) against the new archive / shared object.

**Ecosystem:** **CactKernel** · **CactLib** · **Cactsole** · **Cgoct** · **LocalRepoCactOS** (`cctkfs.img` packer).

---

## 🔨 Building

**Requirements:** `gcc` with **`-m32`**, GNU **`binutils`**, **`make`**. On amd64 Debian/Ubuntu install **`gcc-multilib`** or an equivalent multilib toolchain.

```sh
git clone https://github.com/QwaYer/CactLib-x86_32
cd CactLib-x86_32

make              # libc.a + libc.so + PIC objects under build/pic/
make clean        # remove build/ trees and libraries
```

**Default `CFLAGS`** (see [`Makefile`](Makefile)):

```makefile
CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib \
         -Iinclude -Wall -Wextra
```

**Static link example**

```sh
gcc -m32 -nostdlib -ffreestanding -o myprogram myprogram.o libc.a
```

**Shared / PIE note:** `libc.so` is built as **`ET_DYN`** with a fixed link script (`libc.ld`). PIE executables link against **`build/pic/start.o`** + relocatable `*.o` from `build/pic/`.

> ⚠️ **i686 only.** Building `-m32` will fail on a pure 64-bit toolchain without multilib.

---

## 📂 Layout

```
CactLib-x86_32/
├── src/
│   ├── stdio.c      printf family, puts, putchar, kprint (SYS_PRINT), rename
│   ├── stdlib.c     malloc/brk heap, exit, atoi, itoa, …
│   ├── string.c     memset, memcpy, strlen, strcmp, …
│   ├── unistd.c     POSIX-like file + process + mount + module syscalls
│   ├── socket.c     BSD sockets (stream/dgram)
│   ├── dns.c        dns_resolve() → SYS_DNS_RESOLVE
│   ├── signal.c     signals, masks, alarm, interval timers
│   ├── stat.c       stat, fstat
│   ├── dirent.c     getdents
│   ├── fcntl.c      open variants, symlink, readlink, link, unlink, …
│   ├── mman.c       mmap, munmap, mprotect
│   ├── shm.c        SysV shared memory wrappers
│   ├── termios.c    tcgetattr / tcsetattr
│   ├── time.c       clocks + nanosleep
│   ├── syscall.c    variadic syscall() helper
│   └── start.S      user _start (non-PIC + PIC flavours)
├── include/
│   ├── syscall.h    authoritative SYS_* list — keep identical to the kernel!
│   ├── socket.h     sockaddr_in helpers + dns_resolve
│   ├── stdio.h string.h stdlib.h unistd.h …
│   └── sys/mman.h
├── Makefile
└── LICENSE          GPLv3
```

---

## 📖 API reference

### `stdio.h`

| Function | Role |
|----------|------|
| `printf` | Tiny `printf` — see format limits below |
| `puts` / `putchar` | Line / character output to fd 1 |
| `kprint` | Writes to the kernel debug channel via **`SYS_PRINT`** |
| `rename` | `rename(2)` wrapper |

> ⚠️ **`printf`** supports only **`%d` `%s` `%x` `%c` `%%`**. Width, precision, and floating-point formats are **not** implemented.

### `stdlib.h`

| Function | Role |
|----------|------|
| `malloc` / `free` / `calloc` / `realloc` | See **malloc internals** below |
| `exit` | `_exit` via syscall |
| `atoi` | naive decimal parser |
| `itoa` / `hex_to_ascii` | small integer → ASCII helpers |

### `string.h`

`memset` · `memcpy` · `memcmp` · `strlen` · `strcmp` · `strncmp` · `strcpy` · `strncpy` · `strcat` · `buf_append` · `buf_append_int` …

### `unistd.h`

Core POSIX-like wrappers: `read`, `write`, `open`, `close`, `fork`, `execve`, `getpid`, `getppid`, `waitpid`, `lseek`, `pipe`, `dup`, `dup2`, `select`, `poll`, `getcwd`, `chdir`, `mkdir`, `rmdir`, `ioctl`, `sleep`, `brk`, **`mount`/`umount`**, **`module_load` / `module_unload`** (**`SYS_MODULE_LOAD` 92**, **`SYS_MODULE_UNLOAD` 93**), and many more — always cross-check the `.c` file against [`syscall.h`](include/syscall.h).

### `socket.h`

Full BSD-style set: `socket`, `bind`, `connect`, `listen`, `accept`, `send`, `recv`, `sendto`, `recvfrom`, `shutdown`, `setsockopt`, `getsockopt`.

Additionally **`dns_resolve(const char *name, uint32_t *out_ip_host)`** wraps **`SYS_DNS_RESOLVE` (94)** — resolves a **dotted IPv4 literal** or performs a **blocking DNS A query** (requires the kernel to have a DNS server address from DHCP or `netcfg_set`). Return **`0`** on success, **`-1`** on error.

### `signal.h`

`signal`, `kill`, `sigaction`, `sigprocmask`, `sigpending`, `sigsuspend`, `alarm`, `setitimer`.

### Other headers

| Header | Highlights |
|--------|------------|
| **`sys/mman.h`** | `mmap`, `munmap`, `mprotect` |
| **`shm.h`** | `shmget`, `shmat`, `shmdt`, `shmctl` |
| **`time.h`** | `gettimeofday`, `clock_gettime`, `nanosleep` |
| **`termios.h`** | `tcgetattr`, `tcsetattr` |
| **`fcntl.h`** / **`poll.h`** / **`select.h`** | open flags, non-blocking pollable fds |

> 💡 Thin libc wrappers for **`SYS_PING_ECHO` (90)** and **`SYS_NETCFG_SET` (91)** may be missing — call **`syscall()`** from [`syscall.c`](src/syscall.c) / inline helpers with the right argument packing as the kernel expects.

---

## 🧠 `malloc` internals

The heap is a **singly linked list** of blocks carved out of the **`brk`** region returned by the kernel.

```
┌──────────────────┬───────────────────────┬──────────────┐
│  block_header    │  user payload        │  slack       │
│  16 bytes        │  8-byte aligned      │  optional    │
└──────────────────┴───────────────────────┴──────────────┘
```

```c
struct block_header {
    uint32_t magic;              /* 0xA110CA7E — canary */
    uint32_t size;             /* usable bytes excluding header */
    uint32_t is_free;          /* 1 = on free list */
    struct block_header *next;
};
```

- **First-fit** scan across `free_list`.
- **Coalescing** of adjacent free blocks on `free()`.
- **`sbrk`** requests are rounded up to at least **`max(header + size, 4096)`** bytes to reduce syscall chatter.

---

## ⚙️ ABI — `int 0x80`

**Register convention (3-scalar syscalls):**

```
EAX = syscall number (SYS_*)
EBX = 1st argument
ECX = 2nd argument
EDX = 3rd argument
EAX ← return value (signed int semantics)
```

```c
static inline int __syscall3(int num, int a1, int a2, int a3) {
    int ret;
    __asm__ volatile (
        "int $0x80"
        : "=a"(ret)
        : "a"(num), "b"(a1), "c"(a2), "d"(a3)
        : "memory"
    );
    return ret;
}
```

Many syscalls pass a **pointer to a struct** in **EBX** (and sometimes use **`__syscall1`**). The kernel distinguishes those with `_needs_frame()` — see CactKernel **`syscall_handler`** in `mod.c`.

**Sample numbers** (full table only in [`include/syscall.h`](include/syscall.h)):

| # | Constant | Typical use |
|---|----------|-------------|
| 0 | `SYS_PRINT` | Debug string to kernel console |
| 3 | `SYS_FORK` | |
| 5 | `SYS_EXIT` | |
| 22 | `SYS_READ` | |
| 23 | `SYS_WRITE` | |
| 61 | `SYS_BRK` | |
| 62 | `SYS_MMAP` | Uses full register frame in kernel |
| 65 | `SYS_SHMGET` | |
| 78 | `SYS_SOCKET` | |
| 90 | `SYS_PING_ECHO` | ICMP echo helper (kernel / Rust path) |
| 91 | `SYS_NETCFG_SET` | Push IPv4 / DHCP metadata into Rust stack |
| 92 | `SYS_MODULE_LOAD` | |
| 93 | `SYS_MODULE_UNLOAD` | |
| 94 | `SYS_DNS_RESOLVE` | `dns_resolve()` |

---

## ⚖️ License

**GNU General Public License v3.0** — see [`LICENSE`](LICENSE).

---

<p align="center">
  <strong>Developer:</strong> <a href="https://github.com/QwaYer">QwaYer</a>
  &nbsp;·&nbsp; <strong>Kernel:</strong> <a href="https://github.com/QwaYer/CactKernel-x86_32">CactKernel-x86_32</a>
  &nbsp;·&nbsp; <strong>OS:</strong> <a href="https://github.com/QwaYer/CactOS-x86_32">CactOS-x86_32</a>
</p>
