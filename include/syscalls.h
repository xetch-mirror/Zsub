#ifndef SYSCALLS_H
#define SYSCALLS_H


#define O_RDONLY  0
#define O_WRONLY  1
#define O_CREAT   64
#define O_TRUNC   512

#define SEEK_SET  0
#define SEEK_END  2

#define S_IFMT   0170000
#define S_IFDIR  0040000
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)

/* Sufficient size compensates for the difference in 32/64-bit architectures. */
struct stat_custom {
    unsigned char representation[144];
};

#if defined(__x86_64__)
#define STAT_MODE(st)  (*(unsigned int *)((st)->representation + 24))
#define STAT_UID(st)   (*(unsigned int *)((st)->representation + 28))
#define STAT_GID(st)   (*(unsigned int *)((st)->representation + 32))
#define STAT_SIZE(st)  (*(long *)((st)->representation + 48))
#define STAT_MTIME(st) (*(unsigned long *)((st)->representation + 88))
#elif defined(__i386__)
#define STAT_MODE(st)  (*(unsigned short *)((st)->representation + 16))
#define STAT_UID(st)   (*(unsigned short *)((st)->representation + 24))
#define STAT_GID(st)   (*(unsigned short *)((st)->representation + 28))
#define STAT_SIZE(st)  (*(long *)((st)->representation + 44))
#define STAT_MTIME(st) (*(unsigned long *)((st)->representation + 64))
#else

#define STAT_MODE(st)  (*(unsigned int *)((st)->representation + 16))
#define STAT_UID(st)   (*(unsigned int *)((st)->representation + 20))
#define STAT_GID(st)   (*(unsigned int *)((st)->representation + 24))
#define STAT_SIZE(st)  (*(long *)((st)->representation + 48))
#define STAT_MTIME(st) (*(long *)((st)->representation + 72))
#endif

/* catalog getdents64 */
struct linux_dirent64 {
    unsigned long long d_ino;
    long long          d_off;
    unsigned short     d_reclen;
    unsigned char      d_type;
    char               d_name[1];
};

#if defined(__x86_64__)
#  define SYS_read        0
#  define SYS_write       1
#  define SYS_open        2
#  define SYS_close       3
#  define SYS_newstat     4
#  define SYS_lseek       8
#  define SYS_chdir       80
#  define SYS_mkdir       83
#  define SYS_exit        60
#  define SYS_getdents64  217

#elif defined(__i386__)
#  define SYS_exit        1
#  define SYS_read        3
#  define SYS_write       4
#  define SYS_open        5
#  define SYS_close       6
#  define SYS_mkdir       39
#  define SYS_lseek       19
#  define SYS_stat64      195
#  define SYS_chdir       12
#  define SYS_getdents64  220

#elif defined(__aarch64__)
#  define SYS_exit        93
#  define SYS_read        63
#  define SYS_write       64
#  define SYS_openat      56
#  define SYS_close       57
#  define SYS_mkdirat     34
#  define SYS_lseek       62
#  define SYS_fstatat     79
#  define SYS_chdir       49
#  define SYS_getdents64  61

#elif defined(__arm__)
#  define SYS_exit        1
#  define SYS_read        3
#  define SYS_write       4
#  define SYS_open        5
#  define SYS_close       6
#  define SYS_mkdir       39
#  define SYS_lseek       19
#  define SYS_stat64      195
#  define SYS_chdir       12
#  define SYS_getdents64  217

#elif defined(__mips__)
#  define SYS_exit        4001
#  define SYS_read        4003
#  define SYS_write       4004
#  define SYS_open        4005
#  define SYS_close       4006
#  define SYS_mkdir       4039
#  define SYS_lseek       4019
#  define SYS_stat64      4188
#  define SYS_chdir       4012
#  define SYS_getdents64  4219

#elif defined(__riscv)
#  define SYS_exit        93
#  define SYS_read        63
#  define SYS_write       64
#  define SYS_openat      56
#  define SYS_close       57
#  define SYS_mkdirat     34
#  define SYS_lseek       62
#  define SYS_fstatat     79
#  define SYS_chdir       49
#  define SYS_getdents64  61
#endif

static long my_syscall0(long num) {
    long ret;
#if defined(__x86_64__)
    __asm__ __volatile__("syscall" : "=a"(ret) : "a"(num) : "rcx", "r11", "memory");
#elif defined(__i386__)
    __asm__ __volatile__("int $0x80" : "=a"(ret) : "a"(num) : "memory");
#elif defined(__aarch64__)
    register long x8 __asm__("x8") = num;
    __asm__ __volatile__("svc #0" : "=r"(ret) : "r"(x8) : "memory");
#elif defined(__arm__)
    register long r7 __asm__("r7") = num;
    __asm__ __volatile__("swi 0" : "=r"(ret) : "r"(r7) : "memory");
#elif defined(__mips__)
    __asm__ __volatile__("syscall" : "=v0"(ret) : "v0"(num) : "$t0", "$t1", "$t2", "memory");
#elif defined(__riscv)
    register long a7 __asm__("a7") = num;
    __asm__ __volatile__("ecall" : "=r"(ret) : "r"(a7) : "memory");
#endif
    return ret;
}

