#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <ctype.h>
#include <stdio.h>

#define MEMORY_SIZE (uint16_t)0xFFFF
#define NUM_REGISTERS (size_t)8

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

// Writes instruction to memory and returns the next free address
// Creates a local scope for the adjusted address and then "returns" that
#define write_instr(addr, opcode, mod, reg, rm, imm)          \
    ({                                                        \
        uint16_t temp_addr = (addr);                          \
        cpu.memory[temp_addr++] = (opcode & 0xFF);            \
        cpu.memory[temp_addr++] = encode_modrm(mod, reg, rm); \
        cpu.memory[temp_addr++] = (imm & 0xFF);               \
        cpu.memory[temp_addr++] = ((imm >> 8) & 0xFF);        \
        temp_addr;                                            \
    })

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
    // General-purpose registers (AX, BX, CX, DX)
    uint16_t ax; // Accumulator register
    uint16_t bx; // Base register
    uint16_t cx; // Count register
    uint16_t dx; // Data register

    // Segment registers (CS, DS, ES, SS)
    uint16_t cs; // Code segment
    uint16_t ds; // Data segment
    uint16_t es; // Extra segment
    uint16_t ss; // Stack segment

    // Pointer and index registers (SP, BP, SI, DI)
    uint16_t sp; // Stack pointer
    uint16_t bp; // Base pointer
    uint16_t si; // Source index
    uint16_t di; // Destination index

    // Instruction Pointer
    uint16_t ip; // Instruction pointer, this is the "program counter"

    // Flags register
    uint16_t flags; // Status and control flags

    // Main memory
    uint16_t memory[MEMORY_SIZE]; // Main memory (default 256 words)
} CPU;

CPU cpu;

bool is_running = false;

typedef enum {
    NOP = 0x00,
    LOAD = 0x01,
    ADD = 0x02,
    HALT = 0xFF
} OPCODE;

// TODO: Fix alignment
void print_memory_bytes(uint16_t start_addr, uint16_t n_bytes) {
    printf("ADDR  | RAW BYTES         | STRING   |\n");
    printf("------|-------------------|----------|\n");

    for (uint16_t i = 0; i < n_bytes; i += 16) {
        printf("%04X  | ", start_addr + i);

        // Print raw bytes
        for (uint16_t j = 0; j < 16; ++j) {
            if (i + j < n_bytes) {
                printf("%02X ", ((uint8_t *)cpu.memory)[start_addr + i + j]);
            } else {
                printf("   ");
            }
        }

        printf(" | ");

        // Print ASCII representation
        for (uint16_t j = 0; j < 16; ++j) {
            if (i + j < n_bytes) {
                uint8_t byte = ((uint8_t *)cpu.memory)[start_addr + i + j];
                printf("%c", isprint(byte) ? byte : '.');
            }
        }
        printf("\n");
    }
}

// TODO: Fix alignment
void print_memory_words(uint16_t start_addr, uint16_t n_words) {
    printf("ADDR  | Hexadecimal Values  | STRING   |\n");
    printf("------|---------------------|----------|\n");

    for (uint16_t i = 0; i < n_words; i += 4) {
        printf("%04X  | ", start_addr + i);

        // Print raw words
        for (uint16_t j = 0; j < 4; ++j) {
            if (i + j < n_words) {
                printf("%04X ", cpu.memory[start_addr / 2 + i + j]);
            } else {
                printf("     ");
            }
        }

        printf(" | ");

        // Print ASCII representation (high and low bytes of each word)
        for (uint16_t j = 0; j < 4; ++j) {
            if (i + j < n_words) {
                uint16_t word = cpu.memory[start_addr / 2 + i + j];
                char high_byte = (word >> 8) & 0xFF;
                char low_byte = word & 0xFF;

                printf("%c", isprint(high_byte) ? high_byte : '.');
                printf("%c", isprint(low_byte) ? low_byte : '.');
            }
        }
        printf("\n");
    }
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

void print_registers(uint16_t n_words, uint16_t words_per_row) {
    printf("Instruction Pointer: %04X\n", cpu.ip);
    printf("Stack Pointer:       %04X\n", cpu.sp);
    printf("\nGeneral Registers:\n");
    printf("AX: %04X  BX: %04X  CX: %04X  DX: %04X\n", cpu.ax, cpu.bx, cpu.cx, cpu.dx);
    printf("\nSegment Registers:\n");
    printf("CS: %04X  DS: %04X  ES: %04X  SS: %04X\n", cpu.cs, cpu.ds, cpu.es, cpu.ss);
    printf("\nPointer/Index Registers:\n");
    printf("SP: %04X  BP: %04X  SI: %04X  DI: %04X\n", cpu.sp, cpu.bp, cpu.si, cpu.di);
}

void initialize_cpu() {
    cpu.ax = cpu.bx = cpu.cx = cpu.dx = 0;
    cpu.cs = cpu.ds = cpu.es = cpu.ss = 0;
    cpu.sp = cpu.bp = cpu.si = cpu.di = 0;
    cpu.ip = 0;
    for (int i = 0; i < MEMORY_SIZE; i++) {
        cpu.memory[i] = 0;
    }
}

void hello_world() {
    initialize_cpu();

    write_bytes(0, 'H', 'e');
    write_bytes(1, 'l', 'l');
    write_bytes(2, 'o', ',');
    write_bytes(3, ' ', 'W');
    write_bytes(4, 'o', 'r');
    write_bytes(5, 'l', 'd');
    print_memory_bytes(0, 12 * 2);
}

uint16_t read_word() {
    return cpu.memory[cpu.ip++];
}

int execute_step() {
    uint8_t opcode = cpu.memory[cpu.ip] & 0xFF;
    uint8_t modrm = (cpu.memory[cpu.ip + 1]) & 0xFF;
    uint8_t mod, reg, rm;

    decode_modrm(modrm, &mod, &reg, &rm);
    cpu.ip += 2;

    switch (opcode) {
    case LOAD: {
        uint16_t value = cpu.memory[cpu.ip] | (cpu.memory[cpu.ip + 1] << 8);
        cpu.ax = value;
        cpu.ip += 2;
        break;
    }

    case ADD: {
        uint16_t src = (mod == 0b11) ? cpu.ax : cpu.memory[rm];
        cpu.ax += src;
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

    // Write instructions
    uint16_t pc = 0;
    pc = write_instr(pc, LOAD, MOD_REGISTER_DIRECT, 0, 0, 42); // LOAD AX, 42
    pc = write_instr(pc, LOAD, MOD_REGISTER_DIRECT, 1, 0, 99); // LOAD BX, 99

    // Dump memory for debugging
    printf("\nInstruction Memory (Byte-Oriented):\n");
    print_memory_bytes(0, pc * 2);

    printf("\nData Memory (Word-Oriented):\n");
    print_memory_words(0, pc);

    return 0;
}