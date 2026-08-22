#include <stdio.h>
#include "zrt.h"

void zrt_execute(const char *code) {
    unsigned char tape[ZRT_TAPE_SIZE] = {0};
    unsigned char *ptr = tape;
    int pc = 0;
    int len = 0;
    while (code[len] != '\0') len++;

    while (code[pc] != '\0') {
        switch (code[pc]) {
            case '>':
                if (ptr < tape + ZRT_TAPE_SIZE - 1) ptr++;
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