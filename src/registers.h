#ifndef REGISTERS_H
#define REGISTERS_H

#include "cpu.h"

void set_register(CPU *cpu, const char *reg_name, uint16_t value);
uint16_t get_register(const CPU *cpu, const char *reg_name);
void print_registers(const CPU *cpu);

#endif // REGISTERS_H