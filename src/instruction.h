#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stddef.h>

#include "cpu.h"

typedef struct {
    uint16_t bytes[15];
    size_t length;
} EncodedInstruction;

// Enum for MOD values
typedef enum {
    MOD_REGISTER_INDIRECT = 0b00,
    MOD_INDIRECT_DISP8 = 0b01,
    MOD_INDIRECT_DISP16 = 0b10,
    MOD_REGISTER_DIRECT = 0b11
} MOD;

// Opcode constants
typedef enum {
    NOP = 0x00,
    LOAD = 0x01,
    ADD = 0x02,
    HALT = 0xFF
} OPCODE;

// Function prototypes
EncodedInstruction encode_instruction_helper(uint8_t opcode, uint8_t modrm, uint8_t sib, int32_t displacement, int32_t immediate);
uint16_t encode_instruction(CPU *cpu, uint16_t addr, uint8_t opcode, uint8_t mod, uint8_t reg, uint8_t rm, uint16_t imm);
void decode_modrm(uint8_t modrm, uint8_t *mod, uint8_t *reg, uint8_t *rm);
int execute_step(CPU *cpu);

#endif // INSTRUCTION_H