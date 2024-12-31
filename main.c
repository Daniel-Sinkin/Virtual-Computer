#include "cpu.h"
#include "instruction.h"
#include "memory.h"
#include "registers.h"
#include <stdio.h>

void test_encode_instruction_helper_valid_inputs(void);
void test_encode_instruction_helper_with_displacement_and_immediate(void);

int main() {
    UNITY_BEGIN(); // Start Unity test runner

    RUN_TEST(test_encode_instruction_helper_valid_inputs);
    RUN_TEST(test_encode_instruction_helper_with_displacement_and_immediate);

    return UNITY_END(); // End Unity test runner
}
/*
    CPU cpu;
    initialize_cpu(&cpu);

    uint16_t pc = 0;
    pc = encode_instruction(&cpu, pc, LOAD, MOD_REGISTER_DIRECT, 0, 0, 42); // LOAD AX, 255
    pc = encode_instruction(&cpu, pc, LOAD, MOD_REGISTER_DIRECT, 1, 0, 99); // LOAD BX, 99
    pc = encode_instruction(&cpu, pc, ADD, MOD_REGISTER_DIRECT, 0, 1, 0);   // ADD AX, BX
    pc = encode_instruction(&cpu, pc, HALT, 0, 0, 0, 0);                    // HALT

    printf("Initial CPU State:\n");
    print_registers(&cpu);
    printf("\nMemory State:\n");
    print_memory_words(cpu.memory, 0, 32);

    printf("\nExecuting instructions...\n");
    bool is_running = true;
    if (execute_step(&cpu) != 0) {
        printf("Error during execution. Halting.\n");
        return 1;
    }
    print_registers(&cpu);
    print_memory_words(cpu.memory, 0, 32);
    if (execute_step(&cpu) != 0) {
        printf("Error during execution. Halting.\n");
        return 1;
    }
    print_registers(&cpu);
    print_memory_words(cpu.memory, 0, 32);

    return 0;
*/