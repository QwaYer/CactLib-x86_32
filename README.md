# 🌵 CactLib/x86_32

<p align="center">
  <img src="https://img.shields.io/badge/version-1.0.0-green.svg?style=for-the-badge" alt="Version: 1.0.0">
  <img src="https://img.shields.io/badge/license-GPLv3-blue.svg?style=for-the-badge" alt="License: GPLv3">
  <img src="https://img.shields.io/badge/arch-i686-red.svg?style=for-the-badge" alt="Arch: i686">
  <img src="https://img.shields.io/badge/language-C%2FASM-orange.svg?style=for-the-badge" alt="Language: C/ASM">
  <img src="https://img.shields.io/badge/output-libc.a%20%2B%20libc.so-green.svg?style=for-the-badge" alt="libc.a + libc.so">
  <img src="https://img.shields.io/badge/syscall-int%200x80-purple.svg?style=for-the-badge" alt="int 0x80">
  <img src="https://img.shields.io/badge/status-pre--1.0-yellow.svg?style=for-the-badge" alt="pre-1.0">
</p>

<p align="center">
  C standard library for <a href="https://github.com/QwaYer/CactOS-x86_32"><strong>CactOS</strong></a> userspace — freestanding, no host libc, pure <code>int 0x80</code> syscall interface.<br>
  Пары с ядром: <a href="https://github.com/QwaYer/CactKernel-x86_32"><strong>CactKernel-x86_32</strong></a> · номера <code>SYS_*</code> в <a href="include/syscall.h"><code>include/syscall.h</code></a>.
</p>

---

## 📦 Overview

| | |
|---|---|
| **Исходники** | 15 `src/*.c` + `start.S` |
| **Заголовки** | 18 файлов в `include/` (в т.ч. `sys/mman.h`) |
| **Syscall IDs** | **95** (`SYS_SYSCALL_COUNT` — зеркало ядра) |
| **Host dependencies** | 0 (только toolchain) |
| **Output** | **`libc.a`** · **`libc.so`** · PIC-`start.o` |

> Часть экосистемы: **CactKernel** (ядро) · **CactLib** (libc) · **Cactsole** · **Cgoct** · **LocalRepoCactOS** (`cctkfs.img`)

---

## 🔨 Building

**Requirements:** `gcc` с `-m32` (`gcc-multilib` на amd64), `binutils`, `make`

```sh
git clone https://github.com/QwaYer/CactLib-x86_32
cd CactLib-x86_32

# Build → libc.a + libc.so + build/pic/start.o
make

# Clean
make clean
```

**Compiler flags:**

```makefile
CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib \
         -Iinclude -Wall -Wextra
```

**Статическая линковка:**

```sh
gcc -m32 -nostdlib -ffreestanding -o myprogram myprogram.o libc.a
```

> ⚠️ Собирается **только под i686**. На x86_64 без `gcc-multilib` сборка не взлетит.

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
│   ├── dns.c               dns_resolve → SYS_DNS_RESOLVE
│   ├── signal.c            signal, kill, sigprocmask, alarm…
│   ├── stat.c              stat, fstat
│   ├── dirent.c            getdents
│   ├── fcntl.c             open, fcntl, symlink, readlink…
│   ├── mman.c              mmap, munmap, mprotect
│   ├── shm.c               shmget, shmat, shmdt, shmctl
│   ├── termios.c           tcgetattr, tcsetattr
│   ├── time.c              gettimeofday, clock_gettime, nanosleep
│   ├── syscall.c           raw syscall() trampoline
│   └── start.S             _start (обычный + PIC)
├── include/
│   ├── syscall.h           все SYS_* (должны совпадать с ядром!)
│   ├── socket.h          сокеты + dns_resolve
│   ├── stdio.h string.h stdlib.h unistd.h …
│   └── sys/mman.h
├── Makefile                → libc.a + libc.so
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
| `kprint` | `(const char *s)` — debug print via `SYS_PRINT` |
| `rename` | `(const char *old, const char *new) → int` |

> ⚠️ `printf` — только `%d` `%s` `%x` `%c` `%%`. Ширина/точность не реализованы.

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

`read` · `write` · `open` · `close` · `fork` · `execve` · `getpid` · `getppid` · `lseek` · `waitpid` · `pipe` · `dup` · `dup2` · `select` · `poll` · `getcwd` · `chdir` · `mkdir` · `rmdir` · `ioctl` · `sleep` · `brk` · **`module_load`** · **`module_unload`** (`SYS_MODULE_LOAD` **92** / `SYS_MODULE_UNLOAD` **93**)

### socket.h

`socket` · `bind` · `connect` · `listen` · `accept` · `send` · `recv` · `sendto` · `recvfrom` · `shutdown` · `setsockopt` · `getsockopt` · **`dns_resolve`** → **`SYS_DNS_RESOLVE` (94)**

### signal.h

`signal` · `kill` · `sigprocmask` · `sigpending` · `sigsuspend` · `alarm` · `setitimer`

### Other

- **`sys/mman.h`** — `mmap`, `munmap`, `mprotect`
- **`shm.h`** — `shmget`, `shmat`, `shmdt`, `shmctl`
- **`time.h`** — `gettimeofday`, `clock_gettime`, `nanosleep`
- **`termios.h`** — `tcgetattr`, `tcsetattr`
- **`fcntl.h`** — `open`, `fcntl`, `symlink`, `readlink`, `link`, `unlink`

> 💡 Прямых тонких обёрток под **`SYS_PING_ECHO` (90)** и **`SYS_NETCFG_SET` (91)** может не быть — при необходимости: `syscall(SYS_PING_ECHO, …)` из **`syscall.h`**.

---

## 🧠 malloc internals

Реализован через **`brk`**. Линейный список блоков:

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

- **First-fit** по `free_list`
- **Коалесценция** соседних свободных блоков при `free()`
- Минимальный рост к ядру: `max(header + size, 4096)` байт

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

**Примеры номеров** (полная таблица — только в [`include/syscall.h`](include/syscall.h)):

| # | Name | Notes |
|---|------|-------|
| 0 | `SYS_PRINT` | kernel debug print |
| 3 | `SYS_FORK` | |
| 5 | `SYS_EXIT` | status |
| 22 | `SYS_READ` | fd, buf, count |
| 23 | `SYS_WRITE` | fd, buf, count |
| 61 | `SYS_BRK` | |
| 62 | `SYS_MMAP` | кадр в ядре |
| 65 | `SYS_SHMGET` | |
| 78 | `SYS_SOCKET` | |
| 90 | `SYS_PING_ECHO` | ICMP echo helper |
| 91 | `SYS_NETCFG_SET` | IPv4 / DHCP metadata |
| 92 | `SYS_MODULE_LOAD` | path, vendor, device |
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
