#include "cpu.h"
#include "instruction.h"
#include "memory.h"
#include "registers.h"
#include <stdio.h>

int main() {
    CPU cpu;
    initialize_cpu(&cpu);

    uint16_t pc = 0;
    pc = encode_instruction(&cpu, pc, LOAD, MOD_REGISTER_DIRECT, 0, 0, 0xFF); // LOAD AX, 255
    pc = encode_instruction(&cpu, pc, LOAD, MOD_REGISTER_DIRECT, 1, 0, 99);   // LOAD BX, 99
    pc = encode_instruction(&cpu, pc, ADD, MOD_REGISTER_DIRECT, 0, 1, 0);     // ADD AX, BX
    pc = encode_instruction(&cpu, pc, HALT, 0, 0, 0, 0);                      // HALT

    printf("Initial CPU State:\n");
    print_registers(&cpu);
    printf("\nMemory State:\n");
    print_memory_bytes(cpu.memory, 0, 128);

    return 0;

    printf("\nExecuting instructions...\n");
    bool is_running = true;
    if (execute_step(&cpu) != 0) {
        printf("Error during execution. Halting.\n");
        return 1;
    }

    printf("\nFinal CPU State:\n");
    print_registers(&cpu);

    return 0;
}