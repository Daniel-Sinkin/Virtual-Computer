#ifndef MEMORY_H
#define MEMORY_H

#include <stdbool.h>
#include <stdint.h>

#define MEMORY_SIZE 0xFFFF

void initialize_memory(uint16_t *memory);
uint16_t read_word(const uint16_t *memory, uint16_t address);
void write_word(uint16_t *memory, uint16_t address, uint16_t value);
void print_memory_bytes(const uint16_t *memory, uint16_t start_addr, uint16_t n_bytes);
void print_memory_words(const uint16_t *memory, uint16_t start_addr, uint16_t n_words);

#endif // MEMORY_H