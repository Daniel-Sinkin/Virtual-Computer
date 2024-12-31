#include "instruction.h"
#include <stdio.h>

// Encodes an instruction into memory at the specified address
uint16_t encode_instruction(CPU *cpu, uint16_t addr, uint8_t opcode, uint8_t mod, uint8_t reg, uint8_t rm, uint16_t imm) {
    uint16_t temp_addr = addr;
    cpu->memory[temp_addr++] = opcode;

    if (mod != 0 || reg != 0 || rm != 0) {
        cpu->memory[temp_addr++] = (mod << 6) | (reg << 3) | rm;
    }

    if (imm != 0) {
        cpu->memory[temp_addr++] = (imm & 0xFF);
        cpu->memory[temp_addr++] = ((imm >> 8) & 0xFF);
    }

    return temp_addr;
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