static long my_syscall1(long num, long a1) {
    long ret;
#if defined(__x86_64__)
    __asm__ __volatile__("syscall" : "=a"(ret) : "a"(num), "D"(a1) : "rcx", "r11", "memory");
#elif defined(__i386__) /* Hack for I386 */
    __asm__ __volatile__("int $0x80" : "=a"(ret) : "a"(num), "b"(a1) : "memory");
#elif defined(__aarch64__)
    register long x0 __asm__("x0") = a1;
    register long x8 __asm__("x8") = num;
    __asm__ __volatile__("svc #0" : "=r"(ret) : "r"(x0), "r"(x8) : "memory");
#elif defined(__arm__)
    register long r0 __asm__("r0") = a1;
    register long r7 __asm__("r7") = num;
    __asm__ __volatile__("swi 0" : "=r"(ret) : "r"(r0), "r"(r7) : "memory");
#elif defined(__mips__)
    register long a0_reg __asm__("$4") = a1;
    __asm__ __volatile__("syscall" : "=v0"(ret) : "v0"(num), "r"(a0_reg) : "$t0", "$t1", "$t2", "memory");
#elif defined(__riscv)
    register long a0 __asm__("a0") = a1;
    register long a7 __asm__("a7") = num;
    __asm__ __volatile__("ecall" : "=r"(ret) : "r"(a0), "r"(a7) : "memory");
#endif
    return ret;
}

static long my_syscall2(long num, long a1, long a2) {
    long ret;
#if defined(__x86_64__)
    __asm__ __volatile__("syscall" : "=a"(ret) : "a"(num), "D"(a1), "S"(a2) : "rcx", "r11", "memory");
#elif defined(__i386__)
    __asm__ __volatile__("int $0x80" : "=a"(ret) : "a"(num), "b"(a1), "c"(a2) : "memory");
#elif defined(__aarch64__)
    register long x0 __asm__("x0") = a1;
    register long x1 __asm__("x1") = a2;
    register long x8 __asm__("x8") = num;
    __asm__ __volatile__("svc #0" : "=r"(ret) : "r"(x0), "r"(x1), "r"(x8) : "memory");
#elif defined(__arm__)
    register long r0 __asm__("r0") = a1;
    register long r1 __asm__("r1") = a2;
    register long r7 __asm__("r7") = num;
    __asm__ __volatile__("swi 0" : "=r"(ret) : "r"(r0), "r"(r1), "r"(r7) : "memory");
#elif defined(__mips__)
    register long a0_reg __asm__("$4") = a1;
    register long a1_reg __asm__("$5") = a2;
    __asm__ __volatile__("syscall" : "=v0"(ret) : "v0"(num), "r"(a0_reg), "r"(a1_reg) : "$t0", "$t1", "$t2", "memory");
#elif defined(__riscv)
    register long a0 __asm__("a0") = a1;
    register long a1 __asm__("a1") = a2;
    register long a7 __asm__("a7") = num;
    __asm__ __volatile__("ecall" : "=r"(ret) : "r"(a0), "r"(a1), "r"(a7) : "memory");
#endif
    return ret;
}

static long my_syscall3(long num, long a1, long a2, long a3) {
    long ret;
#if defined(__x86_64__)
    __asm__ __volatile__("syscall" : "=a"(ret) : "a"(num), "D"(a1), "S"(a2), "d"(a3) : "rcx", "r11", "memory");
#elif defined(__i386__)
    __asm__ __volatile__("int $0x80" : "=a"(ret) : "a"(num), "b"(a1), "c"(a2), "d"(a3) : "memory");
#elif defined(__aarch64__)
    register long x0 __asm__("x0") = a1;
    register long x1 __asm__("x1") = a2;
    register long x2 __asm__("x2") = a3;
    register long x8 __asm__("x8") = num;
    __asm__ __volatile__("svc #0" : "=r"(ret) : "r"(x0), "r"(x1), "r"(x2), "r"(x8) : "memory");
#elif defined(__arm__)
    register long r0 __asm__("r0") = a1;
    register long r1 __asm__("r1") = a2;
    register long r2 __asm__("r2") = a3;
    register long r7 __asm__("r7") = num;
    __asm__ __volatile__("swi 0" : "=r"(ret) : "r"(r0), "r"(r1), "r"(r2), "r"(r7) : "memory");
#elif defined(__mips__)
    register long a0_reg __asm__("$4") = a1;
    register long a1_reg __asm__("$5") = a2;
    register long a2_reg __asm__("$6") = a3;
    __asm__ __volatile__("syscall" : "=v0"(ret) : "v0"(num), "r"(a0_reg), "r"(a1_reg), "r"(a2_reg) : "$t0", "$t1", "$t2", "memory");
#elif defined(__riscv)
    register long a0 __asm__("a0") = a1;
    register long a1 __asm__("a1") = a2;
    register long a2 __asm__("a2") = a3;
    register long a7 __asm__("a7") = num;
    __asm__ __volatile__("ecall" : "=r"(ret) : "r"(a0), "r"(a1), "r"(a2), "r"(a7) : "memory");
#endif
    return ret;
}

