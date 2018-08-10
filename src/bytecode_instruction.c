#include "bytecode_instruction.h"
#include "bytecode_instruction_handler.h"
#include "bytecode_runner.h"
#include "bytecode_opcode.h"

#include <stdio.h>

static bytecode_instruction_handler *instruction_handlers[BYTECODE_OPCODE_COUNT] = {
    [BYTECODE_OPCODE_HALT]          = exec_op_halt,
    [BYTECODE_OPCODE_PUSH_INT]      = exec_op_push_int,
    [BYTECODE_OPCODE_PUSH_FLOAT]    = exec_op_push_float,
    [BYTECODE_OPCODE_ADD_INT_IMM]   = exec_op_add_int_imm,
    [BYTECODE_OPCODE_ADD_FLOAT_IMM] = exec_op_add_float_imm,
    [BYTECODE_OPCODE_PUSH_REG]      = exec_op_push_reg,
    [BYTECODE_OPCODE_POP_REG]       = exec_op_pop_reg,
};

void execute_instruction(struct bytecode_runner *bcr, struct bytecode_instruction instr)
{
    if (instruction_handlers[instr.op]) {
        instruction_handlers[instr.op](bcr, instr.op, instr.r1, instr.r2);
    } else {
        fprintf(stderr, "%2X: missing instruction handler\n", instr.op);
    }
}

uint64_t fetch_instruction(struct bytecode_runner *bcr)
{
    return bcr->text[bcr->reg[BYTECODE_REGISTER_RIP]._u64++];
}

struct bytecode_instruction decode_instruction(uint64_t raw_instr)
{
    struct bytecode_instruction instr;
    instr.op = (raw_instr & 0xff00000000000000) >> 56;
    instr.r1 = (raw_instr & 0x00f0000000000000) >> 52;
    instr.r2 = (raw_instr & 0x000f000000000000) >> 48;
    return instr;
}

uint64_t encode_instruction(uint8_t instr)
{
    uint64_t result = (uint64_t)instr << 56;
    return result;
}

uint64_t encode_instruction_r1(uint8_t instr, uint8_t r1)
{
    uint64_t result = ((uint64_t)instr << 56) | ((uint64_t)r1 << 52);
    return result;
}

uint64_t encode_instruction_r2(uint8_t instr, uint8_t r1, uint8_t r2)
{
    uint64_t result = ((uint64_t)instr << 56) | ((uint64_t)r1 << 52) | ((uint64_t)r2 << 48);
    return result;
}
