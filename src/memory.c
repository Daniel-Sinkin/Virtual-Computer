#include "memory.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void initialize_memory(uint16_t *memory) {
    memset(memory, 0, MEMORY_SIZE * sizeof(uint16_t));
}

uint16_t read_word(const uint16_t *memory, uint16_t address) {
    return memory[address];
}

void write_word(uint16_t *memory, uint16_t address, uint16_t value) {
    memory[address] = value;
}

void print_memory_bytes(const uint16_t *memory, uint16_t start_addr, uint16_t n_bytes) {
    printf("ADDR  | RAW BYTES                                        | STRING           |\n");
    printf("------|--------------------------------------------------|------------------|\n");

    for (uint16_t i = 0; i < n_bytes; i += 16) {
        printf("%04X  | ", start_addr + i);

        for (uint16_t j = 0; j < 16; ++j) {
            if (i + j < n_bytes) {
                printf("%02X ", ((uint8_t *)memory)[start_addr + i + j]);
            } else {
                printf("   ");
            }
        }

        printf(" | ");

        for (uint16_t j = 0; j < 16; ++j) {
            if (i + j < n_bytes) {
                uint8_t byte = ((uint8_t *)memory)[start_addr + i + j];
                printf("%c", isprint(byte) ? byte : '.');
            }
        }
        printf(" |\n");
    }
}

void print_memory_words(const uint16_t *memory, uint16_t start_addr, uint16_t n_words) {
    printf("ADDR  | Hexadecimal Values  | STRING   |\n");
    printf("------|---------------------|----------|\n");

    for (uint16_t i = 0; i < n_words; i += 4) {
        printf("%04X  | ", start_addr + i);

        for (uint16_t j = 0; j < 4; ++j) {
            if (i + j < n_words) {
                printf("%04X ", memory[start_addr / 2 + i + j]);
            } else {
                printf("     ");
            }
        }

        printf(" | ");

        for (uint16_t j = 0; j < 4; ++j) {
            if (i + j < n_words) {
                uint16_t word = memory[start_addr / 2 + i + j];
                char high_byte = (word >> 8) & 0xFF;
                char low_byte = word & 0xFF;

                printf("%c", isprint(high_byte) ? high_byte : '.');
                printf("%c", isprint(low_byte) ? low_byte : '.');
            }
        }
        printf("\n");
    }
}