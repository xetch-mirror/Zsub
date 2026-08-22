#include "zrt.h"
#include "syscalls.h"

#define ZSUB_BUF_SIZE 65536

static char g_buf[ZSUB_BUF_SIZE];

static long zsub_strlen(char *s) {
    long n = 0;
    while (s[n] != '\0') n++;
    return n;
}

static void zsub_err(char *msg) {
    sys_write(2, msg, zsub_strlen(msg));
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        char *hello_world =
            "+++++ +++++ [ > +++++ ++ > +++++ +++++ > +++ > + <<<< - ] "
            "> ++ . > + . +++++ ++ . . +++ . > ++ . << +++++ +++++ +++++ . "
            "> . +++ . ----- - . ----- --- . > + . > .";
        zrt_execute(hello_world);
        return 0;
    }

    char *path = argv[1];
    long len = zsub_strlen(path);

    if (len < 2 || path[len - 2] != '.' || path[len - 1] != 'x') {
        zsub_err("zsub: not a .x program\n");
        return 1;
    }

    int fd = sys_open(path, O_RDONLY, 0);
    if (fd < 0) {
        zsub_err("zsub: could not open file\n");
        return 1;
    }

    long total = 0;
    long n;
    while (total < ZSUB_BUF_SIZE - 1 &&
           (n = sys_read(fd, g_buf + total, ZSUB_BUF_SIZE - 1 - total)) > 0) {
        total += n;
    }
    g_buf[total] = '\0';
    sys_close(fd);

    zrt_execute(g_buf);

    return 0;
}