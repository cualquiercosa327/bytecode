#include "bytecode_instruction.h"
#include "bytecode_instruction_handler.h"
#include "bytecode_runner.h"
#include "bytecode_opcode.h"

#include <stdio.h>

static bytecode_instruction_handler *instruction_handlers[BYTECODE_OPCODE_COUNT] = {
    [BYTECODE_OPCODE_HALT]             = exec_op_halt,

    [BYTECODE_OPCODE_MOVU_REG_IMM]     = exec_op_movu_reg_imm,
    [BYTECODE_OPCODE_MOVI_REG_IMM]     = exec_op_movi_reg_imm,
    [BYTECODE_OPCODE_MOVF_REG_IMM]     = exec_op_movf_reg_imm,
    [BYTECODE_OPCODE_MOV_REG_REG]      = exec_op_mov_reg_reg,

    [BYTECODE_OPCODE_PUSHU_IMM]        = exec_op_pushu_imm,
    [BYTECODE_OPCODE_PUSHI_IMM]        = exec_op_pushi_imm,
    [BYTECODE_OPCODE_PUSHF_IMM]        = exec_op_pushf_imm,
    [BYTECODE_OPCODE_PUSH_REG]         = exec_op_push_reg,
    [BYTECODE_OPCODE_POP_REG]          = exec_op_pop_reg,

    [BYTECODE_OPCODE_ADDU_REG_IMM]     = exec_op_addu_reg_imm,
    [BYTECODE_OPCODE_ADDI_REG_IMM]     = exec_op_addi_reg_imm,
    [BYTECODE_OPCODE_ADDF_REG_IMM]     = exec_op_addf_reg_imm,
    [BYTECODE_OPCODE_ADD_REG_REG]      = exec_op_add_reg_reg,

    [BYTECODE_OPCODE_SUBU_REG_IMM]     = exec_op_subu_reg_imm,
    [BYTECODE_OPCODE_SUBI_REG_IMM]     = exec_op_subi_reg_imm,
    [BYTECODE_OPCODE_SUBF_REG_IMM]     = exec_op_subf_reg_imm,
    [BYTECODE_OPCODE_SUB_REG_REG]      = exec_op_sub_reg_reg,

    [BYTECODE_OPCODE_MULU_REG_IMM]     = exec_op_mulu_reg_imm,
    [BYTECODE_OPCODE_MULI_REG_IMM]     = exec_op_muli_reg_imm,
    [BYTECODE_OPCODE_MULF_REG_IMM]     = exec_op_mulf_reg_imm,
    [BYTECODE_OPCODE_MUL_REG_REG]      = exec_op_mul_reg_reg,

    [BYTECODE_OPCODE_NEG_REG]          = exec_op_neg_reg,
    [BYTECODE_OPCODE_INC_REG]          = exec_op_inc_reg,
    [BYTECODE_OPCODE_DEC_REG]          = exec_op_dec_reg,
    [BYTECODE_OPCODE_XOR_REG_REG]      = exec_op_xor_reg_reg,

    [BYTECODE_OPCODE_CALL_IMM]         = exec_op_call_imm,
    [BYTECODE_OPCODE_CALL_REG]         = exec_op_call_reg,

    [BYTECODE_OPCODE_ENTER]            = exec_op_enter,
    [BYTECODE_OPCODE_LEAVE]            = exec_op_leave,
    [BYTECODE_OPCODE_STORE_LOCAL_IMM]  = exec_op_store_local_imm,
    [BYTECODE_OPCODE_STORE_LOCAL_REG]  = exec_op_store_local_reg,
    [BYTECODE_OPCODE_LOAD_LOCAL_IMM]   = exec_op_load_local_imm,
    [BYTECODE_OPCODE_LOAD_LOCAL_REG]   = exec_op_load_local_reg,

    [BYTECODE_OPCODE_NOP]              = exec_op_nop
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
    instr.r1 = (raw_instr & 0x00ff000000000000) >> 48;
    instr.r2 = (raw_instr & 0x0000ff0000000000) >> 40;
    return instr;
}

uint64_t encode_instruction(uint8_t instr)
{
    uint64_t result = (uint64_t)instr << 56;
    return result;
}

uint64_t encode_instruction_r1(uint8_t instr, uint8_t r1)
{
    uint64_t result = ((uint64_t)instr << 56) | ((uint64_t)r1 << 48);
    return result;
}

uint64_t encode_instruction_r2(uint8_t instr, uint8_t r1, uint8_t r2)
{
    uint64_t result = ((uint64_t)instr << 56) | ((uint64_t)r1 << 48) | ((uint64_t)r2 << 40);
    return result;
}
