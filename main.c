#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <ctype.h>
#include <stdio.h>

#define MEMORY_SIZE (uint16_t)0xFFFF
#define NUM_REGISTERS (size_t)8
#define print_memory(x) _print_memory(x, 4)
#define print_cpu(x) _print_cpu(x, 4)
#define get_opcode(addr) get_upper(addr)
#define get_operand(addr) get_lower(addr)

// x86 ISA specifics

// Intel® 64 and IA-32 Architectures Software Developer's Manual, Volume 2: Instruction Set Reference, A-Z
// https://cdrdv2-public.intel.com/825760/325383-sdm-vol-2abcd.pdf

// https://en.wikipedia.org/wiki/ModR/M

typedef enum {
    MOD_REGISTER_INDIRECT = 0b00, // Operand is a regsiter
    MOD_INDIRECT_DISP8 = 0b01,    // Operand is memory address in a register
    MOD_INDIRECT_DISP16 = 0b10,   // Memory addres + 8-bit displacement
    MOD_REGISTER_DIRECT = 0b11    // Memory Address + 16-bit displacement
} MOD;

#define write_instr(addr, opcode, mod, reg, rm, imm)        \
    do {                                                    \
        uint16_t temp_addr = addr;                          \
        cpu.memory[temp_addr] = (opcode & 0xF);             \
        temp_addr = temp_addr + 1;                          \
        cpu.memory[temp_addr] = encode_modrm(mod, reg, rm); \
        temp_addr = temp_addr + 1;                          \
        if (imm != 0) {                                     \
            cpu.memory[temp_addr] = imm & 0xFF;             \
            temp_addr = temp_addr + 1;                      \
            cpu.memory[temp_addr] = (imm >> 8) & 0xFF;      \
            temp_addr = temp_addr + 1;                      \
        }                                                   \
    } while (0)

uint8_t encode_modrm(uint8_t mod, uint8_t reg, uint8_t rm) {
    return (mod << 6) | (reg << 3) | rm;
}

void decode_modrm(uint8_t modrm, uint8_t *mod, uint8_t *reg, uint8_t *rm) {
    *mod = (modrm >> 6) & 0x3;
    *reg = (modrm >> 3) & 0x7;
    *rm = modrm & 0x7;
}

// End of x86 ISA specifics
typedef struct {
    uint16_t registers[NUM_REGISTERS];
    uint16_t pc;    // Program counter
    uint16_t sp;    // Stack pointer
    uint16_t flags; // Status flags

    uint16_t memory[MEMORY_SIZE]; // Main memory (for now 256 "words")
} CPU;

CPU cpu;

bool is_running = false;
bool is_initialized = false;

typedef enum {
    NOP = 0x00,
    LOAD = 0x01,
    ADD = 0x02,
    HALT = 0xFF
} OPCODE;

uint16_t read_word() {
    return cpu.memory[cpu.pc++];
}

void _print_memory(uint16_t n_words, uint16_t words_per_row) {
    if (n_words > MEMORY_SIZE) {
        n_words = MEMORY_SIZE;
    }

    printf("ADDR | Hexadecimal Values  | STRING   |\n");
    printf("-----|---------------------|----------|\n");

    for (int i = 0; i < n_words; i += words_per_row) {
        printf("%04X | ", i);

        for (int j = 0; j < words_per_row; j++) {
            if (i + j < n_words) {
                printf("%04X ", cpu.memory[i + j]);
            } else {
                // Padding
                printf("     ");
            }
        }

        printf("| ");

        for (int j = 0; j < words_per_row; j++) {
            if (i + j < n_words) {
                uint16_t value = cpu.memory[i + j];
                char high_byte = (value >> 8) & 0xFF;
                char low_byte = value & 0xFF;

                printf("%c", isprint(high_byte) ? high_byte : '.');
                printf("%c", isprint(low_byte) ? low_byte : '.');
            } else {
                // Padding
                printf(" ");
            }
        }
        // Padding
        for (int k = 0; k < i + words_per_row - n_words; k++) {
            printf(" ");
        }

        printf(" |\n");
    }
}

void _print_cpu(uint16_t n_words, uint16_t words_per_row) {
    printf("Program Counter: %02X\n", cpu.pc);
    printf("Stack Pointer:   %02X\n", cpu.sp);
    printf("\n");
    printf("Registers:\n");
    for (size_t i = 0; i < NUM_REGISTERS; i++) {
        printf("    %zu: %02X\n", i, cpu.registers[i]);
    }
    printf("\n");
    if (n_words > 0) {
        _print_memory(n_words, words_per_row);
    }
}

void initialize_cpu() {
    for (int i = 0; i < 8; i++) {
        cpu.registers[i] = 0;
    }

    cpu.pc = 0;

    for (int i = 0; i < MEMORY_SIZE; i++) {
        cpu.memory[i] = 0;
    }

    is_initialized = true;
}

void write_upper_byte(uint16_t addr, uint8_t val) {
    cpu.memory[addr] |= val;
}

void write_lower_byte(uint16_t addr, uint8_t val) {
    cpu.memory[addr] |= val << 8;
}

void write_bytes(uint16_t addr, uint16_t lower, uint16_t upper) {
    write_upper_byte(addr, upper);
    write_lower_byte(addr, lower);
}

uint8_t get_lower(uint16_t addr) {
    return (cpu.memory[addr] >> 8) & 0xF;
}

uint8_t get_upper(uint16_t addr) {
    return cpu.memory[addr] & 0xF;
}

void hello_world() {
    initialize_cpu();

    write_bytes(0, 'H', 'e');
    write_bytes(1, 'l', 'l');
    write_bytes(2, 'o', ',');
    write_bytes(3, ' ', 'W');
    write_bytes(4, 'o', 'r');
    write_bytes(5, 'l', 'd');

    print_memory(12);

    initialize_cpu();
}

int execute_step() {
    uint8_t opcode = get_opcode(cpu.pc);
    uint8_t modrm = cpu.memory[cpu.pc + 1];
    uint8_t mod, reg, rm;
    decode_modrm(modrm, &mod, &reg, &rm);
    cpu.pc += 2;

    switch (opcode) {
    case LOAD: {
        uint16_t value = read_word();
        cpu.registers[reg] = value;
        cpu.pc += 2;
        break;
    }

    case ADD: {
        uint16_t src = (mod == 0b11) ? cpu.registers[rm] : cpu.memory[rm];
        cpu.registers[reg] += src;
        break;
    }

    case HALT: {
        is_running = false;
        break;
    }

    default:
        printf("Unknown opcode: %02X\n", opcode);
        return -1;
    }

    return 0;
}

int main() {
    initialize_cpu();
    is_running = true;

    write_instr(0, LOAD, MOD_REGISTER_DIRECT, 0, 0, 42);
    write_instr(1, ADD, MOD_REGISTER_DIRECT, 0, 1, 0);
    write_instr(2, HALT, 0, 0, 0, 1);

    // print_cpu(12);

    hello_world();

    return 0;
}