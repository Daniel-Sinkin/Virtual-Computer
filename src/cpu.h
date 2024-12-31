#ifndef CPU_H
#define CPU_H

#include <stdint.h>

#define MEMORY_SIZE 0xFFFF

typedef struct {
    uint16_t ax, bx, cx, dx;      // General-purpose registers
    uint16_t cs, ds, es, ss;      // Segment registers
    uint16_t sp, bp, si, di;      // Pointer/index registers
    uint16_t ip;                  // Instruction pointer
    uint16_t flags;               // Flags
    uint16_t memory[MEMORY_SIZE]; // Main memory
} CPU;

void initialize_cpu(CPU *cpu);
void print_registers(const CPU *cpu);

#endif // CPU_H