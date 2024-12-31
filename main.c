#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define MEMORY_SIZE (uint16_t)0xFFFF
#define print_memory(x) _print_memory(x, 4)

// Notation for bits in the 16 bit words is just using hexcode:
// [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F]
// Z denotes a value of 0 (zero)
// U denotes a value of 1 (uno)
typedef struct {
    uint16_t registers[8];
    uint16_t pc;    // Program counter
    uint16_t sp;    // Stack pointer
    uint16_t flags; // Status flags

    uint16_t memory[MEMORY_SIZE]; // Main memory (for now 256 "words")
} CPU;

CPU cpu;

typedef enum {
    NOP = 0x00,
    LOAD = 0x01,
    ADD = 0x02,
    HALT = 0xFF
} OPCODE;

uint16_t read_word() {
    return cpu.memory[cpu.pc++];
}

#include <ctype.h> // For isprint()
#include <stdio.h>

void _print_memory(uint16_t n_words, uint16_t bytes_per_row) {
    if (n_words > MEMORY_SIZE) {
        n_words = MEMORY_SIZE;
    }

    printf("ADDR | Hexadecimal Values  | STRING   |\n");
    printf("-----|---------------------|----------|\n");

    for (int i = 0; i < n_words; i += bytes_per_row) {
        printf("%04X | ", i);

        for (int j = 0; j < bytes_per_row; j++) {
            if (i + j < n_words) {
                printf("%04X ", cpu.memory[i + j]);
            } else {
                // Padding
                printf("     ");
            }
        }

        printf("| ");

        for (int j = 0; j < bytes_per_row; j++) {
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
        for (int k = 0; k < i + bytes_per_row - n_words; k++) {
            printf(" ");
        }

        printf(" |\n");
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

int main() {
    hello_world();

    return 0;
}