static long my_syscall4(long num, long a1, long a2, long a3, long a4) {
    long ret;
#if defined(__x86_64__)
    register long r10 __asm__("r10") = a4;
    __asm__ __volatile__("syscall" : "=a"(ret) : "a"(num), "D"(a1), "S"(a2), "d"(a3), "r"(r10) : "rcx", "r11", "memory");
#elif defined(__i386__)
    __asm__ __volatile__("int $0x80" : "=a"(ret) : "a"(num), "b"(a1), "c"(a2), "d"(a3), "S"(a4) : "memory");
#elif defined(__aarch64__)
    register long x0 __asm__("x0") = a1;
    register long x1 __asm__("x1") = a2;
    register long x2 __asm__("x2") = a3;
    register long x3 __asm__("x3") = a4;
    register long x8 __asm__("x8") = num;
    __asm__ __volatile__("svc #0" : "=r"(ret) : "r"(x0), "r"(x1), "r"(x2), "r"(x3), "r"(x8) : "memory");
#elif defined(__arm__)
    register long r0 __asm__("r0") = a1;
    register long r1 __asm__("r1") = a2;
    register long r2 __asm__("r2") = a3;
    register long r3 __asm__("r3") = a4;
    register long r7 __asm__("r7") = num;
    __asm__ __volatile__("swi 0" : "=r"(ret) : "r"(r0), "r"(r1), "r"(r2), "r"(r3), "r"(r7) : "memory");
#elif defined(__mips__)
    register long a0_reg __asm__("$4") = a1;
    register long a1_reg __asm__("$5") = a2;
    register long a2_reg __asm__("$6") = a3;
    register long a3_reg __asm__("$7") = a4;
    __asm__ __volatile__("syscall" : "=v0"(ret) : "v0"(num), "r"(a0_reg), "r"(a1_reg), "r"(a2_reg), "r"(a3_reg) : "$t0", "$t1", "$t2", "memory");
#elif defined(__riscv)
    register long a0 __asm__("a0") = a1;
    register long a1 __asm__("a1") = a2;
    register long a2 __asm__("a2") = a3;
    register long a3 __asm__("a3") = a4;
    register long a7 __asm__("a7") = num;
    __asm__ __volatile__("ecall" : "=r"(ret) : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a7) : "memory");
#endif
    return ret;
}

void sys_exit(int status) {
    my_syscall1(SYS_exit, status);
    while (1); /* Protection */
}

long sys_write(int fd, void *buf, unsigned long count) {
    return (long)my_syscall3(SYS_write, fd, (long)buf, count);
}

long sys_read(int fd, void *buf, unsigned long count) {
    return (long)my_syscall3(SYS_read, fd, (long)buf, count);
}

int sys_open(char *pathname, int flags, int mode) {
#if defined(SYS_openat)
    return (int)my_syscall4(SYS_openat, -100, (long)pathname, flags, mode);
#else
    return (int)my_syscall3(SYS_open, (long)pathname, flags, mode);
#endif
}

int sys_close(int fd) {
    return (int)my_syscall1(SYS_close, fd);
}

long sys_lseek(int fd, long offset, int whence) {
    return (long)my_syscall3(SYS_lseek, fd, offset, whence);
}

int sys_stat(char *pathname, struct stat_custom *statbuf) {
#if defined(SYS_fstatat)
    return (int)my_syscall4(SYS_fstatat, -100, (long)pathname, (long)statbuf, 0);
#elif defined(SYS_stat64)
    return (int)my_syscall2(SYS_stat64, (long)pathname, (long)statbuf);
#else
    return (int)my_syscall2(SYS_newstat, (long)pathname, (long)statbuf);
#endif
}

int sys_mkdir(char *pathname, int mode) {
#if defined(SYS_mkdirat)
    return (int)my_syscall3(SYS_mkdirat, -100, (long)pathname, mode);
#else
    return (int)my_syscall2(SYS_mkdir, (long)pathname, mode);
#endif
}

int sys_getdents64(unsigned int fd, void *dirp, unsigned int count) {
    return (int)my_syscall3(SYS_getdents64, fd, (long)dirp, count);
}

int sys_chdir(char *path) {
    return (int)my_syscall1(SYS_chdir, (long)path);
}

#endif