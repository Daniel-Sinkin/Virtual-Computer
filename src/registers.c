#include "registers.h"
#include <stdio.h>
#include <string.h>

// clang-format off
void set_register(CPU *cpu, const char *reg_name, uint16_t value) {
    if (strcmp(reg_name, "AX") == 0) cpu->ax = value;
    else if (strcmp(reg_name, "BX") == 0) cpu->bx = value;
    else if (strcmp(reg_name, "CX") == 0) cpu->cx = value;
    else if (strcmp(reg_name, "DX") == 0) cpu->dx = value;
    else if (strcmp(reg_name, "SP") == 0) cpu->sp = value;
    else if (strcmp(reg_name, "BP") == 0) cpu->bp = value;
    else if (strcmp(reg_name, "SI") == 0) cpu->si = value;
    else if (strcmp(reg_name, "DI") == 0) cpu->di = value;
    else if (strcmp(reg_name, "IP") == 0) cpu->ip = value;
    else printf("Unknown register: %s\n", reg_name);
}

uint16_t get_register(const CPU *cpu, const char *reg_name) {
    if (strcmp(reg_name, "AX") == 0) return cpu->ax;
    else if (strcmp(reg_name, "BX") == 0) return cpu->bx;
    else if (strcmp(reg_name, "CX") == 0) return cpu->cx;
    else if (strcmp(reg_name, "DX") == 0) return cpu->dx;
    else if (strcmp(reg_name, "SP") == 0) return cpu->sp;
    else if (strcmp(reg_name, "BP") == 0) return cpu->bp;
    else if (strcmp(reg_name, "SI") == 0) return cpu->si;
    else if (strcmp(reg_name, "DI") == 0) return cpu->di;
    else if (strcmp(reg_name, "IP") == 0) return cpu->ip;
    else {
        printf("Unknown register: %s\n", reg_name);
        return 0;
    }
}

void print_registers(const CPU *cpu) {
    printf("Instruction Pointer: %04X\n", cpu->ip);
    printf("Stack Pointer:       %04X\n", cpu->sp);
    printf("AX: %04X  BX: %04X  CX: %04X  DX: %04X\n", cpu->ax, cpu->bx, cpu->cx, cpu->dx);
    printf("SP: %04X  BP: %04X  SI: %04X  DI: %04X\n", cpu->sp, cpu->bp, cpu->si, cpu->di);
}
// clang-format on