#include <stdio.h>
#include <stdlib.h>

#define TAPE_SIZE 30000

void execute_bf(const char *code) {
    unsigned char tape[TAPE_SIZE] = {0};
    unsigned char *ptr = tape;
    int pc = 0;
    int len = 0;
    while (code[len] != '\0') len++;

    while (code[pc] != '\0') {
        switch (code[pc]) {
            case '>':
                if (ptr < tape + TAPE_SIZE - 1) ptr++;
                break;
            case '<':
                if (ptr > tape) ptr--;
                break;
            case '+':
                (*ptr)++;
                break;
            case '-':
                (*ptr)--;
                break;
            case '.':
                putchar(*ptr);
                break;
            case ',': {
                int c = getchar();
                if (c != EOF) *ptr = (unsigned char)c;
                break;
            }
            case '[':
                if (*ptr == 0) {
                    int loop = 1;
                    while (loop > 0 && pc < len - 1) {
                        pc++;
                        if (code[pc] == '[') loop++;
                        else if (code[pc] == ']') loop--;
                    }
                }
                break;
            case ']':
                if (*ptr != 0) {
                    int loop = 1;
                    while (loop > 0 && pc > 0) {
                        pc--;
                        if (code[pc] == ']') loop++;
                        else if (code[pc] == '[') loop--;
                    }
                }
                break;
            default:
                break;
        }
        pc++;
    }
}

char *read_file(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "zsub: could not open '%s'\n", path);
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char *buf = malloc(size + 1);
    if (!buf) {
        fprintf(stderr, "zsub: out of memory\n");
        fclose(f);
        exit(1);
    }

    size_t read = fread(buf, 1, size, f);
    buf[read] = '\0';

    fclose(f);
    return buf;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        const char *hello_world =
            "+++++ +++++ [ > +++++ ++ > +++++ +++++ > +++ > + <<<< - ] "
            "> ++ . > + . +++++ ++ . . +++ . > ++ . << +++++ +++++ +++++ . "
            "> . +++ . ----- - . ----- --- . > + . > .";
        execute_bf(hello_world);
        return 0;
    }

    const char *path = argv[1];
    size_t len = 0;
    while (path[len] != '\0') len++;

    if (len < 2 || path[len - 2] != '.' || path[len - 1] != 'x') {
        fprintf(stderr, "zsub: '%s' is not a .x program\n", path);
        return 1;
    }

    char *code = read_file(path);
    execute_bf(code);
    free(code);

    return 0;
}