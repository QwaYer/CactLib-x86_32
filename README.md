# CactLib

C standard library for Cact kernel userspace.

## Structure

```
src/               - userspace libc sources (compiled to libc.a)
include/           - userspace libc headers
Makefile           - builds libc.a from src/
```

## Building

```
make        # builds libc.a
make clean  # removes build artifacts
```

## License

GPLv3
