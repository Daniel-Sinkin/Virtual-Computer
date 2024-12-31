#include "instruction.h"
#include <stdio.h>

typedef struct {
    uint16_t bytes[15];
    size_t length;
} EncodedInstruction;

// Encodes an instruction into memory at the specified address
EncodedInstruction encode_instruction_helper(uint8_t opcode, uint8_t modrm, uint8_t sib, int32_t displacement, int32_t immediate) {
    EncodedInstruction result = {{0}, 0};
    size_t index = 0;

    // Add the opcode
    result.bytes[index++] = opcode;

    // Add the ModR/M byte if provided
    if (modrm != 0) {
        result.bytes[index++] = modrm;
    }

    // Add the SIB byte if provided
    if (sib != 0) {
        result.bytes[index++] = sib;
    }

    // Add displacement if it's non-zero
    if (displacement != 0) {
        if (displacement >= -128 && displacement <= 127) {
            result.bytes[index++] = (uint8_t)(displacement & 0xFF); // 1-byte displacement
        } else {
            result.bytes[index++] = (uint8_t)((displacement >> 0) & 0xFF); // 4-byte displacement
            result.bytes[index++] = (uint8_t)((displacement >> 8) & 0xFF);
            result.bytes[index++] = (uint8_t)((displacement >> 16) & 0xFF);
            result.bytes[index++] = (uint8_t)((displacement >> 24) & 0xFF);
        }
    }

    // Add immediate if it's non-zero
    if (immediate != 0) {
        result.bytes[index++] = (uint8_t)((immediate >> 0) & 0xFF); // 4-byte immediate
        result.bytes[index++] = (uint8_t)((immediate >> 8) & 0xFF);
        result.bytes[index++] = (uint8_t)((immediate >> 16) & 0xFF);
        result.bytes[index++] = (uint8_t)((immediate >> 24) & 0xFF);
    }

    // Set the length of the instruction
    result.length = index;
    return result;
}

int encode_instructions(uint8_t *cpu_memory, size_t memory_size, uint8_t opcode, uint8_t modrm, uint8_t sib, int32_t displacement, int32_t immediate) {
    // Use the helper function to encode the instruction
    EncodedInstruction instruction = encode_instruction_helper(opcode, modrm, sib, displacement, immediate);

    if (instruction.length > memory_size) {
        // TODO: Handle this error case
        return -1;
    }

    memcpy(cpu_memory, instruction.bytes, instruction.length);
    return 0;
}

// Decodes a ModR/M byte
void decode_modrm(uint8_t modrm, uint8_t *mod, uint8_t *reg, uint8_t *rm) {
    *mod = (modrm >> 6) & 0x3;
    *reg = (modrm >> 3) & 0x7;
    *rm = modrm & 0x7;
}

// Executes one instruction based on the CPU's state
int execute_step(CPU *cpu) {
    uint8_t opcode = cpu->memory[cpu->ip++];
    uint8_t modrm = 0;
    uint8_t mod, reg, rm;

    // Decode ModR/M if the instruction uses it
    if (opcode == ADD || opcode == LOAD) {
        modrm = cpu->memory[cpu->ip++];
        decode_modrm(modrm, &mod, &reg, &rm);
    }

    switch (opcode) {
    case NOP:
        // Do nothing
        break;

    case LOAD: {
        // LOAD AX, imm or memory location
        uint16_t value = cpu->memory[cpu->ip++] | (cpu->memory[cpu->ip++] << 8);
        if (mod == MOD_REGISTER_DIRECT && reg == 0) {
            cpu->ax = value;
        }
        break;
    }

    case ADD: {
        // ADD AX, imm or memory location
        uint16_t src = (mod == MOD_REGISTER_DIRECT && reg == 0) ? cpu->ax : cpu->memory[rm];
        cpu->ax += src;
        break;
    }

    case HALT:
        // Stop the execution
        return 1;

    default:
        printf("Unknown opcode: %02X\n", opcode);
        return -1;
    }

    return 0;
}