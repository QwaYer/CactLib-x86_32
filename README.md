# 🌵 CactLib/x86_32

<p align="center">
  <img src="https://img.shields.io/badge/license-GPLv3-blue.svg?style=for-the-badge" alt="License: GPLv3">
  <img src="https://img.shields.io/badge/arch-i686-red.svg?style=for-the-badge" alt="Arch: i686">
  <img src="https://img.shields.io/badge/language-C%2FASM-orange.svg?style=for-the-badge" alt="Language: C/ASM">
  <img src="https://img.shields.io/badge/output-libc.a-green.svg?style=for-the-badge" alt="Output: libc.a">
  <img src="https://img.shields.io/badge/syscall-int%200x80-purple.svg?style=for-the-badge" alt="int 0x80">
  <img src="https://img.shields.io/badge/status-pre--1.0-yellow.svg?style=for-the-badge" alt="pre-1.0">
</p>

<p align="center">
  C standard library for <a href="https://github.com/QwaYer/CactOS-x86_32">CactOS</a> userspace — freestanding, no host libc, pure <code>int 0x80</code> syscall interface.
</p>

---

## 📦 Overview

| | |
|---|---|
| **Source files** | 14 `.c` / `.S` |
| **Headers** | 17 |
| **Syscall IDs** | 65 |
| **Host dependencies** | 0 |
| **Output** | `libc.a` (static archive) |

CactLib — минималистичная реализация стандартной библиотеки C для пользовательского пространства ядра CactOS. Компилируется в статический архив `libc.a`, линкуется с любым userspace ELF-бинарником. Нет зависимостей от host libc — всё через `int 0x80`.

> Часть CactOS ecosystem: **CactKernel** (ядро) · **CactLib** (libc) · **Cactsole** (консоль) · **Optics** (графический сервер)

---

## 🔨 Building

**Requirements:** `gcc ≥ 9.0` with `-m32` support, `binutils`, `make`

```sh
# Clone
git clone https://github.com/QwaYer/CactLib-x86_32
cd CactLib-x86_32

# Build → libc.a
make

# Clean
make clean
```

**Compiler flags:**
```makefile
CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -Iinclude -Wall -Wextra
```

**Linking your binary:**
```sh
gcc -m32 -nostdlib -ffreestanding -o myprogram myprogram.o libc.a
```

> ⚠️ Собирается **только под i686**. На x86_64 хосте может потребоваться `gcc-multilib`.

---

## 📂 Structure

```
CactLib-x86_32/
├── src/                    ← implementation (.c + .S)
│   ├── stdio.c             printf, puts, putchar, kprint, rename
│   ├── stdlib.c            malloc, free, calloc, realloc, exit
│   ├── string.c            memset, memcpy, strlen, strcmp, itoa…
│   ├── unistd.c            read, write, fork, exec, pipe, select…
│   ├── socket.c            socket, bind, connect, send, recv…
│   ├── signal.c            signal, kill, sigprocmask, alarm…
│   ├── stat.c              stat, fstat
│   ├── dirent.c            getdents
│   ├── fcntl.c             open, fcntl, symlink, readlink…
│   ├── mman.c              mmap, munmap, mprotect
│   ├── shm.c               shmget, shmat, shmdt, shmctl
│   ├── termios.c           tcgetattr, tcsetattr
│   ├── time.c              gettimeofday, clock_gettime, nanosleep
│   └── syscall.c           raw syscall() trampoline
├── include/                ← public headers
│   ├── stdio.h  string.h  stdlib.h  unistd.h
│   ├── socket.h signal.h  stat.h    dirent.h
│   ├── fcntl.h  poll.h    select.h  termios.h
│   ├── time.h   shm.h     start.h   syscall.h
│   └── sys/mman.h
├── Makefile                → libc.a
└── LICENSE                 GPLv3
```

---

## 📖 API Reference

### stdio.h
| Function | Signature |
|----------|-----------|
| `printf` | `(const char *fmt, ...) → int` |
| `puts` | `(const char *str) → int` |
| `putchar` | `(int c) → int` |
| `kprint` | `(const char *s)` — kernel debug print via `SYS_PRINT` |
| `rename` | `(const char *old, const char *new) → int` |

