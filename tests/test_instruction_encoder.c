#include "instruction.h" // Include your function prototypes
#include "unity.h"

// Test setup function (optional)
void setUp(void) {}

// Test teardown function (optional)
void tearDown(void) {}

void test_encode_instruction_helper_valid_inputs(void) {
    uint8_t opcode = 0x90; // NOP
    uint8_t modrm = 0x00;
    uint8_t sib = 0x00;
    int32_t displacement = 0;
    int32_t immediate = 0;

    EncodedInstruction result = encode_instruction_helper(opcode, modrm, sib, displacement, immediate);

    TEST_ASSERT_EQUAL_UINT8(1, result.length);
    TEST_ASSERT_EQUAL_UINT16(opcode, result.bytes[0]);
}

void test_encode_instruction_helper_with_displacement_and_immediate(void) {
    uint8_t opcode = 0xC7; // MOV
    uint8_t modrm = 0x00;
    uint8_t sib = 0x00;
    int32_t displacement = 0x1234;
    int32_t immediate = 0x56789ABC;

    EncodedInstruction result = encode_instruction_helper(opcode, modrm, sib, displacement, immediate);

    TEST_ASSERT_EQUAL_UINT8(10, result.length);
    TEST_ASSERT_EQUAL_UINT16(opcode, result.bytes[0]);
    TEST_ASSERT_EQUAL_UINT16(modrm, result.bytes[1]);
    TEST_ASSERT_EQUAL_UINT16(0x34, result.bytes[2]); // Displacement LSB
    TEST_ASSERT_EQUAL_UINT16(0x12, result.bytes[3]); // Displacement MSB
    TEST_ASSERT_EQUAL_UINT16(0xBC, result.bytes[4]); // Immediate LSB
    TEST_ASSERT_EQUAL_UINT16(0x9A, result.bytes[5]);
    TEST_ASSERT_EQUAL_UINT16(0x78, result.bytes[6]);
    TEST_ASSERT_EQUAL_UINT16(0x56, result.bytes[7]);
}