> ⚠️ `printf` поддерживает только `%d` `%s` `%x` `%c` `%%`. Флаги ширины/точности не реализованы.

### stdlib.h
| Function | Signature |
|----------|-----------|
| `malloc` | `(size_t size) → void*` |
| `free` | `(void *ptr)` |
| `calloc` | `(size_t n, size_t size) → void*` |
| `realloc` | `(void *ptr, size_t size) → void*` |
| `exit` | `(int status)` |
| `atoi` | `(const char *str) → int` |
| `itoa` | `(int n, char *buf)` |
| `hex_to_ascii` | `(unsigned int n, char *buf)` |

### string.h
`memset` · `memcpy` · `memcmp` · `strlen` · `strcmp` · `strncmp` · `strcpy` · `strncpy` · `strcat` · `buf_append` · `buf_append_int`

### unistd.h
`read` · `write` · `open` · `close` · `fork` · `execve` · `getpid` · `getppid` · `lseek` · `waitpid` · `pipe` · `dup2` · `select` · `poll` · `getcwd` · `chdir` · `mkdir` · `rmdir` · `ioctl` · `sleep` · `usleep` · `sbrk` · `brk`

### socket.h
`socket` · `bind` · `connect` · `listen` · `accept` · `send` · `recv` · `sendto` · `recvfrom` · `shutdown` · `setsockopt` · `getsockopt`

### signal.h
`signal` · `kill` · `sigprocmask` · `sigpending` · `sigsuspend` · `alarm` · `setitimer`

### Other
- **`sys/mman.h`** — `mmap`, `munmap`, `mprotect`
- **`shm.h`** — `shmget`, `shmat`, `shmdt`, `shmctl`
- **`time.h`** — `gettimeofday`, `clock_gettime`, `nanosleep`
- **`termios.h`** — `tcgetattr`, `tcsetattr`
- **`fcntl.h`** — `open`, `fcntl`, `symlink`, `readlink`, `link`, `unlink`

---

## 🧠 malloc internals

Реализован через `brk` syscall. Линейный список блоков:

```
┌──────────────────┬───────────────────────┬──────────────┐
│  block_header    │  user data            │  remainder   │
│  16 bytes        │  ALIGN8(size)         │  if > 16B    │
└──────────────────┴───────────────────────┴──────────────┘
```

```c
struct block_header {
    uint32_t magic;              // 0xA110CA7E — integrity check
    uint32_t size;               // usable bytes (excl. header)
    uint32_t is_free;            // 1 = available for reuse
    struct block_header *next;
};
```

- **First-fit** поиск по `free_list`
- **Коалесценция** соседних свободных блоков при `free()`
- Минимальный запрос к ядру: `max(header + size, 4096)` байт

---

## ⚙️ ABI / int 0x80

```
EAX = syscall number
EBX = arg1
ECX = arg2
EDX = arg3
→ return in EAX
```

```c
// Inline asm wrapper (from syscall.h)
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

**Syscall table (partial):**

| # | Name | Notes |
|---|------|-------|
| 0 | `SYS_PRINT` | kernel debug print |
| 3 | `SYS_READ` | fd, buf, count |
| 4 | `SYS_WRITE` | fd, buf, count |
| 7 | `SYS_EXIT` | status |
| 9 | `SYS_FORK` | COW via kernel |
| 10 | `SYS_EXEC` | path, argv, envp |
| 13 | `SYS_MMAP` | addr, len, prot, flags, fd, off |
| 23 | `SYS_BRK` | addr → new brk |
| 43 | `SYS_SOCKET` | domain, type, protocol |
| 61 | `SYS_SHMGET` | key, size, flags |

---

## ⚖️ License

**GNU General Public License v3.0** — see [`LICENSE`](LICENSE)

---

**Developer:** [QwaYer](https://github.com/QwaYer) · **Kernel:** [CactKernel-x86_32](https://github.com/QwaYer/CactKernel-x86_32) · **OS:** [CactOS-x86_32](https://github.com/QwaYer/CactOS-x86_32)
